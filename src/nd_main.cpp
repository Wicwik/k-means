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

bool kmeans_clustering(std::vector<Point>* points, std::vector<Point>* centroids, int k)
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
	std::vector<double> sum_x;
	std::vector<double> sum_y;

	for (int i = 0; i < k; i++)
	{
		number_of_points.push_back(0);
		sum_x.push_back(0.0);
		sum_y.push_back(0.0);
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
		sum_x[cluster] += p.get_x();
		sum_y[cluster] += p.get_y();

		p.set_minimal_distance(__DBL_MAX__);
	}

	std::vector<Point> old_centroids;
	for (auto &c : (*centroids))
	{
		unsigned int cluster_id = (&c - &(*centroids)[0]);

		old_centroids.push_back(c);
		c.set_x(sum_x[cluster_id]/number_of_points[cluster_id]);
		c.set_y(sum_y[cluster_id]/number_of_points[cluster_id]);
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

std::vector<Point> do_kmeans(std::vector<Point>* points, int epochs, int k)
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
		while(kmeans_clustering(points, &centroids, k))
		{
			std::cout << "epoch: " << ++e << std::endl;
		}
	}
	else
	{
		for (int i = 0; i < epochs; i++)
		{
			if(!kmeans_clustering(points, &centroids, k))
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

		double x = std::stod(parsed[0]);
		double y = std::stod(parsed[1]);

		Point p{x, y};
		points.push_back(p);
	}

	if (!input.eof()) //unexpected end of stream
	{
		std::cout << "Error in input stream\n";
		return 1;
	}

	std::vector<Point> centroids = do_kmeans(&points, EPOCHS, number_of_clusters);

	if (!(output << "x,y,c" << std::endl))
	{
		std::cerr << "Error while writing to file.\n";			
		return 1;
	}

	for (auto &p : points)
	{
		if (!(output << p.get_x() << "," << p.get_y() << "," << p.get_cluster() << std::endl))
		{
			std::cerr << "Error while writing to file.\n";
			return 1;
		}
	}


	return 0;
}