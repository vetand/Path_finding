#ifndef Field_description
#define Field_description
#include"Collection.h"

namespace spa {

    Map::Map() {}

    Map::Map(const Map& other) : height(other.height), width(other.width), obstacles(other.obstacles) {}

    int Map::get_height() const {
        return height;
    }

    int Map::get_width() const {
        return width;
    }

    bool Map::is_free(int x, int y) const {
        return (obstacles.find(y * width + x) == obstacles.end());
    }

    int Map::available() const {
        return (height * width - (int)obstacles.size());
    }

    void Map::get_field(const std::string& name) {
        std::ifstream in(name);
        in >> height >> width;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                char grid;
                in >> grid;
                if (grid == '#') {
                    obstacles.insert(i * width + j);
                }
            }
        }
        in >> start_vertex.first >> start_vertex.second;
        in >> finish_vertex.first >> finish_vertex.second;
        in.close();
    }

};

#endif
