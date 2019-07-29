#ifndef Collection
#define Collection

namespace spa {

    class Map;

    class Path;

    void print_field(const Map& graph, const std::string& name = "output.txt");

    void print_with_path(const Map& graph, const Path& path, std::ostream& out);

    class A_star;

    void solve_with_A_star(const Map& graph, const std::string& name = "output.txt");

};

#endif
