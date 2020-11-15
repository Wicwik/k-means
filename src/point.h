#pragma once

class Point
{

public:
	Point()
		: m_x{0}
		, m_y{0}
		, m_cluster{-1}
		, m_minimal_distance{__DBL_MAX__}
	{
	}

	Point(double x, double y)
		: m_x{x}
		, m_y{y}
		, m_cluster{-1}
		, m_minimal_distance{__DBL_MAX__}
	{
	}

	Point(const Point& other)
		: m_x{other.m_x}
		, m_y{other.m_y}
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

    double distance(Point p)
    {
    	return (p.m_x - m_x) * (p.m_x - m_x) + (p.m_y - m_y) * (p.m_y - m_y);
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

    double get_x()
    {
    	return m_x;
    }

    void set_x(double x)
    {
    	m_x = x;
    }

    double get_y()
    {
    	return m_y;
    }

    void set_y(double y)
    {
    	m_y = y;
    }

private:
	double m_x;
	double m_y;
	int m_cluster;
	double m_minimal_distance;

	void m_swap(Point tmp)
    {
        m_x = tmp.m_x;
        m_y = tmp.m_y;
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
	return lhs << "[" << rhs.m_x << "," << rhs.m_y << "]";
}

Point operator+(Point lhs, const Point& rhs)
{
	return Point{(lhs.m_x + rhs.m_x), (lhs.m_y + rhs.m_y)};
}

Point operator+(Point lhs, const double& rhs)
{
	return Point{(lhs.m_x + rhs), (lhs.m_y + rhs)};
}

Point operator-(Point lhs, const Point& rhs)
{
	return Point{(lhs.m_x - rhs.m_x), (lhs.m_y - rhs.m_y)};
}

Point operator-(Point lhs, const double& rhs)
{
	return Point{(lhs.m_x - rhs), (lhs.m_y - rhs)};
}

Point operator*(Point lhs, const Point& rhs)
{
	return Point{(lhs.m_x * rhs.m_x), (lhs.m_y * rhs.m_y)};
}

Point operator*(Point lhs, const double& rhs)
{
	return Point{(lhs.m_x * rhs), (lhs.m_y * rhs)};
}

bool operator==(const Point& lhs, const Point& rhs)
{
	if (lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y)
	{
		return true;
	}
	else
	{
		return false;
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