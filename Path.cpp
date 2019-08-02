#include"Collection.h"

namespace spa2 {

    //now we also consider that hash = y * width + x
    Path::Path() {}

    double& Path::length() {
        return path_length;
    }

    void Path::push(const Point& A) {
        elements.push_back(A);
    }

    void Path::flip() { // reverse
        for (int i = 0; i < (int)elements.size() / 2; ++i) {
            std::swap(elements[i], elements[elements.size() - i - 1]);
        }
    }

    double Path::get_len() const {
        return path_length;
    }

    std::vector<Point>& Path::nodes() {
        return elements;
    }

    void Path::print(std::ostream& out) const {
        out << "Path length = " << path_length << std::endl;
        for (int i = 0; i < elements.size(); ++i) {
            out << elements[i].x << ' ' << elements[i].y << std::endl;
        }
    }

    void Path::print_to_visual(std:: ostream& out, const std::string& color_name) const {
        out << "    <polyline points=" << '"';
        for (const Point& current : elements) {
            out << current.x  << "," << current.y << " ";
        }
        out << '"' << " fill=" << '"' << "none" << '"' << " stroke=" << '"' << color_name << '"'
                << " stroke-width=" << '"' << "0.05" << '"' << "/>\n";
    }

};
