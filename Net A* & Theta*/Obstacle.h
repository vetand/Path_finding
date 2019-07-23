#ifndef Obstacle_file
#define Obstacle_file
#include"Geometry.h"
#include<fstream>
#include<tuple>
#include<vector>

namespace spa2 {

class Obstacle {
private:
    int vertex_number;
    std::vector<Point> vertex;
    std::pair<double, double> x_bounds;
    std::pair<double, double> y_bounds;

public:

    Obstacle(const std::vector<Point>& points) : vertex_number(points.size()),
                                                 vertex(points) {
        x_bounds = {vertex[0].x, vertex[0].x};
        y_bounds = {vertex[0].y, vertex[0].y};
        for (int i = 1; i < points.size(); ++i) {
            x_bounds.first = std::min(x_bounds.first, vertex[i].x);
            x_bounds.second = std::max(x_bounds.second, vertex[i].x);
            y_bounds.first = std::min(y_bounds.first, vertex[i].y);
            y_bounds.second = std::max(y_bounds.second, vertex[i].y);
        }
    }

    Obstacle(const std::vector<std::pair<int, int>>& points) : vertex_number(points.size()) {
        for (int i = 0; i < vertex_number; ++i) {
            vertex.push_back(Point(points[i]));
        }
        *this = Obstacle(vertex);
    }

    int get_xmin() const {
        return x_bounds.first;
    }

    int get_xmax() const {
        return x_bounds.second;
    }

    int get_ymin() const {
        return y_bounds.first;
    }

    int get_ymax() const {
        return y_bounds.second;
    }

    bool do_cross(const Vector& v) const {
        for (int i = 1; i < vertex.size() + 1; ++i) {
            if (Vector(vertex[i - 1], vertex[i % vertex_number]) & v) {
                return true;
            }
        }
        Point near_end = v.near_B();
        return ((Vector(vertex[vertex_number - 1], vertex[0]) & v) ||
                (is_point_in(near_end)) ||
                (is_point_in(v.get_A())) ||
                (is_point_in(v.get_B())));    // in some cases it is important,
                                              // for example, when the segment is located
                                              // fully in the rectangle
    }

    bool is_point_in(const Point& p) const {
        Vector check(p, Point((double)MAX_WIDTH, p.y + 1));
        int number_of_cross = 0;
        for (int i = 1; i < vertex.size() + 1; ++i) {
            Vector cur(vertex[i - 1], vertex[i % (int)vertex.size()]);
            if (cur.on_Vector(p)) {
                return false; // this point is on the border - it means it is out of the area
            }
            if (cur & check) {
                ++number_of_cross;
            }
        }
        return (number_of_cross % 2 == 1);
    }

    bool operator < (const Obstacle& other) const {
        return std::tie(x_bounds, y_bounds) <
               std::tie(other.x_bounds, other.y_bounds);
    }

    void print_to_visual(std::ostream& out) const {
        out << "    <polygon points=" << '"';
        for (const Point& current : vertex) {
            out << current.x << "," << current.y << " ";
        }
        out << '"' << "/>" << std::endl;
    }

};

Obstacle read_obs(std::istream& in) {
    int number;
    in >> number;
    std::vector<Point> points;
    for (int i = 0; i < number; ++i) {
        int x, y;
        in >> x >> y;
        points.push_back(Point(x, y));
    }
    return Obstacle(points);
}

};

#endif
