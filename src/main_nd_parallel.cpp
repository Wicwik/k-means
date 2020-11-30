#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <math.h>   
#include <omp.h>
#include <mpi.h>

// default values
unsigned int DIMS = 3;
unsigned int CLUSTERS = 4;
unsigned int DATA_SIZE = 400;
int THREADS = 4;
bool STDOUT = true;

int proc_count = 0; 
int mpirank = 0;
unsigned int data_per_proc = 0;

double *allocate_array_double(unsigned int x)
{
	return (double *)malloc(x * sizeof(double));
}

double distance(double *centroids, double *points, unsigned int dims, unsigned int c_point, unsigned int p_point)
{
	double distance = 0;
	for (unsigned int i = 0; i < dims; i++)
	{
		distance+= (centroids[c_point*dims + i] - points[p_point*dims + i]) * (centroids[c_point*dims + i] - points[p_point*dims + i]);
	}

	return distance;
}

bool kmeans_clustering(double **points, double **centroids, unsigned int data_size, unsigned int cluster_count, unsigned int dims, int **points_and_clusters, double *wcss)
{
	*wcss = 0.0;

	double *minimal_distance = allocate_array_double(data_size);
	for (unsigned int i = 0; i < data_size; i++)
	{
		minimal_distance[i] = __DBL_MAX__;
	}
	
	double *sum_cordinates = allocate_array_double(cluster_count*dims);
	
	for (unsigned int i = 0; i < cluster_count; i++)
	{
		for (unsigned int j = 0; j < dims; j++)
		{
			sum_cordinates[i*dims + j] = 0;
		}
	}

	int *points_count = (int *)malloc(cluster_count * sizeof(int));
	for (unsigned int i = 0; i < cluster_count; i++)
	{
		points_count[i] = 0;
	}

	omp_set_dynamic(0);
	omp_set_num_threads(THREADS);
	#pragma omp parallel default(none) shared(points_and_clusters, minimal_distance, sum_cordinates, points_count, cluster_count, wcss) firstprivate(data_size, dims, centroids, points)
	{
		#pragma omp for schedule(static) reduction(+: sum_cordinates[:cluster_count*dims], points_count[:cluster_count]) collapse(2)
		for (unsigned int i = 0; i < cluster_count; i++)
		{
			for (unsigned int j = 0; j < data_size; j++)
			{
				
				double dist = distance(*centroids, *points, dims, i, j); // calculate square distance

				if (dist < minimal_distance[j])
				{
					if ((*points_and_clusters)[j] >= 0) // if point was asigned to a centroid before
					{
						int old = (*points_and_clusters)[j];
						for (unsigned int k = 0; k < dims; k++)
						{
							sum_cordinates[(unsigned int)old*dims + k] -= (*points)[j*dims + k];
						}

						#pragma omp critical
						{
							*wcss -= dist;
						}

						points_count[old]--;
				
					}

					minimal_distance[j] = dist;
					(*points_and_clusters)[j] = (int)i;

					for (unsigned int k = 0; k < dims; k++)
					{
						sum_cordinates[i*dims + k] += (*points)[j*dims + k]; // for each centroid add its coordinates
					}

					#pragma omp critical
					{
						*wcss += dist;
					}

					points_count[i]++;
				}

			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	double *reduced_sum_cordinates = allocate_array_double(cluster_count*dims);
	int *reduced_points_count = (int *)malloc(cluster_count * sizeof(int));

	MPI_Allreduce(sum_cordinates, reduced_sum_cordinates, (int)cluster_count*(int)dims, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	MPI_Allreduce(points_count, reduced_points_count, (int)cluster_count, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	// calculate new centroid cordinates
	// this will be done after reduce

	double *old_centroids = allocate_array_double(cluster_count*dims);

	#pragma omp parallel for schedule(static) default(none) shared(centroids, old_centroids) firstprivate(reduced_sum_cordinates, reduced_points_count, dims, cluster_count) collapse(2)
	for (unsigned int i = 0; i < cluster_count; i++)
	{
		for (unsigned int j = 0; j < dims; j++)
		{
			if (std::isnan(reduced_sum_cordinates[i*dims + j]/reduced_points_count[i]) || std::isinf(reduced_sum_cordinates[i*dims + j]/reduced_points_count[i]))
			{
				continue;
			}

			old_centroids[i*dims + j] = (*centroids)[i*dims + j];
			(*centroids)[i*dims + j] = (reduced_sum_cordinates[i*dims + j]/reduced_points_count[i]);
		}

	}

	MPI_Barrier(MPI_COMM_WORLD);

	for (unsigned int i = 0; i < cluster_count; i++)
	{
		// if (mpirank == 0)
		// {
		// 	std::cout << "old: ";
		// 	for (unsigned int j = 0; j < dims; j++)
		// 	{
		// 		std::cout << old_centroids[i*dims + j] << " ";
		// 	}

		// 	std::cout << std::endl;

		// 	std::cout << "new: ";
		// 	for (unsigned int j = 0; j < dims; j++)
		// 	{
		// 		std::cout << (*centroids)[i*dims + j] << " ";
		// 	}

		// 	std::cout << std::endl;
		// }

		for (unsigned int j = 0; j < dims; j++)
		{
			
			if ((*centroids)[i*dims + j] != old_centroids[i*dims + j]) // if one of new cords does not equal old cords
			{
				free(minimal_distance);
				free(sum_cordinates);
				free(points_count);
				free(reduced_sum_cordinates);
				free(reduced_points_count);
				free(old_centroids);
				return true; // continue algorithm
			}
		}
	}

	if (mpirank == 0 && STDOUT)
	{
		for (unsigned int i = 0; i < cluster_count; i++)
		{
			std::cout << "Centroid " << i+1 << ": [ "; 
			for (unsigned int j = 0; j < dims; j++)
			{
				std::cout << (*centroids)[i*dims + j] << ", ";

				if (j == dims-1)
				{
					std::cout << (*centroids)[i*dims + j] << " ]" << std::endl;
				}
			}
		}
	}

	free(minimal_distance);
	free(sum_cordinates);
	free(points_count);
	free(reduced_sum_cordinates);
	free(reduced_points_count);
	free(old_centroids);
	return false; // stop algorithm
}

int* do_kmeans(double **points, unsigned int data_size, unsigned int cluster_count, unsigned int dims)
{
	double *centroids = allocate_array_double(cluster_count*dims);

	// choose random points as centroids
	srand(std::time(0)); 
	for (unsigned int i = 0; i < cluster_count; i++) 
	{
		unsigned int selected = static_cast<unsigned int>(rand()) % data_size;

    	for (unsigned int j = 0; j < dims; j++)
    	{
    		centroids[i*dims + j] = (*points)[selected*dims + j];
    	}
	}
	
	int *points_and_clusters = (int *)malloc(data_size * sizeof(int));
	for (unsigned int i = 0; i < data_size; i++)
	{
			points_and_clusters[i] = -1;
	}

	unsigned int e = 1;
	double wcss = -1;
	if (mpirank == 0 && STDOUT) std::cout << "epoch: " << e << ", WCSS: " << wcss << std::endl;

	while(kmeans_clustering(points, &centroids, data_size, cluster_count, dims, &points_and_clusters, &wcss))
	{	
		for (unsigned int i = 0; i < data_size; i++)
		{
			points_and_clusters[i] = -1;
		}
		e++;

		if (mpirank == 0 && (e % 50) == 0 && STDOUT) std::cout << "epoch: " << e << ", WCSS: " << wcss << std::endl;
	}

	if (mpirank == 0 && STDOUT) std::cout << "Total number of epochs: " << e << ", WCSS: " << wcss << std::endl;

	free(centroids);
	return points_and_clusters;
}

int main(int argc, char **argv)
{
	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

	int name_len = 0;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
  	MPI_Get_processor_name(processor_name, &name_len);

  	std::cout << "Rank " << mpirank+1 << "/" << proc_count << " running on " << processor_name << "." << std::endl;

	if (argc != 8 && mpirank == 0)
	{
		std::cerr << "Wrong number of arguments\n";
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	std::ifstream input(argv[1]);
	if (!input.is_open() && mpirank == 0)
	{
		std::cerr << "Error, while openning input file\n";
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	std::ofstream output(argv[2]);
	if (!output.is_open() && mpirank == 0)
	{
		std::cerr << "Error, while openning output file\n";
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	THREADS = std::stoi(argv[3]);
	DATA_SIZE = static_cast<unsigned int>(std::stoi(argv[4]));
	DIMS = static_cast<unsigned int>(std::stoi(argv[5]));
	CLUSTERS = static_cast<unsigned int>(std::stoi(argv[6]));
	
	if (!std::string(argv[6]).compare("false"))
	{
		STDOUT = false;
	}

	if (((DATA_SIZE % static_cast<unsigned int>(proc_count)) != 0) && mpirank == 0 )
	{
		std::cerr << DATA_SIZE << std::endl;
		std::cerr << proc_count << std::endl;
		std::cerr << "Error, DATA_SIZE/PROC_COUT is not a whole number.\n";
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	data_per_proc = DATA_SIZE/static_cast<unsigned int>(proc_count);

	if (mpirank == 0) std::cout << DIMS << " dims, " << DATA_SIZE << " points, " << data_per_proc << " data per process.\n";

	double *points = allocate_array_double(DATA_SIZE*DIMS);

	if (mpirank == 0)
	{
		std::string str_point;
		for (unsigned int i = 0; i < DATA_SIZE; i++)
		{
			std::getline(input, str_point);
			std::vector<std::string> parsed;
			
			std::stringstream ss(str_point);
			while(ss.good())
			{
				std::string substr;
				getline(ss, substr, ',');

				parsed.push_back(substr);
			}

			for (unsigned int j = 0; j < DIMS; j++)
			{
				points[i*DIMS + j] = std::stod(parsed[j]);
			}
		}
	}

	
	double *local_points = allocate_array_double(data_per_proc*DIMS);

	if (!(MPI_Scatter(points, static_cast<int>(data_per_proc*DIMS), MPI_DOUBLE, local_points, static_cast<int>(data_per_proc*DIMS), MPI_DOUBLE, 0, MPI_COMM_WORLD) == MPI_SUCCESS))
	{
		std::cerr << "Error in scatter.\n";
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	double time_counter = 0.0;
	time_counter -= MPI_Wtime();
	int *local_points_and_clusters = do_kmeans(&local_points, data_per_proc, CLUSTERS, DIMS);
	time_counter += MPI_Wtime();

	if (mpirank == 0) std::cout << "K-Means took " << time_counter << " seconds to run." << std::endl;

	int *points_and_clusters = NULL;
	if (mpirank == 0)
	{
		points_and_clusters = (int *)malloc(DATA_SIZE * sizeof(int));
	}

	MPI_Gather(local_points_and_clusters, (int)data_per_proc, MPI_INT, points_and_clusters, (int)data_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	if (mpirank == 0)
	{
		for (unsigned int i = 0; i < DIMS; i++)
		{
			if (!(output << i ))
			{
				std::cerr << "Error while writing to file.\n";			
				MPI_Abort(MPI_COMM_WORLD, 1);
			}

			if (i != (DIMS-1))
			{
				if (!(output << ","))
				{
					std::cerr << "Error while writing to file.\n";			
					MPI_Abort(MPI_COMM_WORLD, 1);
				}
			}
			else if (!(output << ",c" << std::endl))
			{	
				std::cerr << "Error while writing to file.\n";			
				MPI_Abort(MPI_COMM_WORLD, 1);
			}
		}

		for (unsigned int i = 0; i < DATA_SIZE; i++)
		{
			for (unsigned int j = 0; j < DIMS; j++)
			{
				if (!(output << points[i*DIMS + j]))
				{
					std::cerr << "Error while writing to file.\n";			
					MPI_Abort(MPI_COMM_WORLD, 1);
				}

				if (j != (DIMS-1))
				{
					if (!(output << ","))
					{
						std::cerr << "Error while writing to file.\n";			
						MPI_Abort(MPI_COMM_WORLD, 1);
					}
				}
				else if (!(output << ',' << points_and_clusters[i] << std::endl))
				{	
					std::cerr << "Error while writing to file.\n";			
					MPI_Abort(MPI_COMM_WORLD, 1);
				}
			}
		}
	}

	free(points_and_clusters);
	free(local_points_and_clusters);
	free(points);
	free(local_points);
	MPI_Finalize();

	return 0;
}