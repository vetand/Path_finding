#ifndef Field_description
#define Field_description
#include<iostream>
#include<unordered_set>
#include<string>
#include<vector>

namespace spa {

class Field {
private:
    int height;
    int width;
    std::unordered_set<int> obstacles; // unfortunetally, we can`t use pair<int, int> in unordered_set

public:

    Field(const std::vector<std::string>& input_field) :    height(input_field.size()),
                                                            width(input_field[0].size()) {
        for (int i = 0; i < (int)input_field.size(); ++i) {
            for (int j = 0; j < (int)input_field[i].size(); ++j) {
                if (input_field[i][j] == '#') {
                    obstacles.insert(i * width + j);
                }
            }
        }
    }

    Field(const Field& other) : height(other.height), width(other.width), obstacles(other.obstacles) {}

    int get_height() const {
        return height;
    }

    int get_width() const {
        return width;
    }

    bool is_free(int x, int y) const {
        return (obstacles.find(y * width + x) == obstacles.end());
    }

    int available() const {
        return (height * width - obstacles.size());
    }

};

Field get_field(const std::string& name = "input.txt") {
    std::ifstream in(name);
    int height, width;
    in >> width >> height;
    std::vector<std::string> input_field;
    for (int i = 0; i < height; ++i) {
        std::string new_str;
        in >> new_str;
        input_field.push_back(new_str);
    }
    in.close();
    return Field(input_field);
}

};

#endif
