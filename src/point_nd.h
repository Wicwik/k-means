#pragma once

class Point
{

public:
	Point()
		: m_coordinates{0}
		, m_cluster{-1}
		, m_minimal_distance{__DBL_MAX__}
	{
	}

	Point(std::vector<double> coordinates)
		: m_coordinates{coordinates}
		, m_cluster{-1}
		, m_minimal_distance{__DBL_MAX__}
	{
	}

	Point(const Point& other)
		: m_coordinates{other.m_coordinates}
		, m_cluster{other.m_cluster}
		, m_minimal_distance{other.m_minimal_distance}
	{
	}

	Point& operator=(const Point& rhs)
    {
        Point tmp(rhs);
        m_swap(tmp);
        return *this;
    }

    double distance(Point other)
    {
    	double dist = 0;

    	if (m_coordinates.size() == other.m_coordinates.size())
		{
			for (const auto &c : other.m_coordinates)
			{
				unsigned int i = (&c - &other.m_coordinates[0]);
				dist += ((c - m_coordinates[i]) * (c - m_coordinates[i]));
			}
		}
		else
		{
			throw "Uneven number of dimensions\n";
		}

    	return dist;
    }

    double get_minimal_distance()
    {
    	return m_minimal_distance;
    }

    void set_minimal_distance(double minimal_distance)
    {
    	m_minimal_distance = minimal_distance;
    }

    int get_cluster()
    {
    	return m_cluster;
    }

    void set_cluster(int cluster)
    {
    	m_cluster = cluster;
    }

    std::vector<double> get_coordinates()
    {
    	return m_coordinates;
    }

    void set_coordinates(std::vector<double> coordinates)
    {
    	m_coordinates = coordinates;
    }

private:
	std::vector<double> m_coordinates;
	int m_cluster;
	double m_minimal_distance;

	void m_swap(Point tmp)
    {
        m_coordinates = tmp.m_coordinates;
        m_cluster = tmp.m_cluster;
        m_minimal_distance = tmp.m_minimal_distance;
    }

	friend std::ostream& operator<<(std::ostream& lhs, const Point& rhs);
	friend Point operator+(Point lhs, const Point& rhs);
	friend Point operator+(Point lhs, const double& rhs);
	friend Point operator-(Point lhs, const Point& rhs);
	friend Point operator-(Point lhs, const double& rhs);
	friend Point operator*(Point lhs, const Point& rhs);
	friend Point operator*(Point lhs, const double& rhs);
	friend bool operator==(const Point& lhs, const Point& rhs);
	friend bool operator!=(const Point& lhs, const Point& rhs);
};

std::ostream& operator<<(std::ostream& lhs, const Point& rhs)
{
	lhs << "[";
	for (const auto &c : rhs.m_coordinates)
	{
		if (&c == &rhs.m_coordinates.back())
		{
			lhs << c << "]";
		}
		else
		{
			lhs << c << ",";
		}
	}

	return lhs;
}

Point operator+(Point lhs, const Point& rhs)
{
	std::vector<double> sum;

	if (lhs.m_coordinates.size() == rhs.m_coordinates.size())
	{
		for (const auto &c : lhs.m_coordinates)
		{
			unsigned int i = (&c - &lhs.m_coordinates[0]);
			sum.push_back(c + rhs.m_coordinates[i]);
		}
	}
	else
	{
		throw "Uneven number of dimensions\n";
	}

	return Point{sum};
}

Point operator+(Point lhs, const double& rhs)
{
	std::vector<double> sum;

	for (const auto &c : lhs.m_coordinates)
		{
			sum.push_back(c + rhs);
		}

	return Point{sum};
}

Point operator-(Point lhs, const Point& rhs)
{
	std::vector<double> diff;

	if (lhs.m_coordinates.size() == rhs.m_coordinates.size())
	{
		for (const auto &c : lhs.m_coordinates)
		{
			unsigned int i = (&c - &lhs.m_coordinates[0]);
			diff.push_back(c - rhs.m_coordinates[i]);
		}
	}
	else
	{
		throw "Uneven number of dimensions\n";
	}

	return Point{diff};
}

Point operator-(Point lhs, const double& rhs)
{
	std::vector<double> diff;

	for (const auto &c : lhs.m_coordinates)
	{
		diff.push_back(c - rhs);
	}

	return Point{diff};
}

Point operator*(Point lhs, const Point& rhs)
{
	std::vector<double> mult;

	if (lhs.m_coordinates.size() == rhs.m_coordinates.size())
	{
		for (const auto &c : lhs.m_coordinates)
		{
			unsigned int i = (&c - &lhs.m_coordinates[0]);
			mult.push_back(c * rhs.m_coordinates[i]);
		}
	}
	else
	{
		throw "Uneven number of dimensions\n";
	}

	return Point{mult};
}

Point operator*(Point lhs, const double& rhs)
{
	std::vector<double> mult;

	for (const auto &c : lhs.m_coordinates)
	{
		mult.push_back(c * rhs);
	}

	return Point{mult};
}

bool operator==(const Point& lhs, const Point& rhs)
{
	if (lhs.m_coordinates.size() == rhs.m_coordinates.size())
	{
		for (const auto &c : lhs.m_coordinates)
		{
			unsigned int i = (&c - &lhs.m_coordinates[0]);
			if (c  != rhs.m_coordinates[i])
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		throw "Uneven number of dimensions\n";
	}
}

bool operator!=(const Point& lhs, const Point& rhs)
{
	if (lhs == rhs)
	{
		return false;
	}

	return true;
}