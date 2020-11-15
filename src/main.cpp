#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <ctime>
#include "point.h"

bool is_number(std::string x)
{
    std::regex e("^[0-9]+$");

    return std::regex_match (x,e); 
}

void kmeans_clustering(std::vector<Point>* points, int epochs, int k)
{
	unsigned int n = points->size();

	std::vector<Point> centroids;
	srand(std::time(0)); 
	for (int i = 0; i < k; i++) 
	{
    	centroids.push_back(points->at(static_cast<unsigned int>(rand()) % n));
	}

	for (auto &c : centroids)
	{
		int cluster_id = (&c - &centroids[0]);

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

	for (auto &p : (*points))
	{
		std::cout << p.get_minimal_distance() << std::endl;
	}
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

	kmeans_clustering(&points, 30, number_of_clusters);

	// std::cout << points[0].distance(points[1]) << std::endl;


	return 0;
}