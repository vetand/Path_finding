#ifndef Path_finder_description
#define Path_finder_description
#include<algorithm>
#include<queue>
#include<ctime>
#include<unordered_map>
#include"field.h"
#include"path.h"
#include"print.h"

namespace spa {

const double eps = 1e-8; // less delta means that potentials are equal

class A_star {
private:

    double h(int vertex) { // this is R2 metric
        int x_current = vertex % graph.get_width();
        int y_current = vertex / graph.get_width();
        int x_finish = finish % graph.get_width();
        int y_finish = finish / graph.get_width();
        int delta_x = x_finish - x_current;
        int delta_y = y_finish - y_current;
        return sqrt(delta_x * delta_x + delta_y * delta_y);
    }

    class Candidate {
    public:

        A_star* owner;
        double potential;
        int vertex;

        Candidate(A_star* input_owner) : potential(0), vertex(0), owner(input_owner) {}

        Candidate(A_star* input_owner, double input_pot, int input_vert) :
                                            potential(input_pot), vertex(input_vert), owner(input_owner) {}

        bool operator < (const Candidate& other) const {
            if (std::abs(potential - other.potential) < eps) {
                if (owner->h(vertex) - eps < owner->h(other.vertex)) {
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

    int start; // (x, y) -> y * width + x - to lower overall memory size
    int finish;
    std::unordered_set<int> used;
    std::priority_queue<Candidate> Q;
    std::unordered_map<int, int> previous;
    std::unordered_set<int> ready;
    std::unordered_map<int, double> dist;
    const spa::Field& graph;

public:

    A_star(const Field& input_field, std::pair<int, int> start_vertex,
                                     std::pair<int, int> finish_vertex) : graph(input_field) {
        int start_x = start_vertex.first - 1;
        int start_y = graph.get_height() - start_vertex.second;
        int finish_x = finish_vertex.first - 1;
        int finish_y = graph.get_height() - finish_vertex.second;
        start = start_x + graph.get_width() * start_y;
        finish = finish_x + graph.get_width() * finish_y;
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
            for (int delta_x = -1; delta_x <= 1; ++delta_x) {
                for (int delta_y = -1; delta_y <= 1; ++delta_y) {
                    int x = current.vertex % graph.get_width() + delta_x;
                    int y = current.vertex / graph.get_width() + delta_y;
                    int neighbor = y * graph.get_width() + x;
                    if (x < 0 || x >= graph.get_width() ||
                        y < 0 || y >= graph.get_height() ||
                        (delta_x == 0 && delta_y == 0) ||
                        !graph.is_free(x, y)) {
                        continue;
                    }
                    // now, this neighbor is correct
                    used.insert(neighbor);
                    double plus;
                    int angle = std::abs(delta_x) + std::abs(delta_y);
                    if (angle == 2) {
                        plus = sqrt(2);
                    } else {
                        plus = 1;
                    }
                    double path_length = plus + dist[current.vertex];
                    if (dist.find(neighbor) == dist.end() || (dist[neighbor] > path_length)) {
                        dist[neighbor] = path_length;
                        previous[neighbor] = current.vertex;
                        Q.push(Candidate(this, path_length + h(neighbor), neighbor));
                    }
                }
            }
        }
    }

    Path find_path() const {
        return Path(graph, previous, start, finish);
    }

    int marked() const {
        return used.size();
    }

};

void solve_with_A_star(const Field& graph, std::pair<int, int> start,
                                           std::pair<int, int> finish, const std::string& name = "output.txt") {
    A_star solver(graph, start, finish);
    Path answer = solver.find_path();
    std::ofstream out(name);
    out << "algorithm reviewed " << solver.marked()
        << " free points from " << graph.available() << " available\n";
    print_with_path(graph, answer, out);
    out.close();
}

};

#endif
