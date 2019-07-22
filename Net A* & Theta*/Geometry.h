#ifndef Geometry_file
#define Geometry_file
#include<algorithm>
#include<iostream>

namespace spa2 {

const double eps = 1e-8;
const int MAX_WIDTH = 10000;

class Point {
public:

    double x;
    double y;

    Point(std::pair<int, int> input) : x(input.first), y(input.second) {}

    Point(std::pair<double, double> input) : x(input.first), y(input.second) {}

    Point(int input_x, int input_y) : x(input_x), y(input_y) {}

    Point(double input_x, double input_y) : x(input_x), y(input_y) {}

};

class Vector {
private:
    Point A;
    Point B;

public:

    Vector(Point p1, Point p2) : A(p1), B(p2) {}

    double length() const {
        double delta_x = B.x - A.x;
        double delta_y = B.y - A.y;
        return sqrt(delta_x * delta_x + delta_y * delta_y);
    }

    double operator * (const Vector& other) const {
        Vector Real_this = Vector(Point(0, 0), Point(B.x - A.x, B.y - A.y));
        Vector Real_other = Vector(Point(0, 0), Point(other.B.x - other.A.x, other.B.y - other.A.y));
        return Real_this.B.x * Real_other.B.y - Real_this.B.y * Real_other.B.x;
    }

    double operator ^ (const Vector& other) const {
        Vector Real_this = Vector(Point(0, 0), Point(B.x - A.x, B.y - A.y));
        Vector Real_other = Vector(Point(0, 0), Point(other.B.x - other.A.x, other.B.y - other.A.y));
        return Real_this.B.x * Real_other.B.x + Real_this.B.y * Real_other.B.y;
    }

    bool on_Vector(const Point& p) const {
        Vector to_point(A, p);
        return ((std::abs(to_point * (*this)) < eps) && ((to_point ^ (*this)) > eps) &&
                                                (to_point.length() - eps < (*this).length()));
    }

    Point near_B() const { // a point near current end of vector
        double delta_x = (B.x - A.x) * (1 - (double)1 / MAX_WIDTH);
        double delta_y = (B.y - A.y) * (1 - (double)1 / MAX_WIDTH);
        return Point(A.x + delta_x, A.y + delta_y);
    }

    Point get_A() const {
        return A;
    }

    Point get_B() const {
        return B;
    }

    void print() const {
        std::cerr << A.x << ' ' << A.y << ' ' << B.x << ' ' << B.y << std::endl;
    }

};

bool operator & (const Vector& v1, const Vector& v2) { //do they cross?
    //first step
    Vector first_side = Vector(v1.get_A(), v2.get_A());
    Vector second_side = Vector(v1.get_A(), v2.get_B());
    double multiply_1 = v1 * first_side;
    double multiply_2 = v1 * second_side;
    double result_1 = multiply_1 * multiply_2;
    //second step
    first_side = Vector(v2.get_A(), v1.get_A());
    second_side = Vector(v2.get_A(), v1.get_B());
    multiply_1 = v2 * first_side;
    multiply_2 = v2 * second_side;
    double result_2 = (multiply_1 * multiply_2);
    return ((result_1 < -eps) && (result_2 < -eps));
}

};

#endif
