#include"Collection.h"

namespace spa2 {

    Point::Point() {}

    Point::Point(std::pair<int, int> input) : x(input.first), y(input.second) {}

    Point::Point(std::pair<double, double> input) : x(input.first), y(input.second) {}

    Point::Point(int input_x, int input_y) : x(input_x), y(input_y) {}

    Point::Point(double input_x, double input_y) : x(input_x), y(input_y) {}

    Vector::Vector(Point p1, Point p2) : A(p1), B(p2) {}

    double Vector::length() const {
        double delta_x = B.x - A.x;
        double delta_y = B.y - A.y;
        return sqrt(delta_x * delta_x + delta_y * delta_y);
    }

    double Vector::operator * (const Vector& other) const {
        Vector Real_this = Vector(Point(0, 0), Point(B.x - A.x, B.y - A.y));
        Vector Real_other = Vector(Point(0, 0), Point(other.B.x - other.A.x, other.B.y - other.A.y));
        return Real_this.B.x * Real_other.B.y - Real_this.B.y * Real_other.B.x;
    }

    double Vector::operator ^ (const Vector& other) const {
        Vector Real_this = Vector(Point(0, 0), Point(B.x - A.x, B.y - A.y));
        Vector Real_other = Vector(Point(0, 0), Point(other.B.x - other.A.x, other.B.y - other.A.y));
        return Real_this.B.x * Real_other.B.x + Real_this.B.y * Real_other.B.y;
    }

    bool Vector::on_Vector(const Point& p) const {
        Vector to_point(A, p);
        return ((std::abs(to_point * (*this)) < eps) && ((to_point ^ (*this)) > eps) &&
                                                (to_point.length() - eps < (*this).length()));
    }

    Point Vector::near_B() const { // a point near current end of vector
        double delta_x = (B.x - A.x) * (1 - (double)1 / MAX_WIDTH);
        double delta_y = (B.y - A.y) * (1 - (double)1 / MAX_WIDTH);
        return Point(A.x + delta_x, A.y + delta_y);
    }

    Point Vector::get_A() const {
        return A;
    }

    Point Vector::get_B() const {
        return B;
    }

    void Vector::print() const {
        std::cerr << A.x << ' ' << A.y << ' ' << B.x << ' ' << B.y << std::endl;
    }

    bool Vector::operator & (const Vector& other) { //do they cross?
        //first step
        Vector first_side = Vector(A, other.get_A());
        Vector second_side = Vector(A, other.get_B());
        double multiply_1 = (*this) * first_side;
        double multiply_2 = (*this) * second_side;
        double result_1 = multiply_1 * multiply_2;
        //second step
        first_side = Vector(other.get_A(), A);
        second_side = Vector(other.get_A(), B);
        multiply_1 = other * first_side;
        multiply_2 = other * second_side;
        double result_2 = (multiply_1 * multiply_2);
        return ((result_1 < -eps) && (result_2 < -eps));
    }

};
