#ifndef Path_description
#define Path_description
#include<algorithm>
#include<iostream>
#include<unordered_map>
#include<vector>
#include"field.h"

namespace spa {

class Path {
private:
    double length;
    std::unordered_set<int> elements;
    const spa::Field& graph;

    double dist(int vertex_1, int vertex_2) {
        std::pair<int, int> point_1 = std::make_pair(vertex_1 % graph.get_width(), vertex_1 / graph.get_width());
        std::pair<int, int> point_2 = std::make_pair(vertex_2 % graph.get_width(), vertex_2 / graph.get_width());
        int delta_x = point_2.first - point_1.first;
        int delta_y = point_2.second - point_1.second;
        return sqrt(delta_x * delta_x + delta_y * delta_y);
    }

public:

    Path(const Field& input_field, const std::unordered_map<int, int>& previous, int start, int finish) : graph(input_field) {
        length = 0;
        while (finish != start) {
            length += dist(finish, previous.find(finish)->second);
            elements.insert(finish);
            finish = previous.find(finish)->second;
        }
        elements.insert(start);
    }

    bool in_path(int x, int y) const {
        return (elements.find(y * graph.get_width() + x) != elements.end());
    }

    double get_len() const {
        return length;
    }

};

};

#endif
