#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <ctime>
#include "point_nd.h"

const int EPOCHS = -1;

bool is_number(std::string x)
{
    std::regex e("^[0-9]+$");

    return std::regex_match (x,e); 
}

bool kmeans_clustering(std::vector<Point>* points, std::vector<Point>* centroids, int k, unsigned int dims)
{
	for (auto &c : (*centroids))
	{
		int cluster_id = (&c - &(*centroids)[0]);

		// std::cout << "Centroid: " << c << std::endl;
		for (auto &p : (*points))
		{
			double d = c.distance(p);
			// std::cout << p << ":" << d << std::endl;

			if (d < p.get_minimal_distance())
			{
				p.set_minimal_distance(d);
				p.set_cluster(cluster_id);
			}
		}
	}

	std::vector<int> number_of_points;
	std::vector<std::vector<double>> sum_cordinates;

	for (int i = 0; i < k; i++)
	{
		number_of_points.push_back(0);

		std::vector<double> sum;
		for (unsigned int j = 0; j < dims; j++)
		{
			sum.push_back(0.0);
		}

		sum_cordinates.push_back(sum);
	}

	for (auto &p : (*points))
	{
		if (p.get_cluster() == -1)
		{
			std::cerr << "error: Point without cluster.\n";
			std::exit(1);
		}

		unsigned int cluster = static_cast<unsigned int>(p.get_cluster());
		number_of_points[cluster]++;

		for (auto &sum : sum_cordinates[cluster])
		{
			unsigned int cord = (&sum - &sum_cordinates[cluster][0]);

			sum += p.get_coordinates()[cord];
		}

		p.set_minimal_distance(__DBL_MAX__);
	}

	std::vector<Point> old_centroids;
	for (auto &c : (*centroids))
	{
		unsigned int cluster_id = (&c - &(*centroids)[0]);
		std::vector<double> new_coordinates;

		old_centroids.push_back(c);
		for (unsigned int i = 0; i < dims; i++)
		{
			new_coordinates.push_back(sum_cordinates[cluster_id][i]/number_of_points[cluster_id]);
		}

		c.set_coordinates(new_coordinates);
	}

	for (auto &c : (*centroids))
	{
		unsigned int cluster_id = (&c - &(*centroids)[0]);

		std::cout << "old centroid: " << old_centroids[cluster_id] << std::endl;
		std::cout << "new centroid: " << c << std::endl;
		if (c != old_centroids[cluster_id])
		{
			return true;
		}
	}

	return false;
}

std::vector<Point> do_kmeans(std::vector<Point>* points, int epochs, int k, unsigned int dims)
{
	unsigned int n = points->size();
	std::vector<Point> centroids;
	srand(std::time(0)); 
	for (int i = 0; i < k; i++) 
	{
    	centroids.push_back(points->at(static_cast<unsigned int>(rand()) % n));
	}

	if (epochs == -1)
	{
		int e = 0;
		while(kmeans_clustering(points, &centroids, k, dims))
		{
			std::cout << "epoch: " << ++e << std::endl;
		}
	}
	else
	{
		for (int i = 0; i < epochs; i++)
		{
			if(!kmeans_clustering(points, &centroids, k, dims))
			{
				break;
			}
		}
	}


	return centroids;
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Wrong number of arguments\n";
		return 1;
	}

	if (!is_number(argv[3]))
	{
		std::cerr << "Third argument should be number\n";
		return 1;
	}

	int number_of_clusters = std::atoi(argv[3]);

	std::ifstream input(argv[1]);
	if (!input.is_open())
	{
		std::cerr << "Error, while openning input file\n";
		return 1;
	}


	std::ofstream output(argv[2]);
	if (!output.is_open())
	{
		std::cerr << "Error, while openning output file\n";
		return 1;
	}

	std::vector<Point> points;
	std::string str_point;
	unsigned int dims = 0;
	while(std::getline(input, str_point))
	{
		std::vector<std::string> parsed;
		
		std::stringstream ss(str_point);
		while(ss.good())
		{
			std::string substr;
			getline(ss, substr, ',');

			parsed.push_back(substr);
		}

		std::vector<double> coordinates;

		for (auto &i : parsed)
		{
			coordinates.push_back(std::stod(i));
		}

		if (!dims)
		{
			dims = coordinates.size();
		}

		if (dims != coordinates.size())
		{
			std::cerr << "Dimensions differ from previous coordinates.\n";
			exit(0);
		}

		Point p{coordinates};
		points.push_back(p);
	}

	if (!input.eof()) //unexpected end of stream
	{
		std::cout << "Error in input stream\n";
		return 1;
	}

	std::vector<Point> centroids = do_kmeans(&points, EPOCHS, number_of_clusters, dims);

	for (unsigned int i = 0; i < dims; i++)
	{
		if (!(output << static_cast<unsigned char>(('a' + i)) ))
		{
			std::cerr << "Error while writing to file.\n";			
			return 1;
		}

		if (i != (dims-1))
		{
			if (!(output << ","))
			{
				std::cerr << "Error while writing to file.\n";			
				return 1;
			}
		}
		else if (!(output << ",z" << std::endl))
		{	
			std::cerr << "Error while writing to file.\n";			
			return 1;
		}
	}

	std::cout << points[0].get_cluster() << std::endl;

	for (auto &p : points)
	{
		std::vector<double> cords = p.get_coordinates();
		for (unsigned int i = 0; i < dims; i++)
		{
			if (!(output << cords[i]))
			{
				std::cerr << "Error while writing to file.\n";			
				return 1;
			}

			if (i != (dims-1))
			{
				if (!(output << ","))
				{
					std::cerr << "Error while writing to file.\n";			
					return 1;
				}
			}
			else if (!(output << ',' << p.get_cluster() << std::endl))
			{	
				std::cerr << "Error while writing to file.\n";			
				return 1;
			}
		}
	}


	return 0;
}