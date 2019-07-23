#ifndef Field_file
#define Field_file
#include"Obstacle.h"
#include<set>

namespace spa2 {

class Field {
private:

    int height;
    int width;
    std::set<Obstacle> let;

public:

    Field(int input_height, int input_width, const std::set<Obstacle>& input_set) :
                        height(input_height),
                        width(input_width),
                        let(input_set) {}

    int get_height() const {
        return height;
    }

    int get_width() const {
        return width;
    }

    bool correct_travel(const Vector& v) const {
        for (const Obstacle& cur : let) {
            //fast pre-check
            //now we build a rectangle of borders
            std::vector<Point> point_list;
            point_list.push_back(Point(cur.get_xmin(), cur.get_ymin()));
            point_list.push_back(Point(cur.get_xmin(), cur.get_ymax()));
            point_list.push_back(Point(cur.get_xmax(), cur.get_ymax()));
            point_list.push_back(Point(cur.get_xmax(), cur.get_ymin()));
            Obstacle pre_check(point_list);
            if (pre_check.do_cross(v) || pre_check.is_point_in(v.get_A())
                                      || pre_check.is_point_in(v.get_B())) {
                //full check
                if (cur.do_cross(v)) {
                    return false;
                }
            }
        }
        return true;
    }

    void print_to_visual(std::ostream& out) const {
        out << "<svg width=" << '"' << 700 << '"' << " height=" << '"' << 700 <<
                '"' << " viewBox=" << '"' << "0 0 " << width << " " << height << '"' << ">\n";
        for (const Obstacle& polygon : let) {
            polygon.print_to_visual(out);
        }
    }

};

Field get_field(const std::string name = "input_field.txt") {
    std::ifstream in(name);
    int height, width;
    in >> height >> width;
    int number_of_obstacles;
    in >> number_of_obstacles;
    std::set<Obstacle> input;
    for (int i = 0; i < number_of_obstacles; ++i) {
        Obstacle obj = read_obs(in);
        input.insert(obj);
    }
    in.close();
    return Field(height + 1, width + 1, input); // rectangle from (0, 0) to (height, width)
}

};

#endif
