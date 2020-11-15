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
	std::vector<Point> centroids;
	srand(std::time(0));  // need to set the random seed

	for (int i = 0; i < k; i++) 
	{
    	centroids.push_back(points->at(rand() % 100));
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

	std::cout << points[0].distance(points[1]) << std::endl;


	return 0;
}