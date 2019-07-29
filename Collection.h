#ifndef COLLECTION
#define COLLECTION
#include<algorithm>
#include<ctime>
#include<fstream>
#include<iostream>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include<string>
#include<vector>

namespace spa {

    class Map {
    private:
        int height;
        int width;
        std::unordered_set<int> obstacles; // unfortunetally, we can`t use pair<int, int> in unordered_set

    public:

        std::pair<int, int> start_vertex;
        std::pair<int, int> finish_vertex;

        Map();

        Map(const Map& other);

        int get_height() const;

        int get_width() const;

        bool is_free(int x, int y) const;

        int available() const;

        void get_field(const std::string& name = "input.txt");

    };

    class Path {
    private:
        double length;
        std::unordered_set<int> elements;
        const spa::Map& graph;

    public:

        Path(const Map& input_map);

        double dist(int vertex_1, int vertex_2);

        bool in_path(int x, int y) const;

        void push(int vertex);

        double& len();

        double get_len() const;

    };

    void print_field(const Map& graph, const std::string& name);

    void print_with_path(const Map& graph, const Path& path, std::ostream& out);

    class A_star {
    private:

        double h(int vertex);

        class SearchNode {
        public:

            A_star* owner;
            int id;
            double g_value;
            double f_value;
            SearchNode* parent;

            SearchNode(A_star* input_owner, int x, int y, double dist);

            bool operator < (const SearchNode& other) const;

            bool operator == (const SearchNode& other) const;

        };

        struct compare {
            bool operator() (const SearchNode* first, const SearchNode* second) const;
        };

        int start; // (x, y) -> y * width + x - to lower overall memory size
        int finish;
        std::priority_queue<SearchNode*, std::vector<SearchNode*>, compare> OPEN;
        std::unordered_set<int> CLOSED;
        std::unordered_map<int, SearchNode*> USED; // vertexes from OPEN and CLOSED
        const spa::Map& graph;

    public:

        A_star(const Map& input_field);

        Path build_path();

        int marked() const;

    };

    void solve_with_A_star(const Map& graph, const std::string& name = "output.txt");

};

#endif // COLLECTION
