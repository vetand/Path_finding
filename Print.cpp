#ifndef Print_description
#define Print_description
#include<ctime>
#include<fstream>
#include"Collection.h"

namespace spa {

void Map::print_field(const std::string& name) const {
    std::ofstream out(name);
    for (int y = 0; y < get_height(); ++y) {
        for (int x = 0; x < get_width(); ++x) {
            if (is_free(x, y)) {
                out << ".";
            } else {
                out << "#";
            }
        }
        out << "\n";
    }
    out << std::endl;
}

void Map::print_with_path(const Path& path, std::ostream& out) const {
    out << "total path length = " << path.get_len() << "\n";
    out << "total time used = " << (double)clock() / 1000 << " seconds\n";
    for (int y = 0; y < get_height(); ++y) {
        for (int x = 0; x < get_width(); ++x) {
            if (is_free(x, y)) {
                if (path.in_path(x, y)) {
                    out << "*";
                } else {
                    out << ".";
                }
            } else {
                out << "#";
            }
        }
        out << "\n";
    }
    out << std::endl;
}

};

#endif
