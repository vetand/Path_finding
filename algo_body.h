#ifndef Algorithm_file
#define Algorithm_file
#include"Path.cpp"
#include<ctime>
#include<queue>
#include<unordered_set>

namespace spa2 {

double default_h(const Point& current, const Point& finish) {
    return Vector(current, finish).length();
}

class Algo {
private:
    double (*h)(const Point&, const Point&);
    std::string algo_name;
    int start; // (x, y) -> y * width + x - to lower overall memory size
    int finish;
    std::unordered_set<int> used;
    std::unordered_map<int, int> previous;
    std::unordered_set<int> ready;
    std::unordered_map<int, double> dist;
    const Field& graph;

    Point to_point(int vertex) const {
        return Point(vertex % graph.get_width(), vertex / graph.get_width());
    }

    class Candidate {
    public:

        Algo* owner;
        double potential;
        int vertex;

        Candidate(Algo* input_owner) : potential(0), vertex(0), owner(input_owner) {}

        Candidate(Algo* input_owner, double input_pot, int input_vert) :
                                            potential(input_pot), vertex(input_vert), owner(input_owner) {}

        bool operator < (const Candidate& other) const {
            if (std::abs(potential - other.potential) < eps) {
                if (owner->h(owner->to_point(vertex), owner->to_point(owner->finish)) - eps <
                    owner->h(owner->to_point(other.vertex), owner->to_point(owner->finish))) {
                    return true;
                } else {
                    return false;
                }
            }
            return (potential > other.potential);
        }

        bool operator == (const Candidate& other) const {
            return !((*this < other) || (other < *this));
        }

    };

    std::priority_queue<Candidate> Q;

public:

    Algo(const std::string name, const Field& input_field,
                        std::pair<int, int> start_vertex,
                        std::pair<int, int> finish_vertex,
                        double input_h(const Point&, const Point&) = default_h) :
                        graph(input_field),
                        algo_name(name),
                        h(input_h) {
        start = start_vertex.first + graph.get_width() * start_vertex.second;
        finish = finish_vertex.first + graph.get_width() * finish_vertex.second;

        Q.push(Candidate(this, (double)0, start));
        dist[start] = 0;
        while (!Q.empty()) {
            Candidate current = Q.top();
            Q.pop();
            if (ready.find(current.vertex) != ready.end()) {
                continue; // we found better way
            }
            if (current.vertex == finish) {
                break;
            }
            ready.insert(current.vertex);
            Point cur = to_point(current.vertex);
            for (int delta_x = -1; delta_x <= 1; ++delta_x) {
                for (int delta_y = -1; delta_y <= 1; ++delta_y) {
                    int next_x = current.vertex % graph.get_width() + delta_x;
                    int next_y = current.vertex / graph.get_width() + delta_y;
                    Point next_vert(next_x, next_y);
                    int neighbor = next_y * graph.get_width() + next_x;
                    Vector travel(cur, next_vert);
                    if (next_x < 0 || next_x >= graph.get_width() ||
                        next_y < 0 || next_y >= graph.get_height() ||
                        (delta_x == 0 && delta_y == 0) ||
                        !graph.correct_travel(travel)) {
                        continue;
                    }
                    // now, this neighbor is correct
                    used.insert(neighbor);
                    double plus;
                    int parent;
                    if (algo_name == "Theta" && !(current.vertex == start) &&
                            (graph.correct_travel(Vector(to_point(previous[current.vertex]), next_vert)))) { // from parent of current to it`s child
                        plus = Vector(to_point(previous[current.vertex]), next_vert).length();
                        parent = previous[current.vertex];
                    } else {
                        plus = Vector(cur, next_vert).length();
                        parent = current.vertex;
                    }
                    double path_length = plus + dist[parent];
                    if (dist.find(neighbor) == dist.end() || (dist[neighbor] > path_length)) {
                        dist[neighbor] = path_length;
                        previous[neighbor] = parent;
                        Q.push(Candidate(this, path_length + h(next_vert, to_point(finish)), neighbor));
                    }
                }
            }
        }
    }

    Path build_answer() const {
        return Path(previous, start, finish, graph.get_width());
    }

};

void solve_with_Theta(const Field& graph, const std::string name_in = "input_aim.txt",
                                          const std::string name_out = "output.txt") {
    std::ifstream in(name_in);
    std::ofstream out(name_out);
    std::pair<int, int> start, finish;
    in >> start.first >> start.second;
    in >> finish.first >> finish.second;
    in.close();
    out << "Theta* algorithm report" << std::endl;
    int start_time = clock();
    Algo solver("Theta", graph, start, finish);
    Path answer = solver.build_answer();
    int end_time = clock();
    out << "Overall runtime: " << (double)(end_time - start_time) / 1000 << " seconds" << std::endl;
    answer.print(out);
    out.close();
}

void solve_with_A_star(const Field& graph, const std::string name_in = "input_aim.txt",
                                           const std::string name_out = "output.txt") {
    std::ifstream in(name_in);
    std::ofstream out(name_out);
    std::pair<int, int> start, finish;
    in >> start.first >> start.second;
    in >> finish.first >> finish.second;
    in.close();
    out << "A* algorithm report" << std::endl;
    int start_time = clock();
    Algo solver("A*", graph, start, finish);
    Path answer = solver.build_answer();
    int end_time = clock();
    out << "Overall runtime: " << (double)(end_time - start_time) / 1000 << " seconds" << std::endl;
    answer.print(out);
    out.close();
}

void solve_both(const Field& graph, const std::string name_in = "input_aim.txt",
                                    const std::string name_out = "output_visual.txt") {
    std::ifstream in(name_in);
    std::ofstream out(name_out);
    std::pair<int, int> start, finish;
    in >> start.first >> start.second;
    in >> finish.first >> finish.second;
    in.close();
    out << "<!DOCTYPE html>\n";
    out << "<html>\n<body>\n\n";
    graph.print_to_visual(out);
    Algo solver_1("A*", graph, start, finish);
    Algo solver_2("Theta", graph, start, finish);
    Path answer_1 = solver_1.build_answer();
    Path answer_2 = solver_2.build_answer();
    answer_1.print_to_visual(out, "green");
    answer_2.print_to_visual(out, "orangered");
    out << "\n</body>\n</body>";
    out.close();
}

};

#endif
