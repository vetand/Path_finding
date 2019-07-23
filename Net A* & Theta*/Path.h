#ifndef Path_file
#define Path_file
#include"Field.h"
#include<unordered_map>

namespace spa2 {

class Path {
private:
    double path_length;
    std::vector<Point> elements;

public:

    //now we also consider that hash = y * width + x
    Path(const std::unordered_map<int, int>& prev, int start, int finish, int width) {
        path_length = 0;
        while (start != finish) {
            elements.push_back(Point(finish % width, finish / width));
            if (elements.size() >= 2) {
                path_length += Vector(elements[elements.size() - 1], elements[elements.size() - 2]).length();
            }
            finish = prev.find(finish)->second;
        }
        elements.push_back(Point(start % width, start / width));
        if (elements.size() >= 2) {
            path_length += Vector(elements[elements.size() - 1], elements[elements.size() - 2]).length();
        }
        //reverse
        for (int i = 0; i < elements.size() / 2; ++i) {
            std::swap(elements[i], elements[elements.size() - 1 - i]);
        }
    }

    double length() const {
        return path_length;
    }

    void print(std::ostream& out) const {
        out << "Path length = " << path_length << std::endl;
        for (int i = 0; i < elements.size(); ++i) {
            out << elements[i].x << ' ' << elements[i].y << std::endl;
        }
    }

    void print_to_visual(std:: ostream& out, const std::string& color_name) const {
        out << "    <polyline points=" << '"';
        for (const Point& current : elements) {
            out << current.x  << "," << current.y << " ";
        }
        out << '"' << " fill=" << '"' << "none" << '"' << " stroke=" << '"' << color_name << '"'
                << " stroke-width=" << '"' << "0.05" << '"' << "/>\n";
    }

};

};

#endif
