#ifndef Path_description
#define Path_description
#include"Collection.h"

namespace spa {

    Path::Path(const Map& input_map) : graph(input_map) {}

    double Path::dist(int vertex_1, int vertex_2) {
        std::pair<int, int> point_1 = std::make_pair(vertex_1 % graph.get_width(), vertex_1 / graph.get_width());
        std::pair<int, int> point_2 = std::make_pair(vertex_2 % graph.get_width(), vertex_2 / graph.get_width());
        int delta_x = point_2.first - point_1.first;
        int delta_y = point_2.second - point_1.second;
        return sqrt(delta_x * delta_x + delta_y * delta_y);
    }

    bool Path::in_path(int x, int y) const {
        return (elements.find(y * graph.get_width() + x) != elements.end());
    }

    void Path::push(int vertex) {
        elements.insert(vertex);
    }

    double& Path::len() {
        return length;
    }

    double Path::get_len() const {
        return length;
    }

};

#endif
