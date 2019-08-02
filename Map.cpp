#include"Collection.h"

namespace spa2 {

    Map::Map() {}

    int Map::get_height() const {
        return height;
    }

    int Map::get_width() const {
        return width;
    }

    bool Map::correct_travel(const Vector& v) const {
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

    void Map::print_to_visual(std::ostream& out) const {
        out << "<svg width=" << '"' << 700 << '"' << " height=" << '"' << 700 <<
                '"' << " viewBox=" << '"' << "0 0 " << width << " " << height << '"' << ">\n";
        for (const Obstacle& polygon : let) {
            polygon.print_to_visual(out);
        }
    }

    void Map::get_field(const std::string name) {
        std::ifstream in(name);
        in >> height >> width;
        int number_of_obstacles;
        in >> number_of_obstacles;
        for (int i = 0; i < number_of_obstacles; ++i) {
            Obstacle obj = read_obs(in);
            let.insert(obj);
        }
        in >> start_vertex.x >> start_vertex.y;
        in >> finish_vertex.x >> finish_vertex.y;
        ++width;
        in.close();
        // field is a rectangle from (0, 0) to (height, width)
    }

};
