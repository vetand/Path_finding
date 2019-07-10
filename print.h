#ifndef Print_description
#define Print_description
#include<fstream>
#include"field.h"
#include"path.h"

namespace spa {

void print_field(const Field& graph, const std::string& name = "output.txt") {
    std::ofstream out(name);
    for (int y = 0; y < graph.get_height(); ++y) {
        for (int x = 0; x < graph.get_width(); ++x) {
            if (graph.is_free(x, y)) {
                out << ".";
            } else {
                out << "#";
            }
        }
        out << "\n";
    }
    out << std::endl;
}

void print_with_path(const Field& graph, const Path& path, std::ostream& out) {
    out << "total path length = " << path.get_len() << "\n";
    out << "total time used = " << (double)clock() / 1000 << " seconds\n";
    for (int y = 0; y < graph.get_height(); ++y) {
        for (int x = 0; x < graph.get_width(); ++x) {
            if (graph.is_free(x, y)) {
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
