#ifndef COLLECTION
#define COLLECTION

#include<algorithm>
#include<ctime>
#include<fstream>
#include<iostream>
#include<queue>
#include<set>
#include<string>
#include<tuple>
#include<unordered_map>
#include<unordered_set>
#include<vector>

namespace spa2 {

    const double eps = 1e-8;
    const int MAX_WIDTH = 10000;

    class Point {
    public:

        double x;
        double y;

        Point();

        Point(std::pair<int, int> input);

        Point(std::pair<double, double> input);

        Point(int input_x, int input_y);

        Point(double input_x, double input_y);

    };

    class Vector {
    private:
        Point A;
        Point B;

    public:

        Vector(Point p1, Point p2);

        double length() const;

        double operator * (const Vector& other) const;

        double operator ^ (const Vector& other) const;

        bool on_Vector(const Point& p) const;

        Point near_B() const; // a point near current end of vector

        Point get_A() const;

        Point get_B() const;

        void print() const;

        bool operator & (const Vector& other); //do they cross?

    };

    class Obstacle {
    private:
        int vertex_number;
        std::vector<Point> vertex;
        std::pair<double, double> x_bounds;
        std::pair<double, double> y_bounds;

    public:

        Obstacle(const std::vector<Point>& points);

        Obstacle(const std::vector<std::pair<int, int>>& points);

        int get_xmin() const;

        int get_xmax() const;

        int get_ymin() const;

        int get_ymax() const;

        bool do_cross(const Vector& v) const;

        bool is_point_in(const Point& p) const;

        bool operator < (const Obstacle& other) const;

        void print_to_visual(std::ostream& out) const;

    };

    Obstacle read_obs(std::istream& in);

    class Map {
    private:

        int height;
        int width;
        std::set<Obstacle> let;

    public:

        Point start_vertex;
        Point finish_vertex;

        Map();

        int get_height() const;

        int get_width() const;

        bool correct_travel(const Vector& v) const;

        void print_to_visual(std::ostream& out) const;

        void get_field(const std::string name = "input_field.txt");

    };

    class Path {
    private:
        double path_length;
        std::vector<Point> elements;

    public:

        //now we also consider that hash = y * width + x
        Path();

        double& length();

        void push(const Point& A);

        void flip(); // reverse

        double get_len() const;

        std::vector<Point>& nodes();

        void print(std::ostream& out) const;

        void print_to_visual(std:: ostream& out, const std::string& color_name) const;

    };

    class Algo {
    private:

        Point to_point(int vertex) const;

        double heuristic(const Point& A) const;

        double heuristic(int vertex) const;

        class SearchNode {
        public:

            Algo* owner;
            int id;
            double g_value;
            double f_value;
            SearchNode* parent;

            SearchNode(Algo* input_owner, int x, int y, double dist);

            bool operator < (const SearchNode& other) const;

            bool operator == (const SearchNode& other) const;

        };

        struct compare {
            bool operator() (const SearchNode* first, const SearchNode* second) const;
        };

        int start; // (x, y) -> y * width + x - to lower overall memory size
        int finish;
        std::string algo_name;
        std::priority_queue<SearchNode*, std::vector<SearchNode*>, compare> OPEN;
        std::unordered_set<int> CLOSED;
        std::unordered_map<int, SearchNode*> USED; // vertexes from OPEN and CLOSED
        const Map& graph;

    public:

        Algo(const std::string name, const Map& input_field);

        Path build_answer() const;

    };

    void solve_with_Theta(const Map& graph, const std::string name_out = "output.txt");

    void solve_with_A_star(const Map& graph, const std::string name_out = "output.txt");

    void solve_both(const Map& graph, const std::string name_out = "output_visual.txt");


};

#endif
