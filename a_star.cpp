#ifndef Path_finder_description
#define Path_finder_description
#include<algorithm>
#include<queue>
#include<ctime>
#include<unordered_map>
#include"map.cpp"
#include"path.cpp"
#include"print.cpp"

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
        return std::abs(delta_x) + std::abs(delta_y);
    }

    class SearchNode {
    public:

        A_star* owner;
        int id;
        double g_value;
        double f_value;
        SearchNode* parent;

        SearchNode(A_star* input_owner, int x, int y, double dist) :
                                        owner(input_owner),
                                        id(y * input_owner->graph.get_width() + x),
                                        g_value(dist) {
            f_value = owner->h(id) + dist;
        }

        bool operator < (const SearchNode& other) const {
            if (std::abs(f_value - other.f_value) < eps) {
                if (owner->h(id) - eps < owner->h(other.id)) {
                    return true;
                } else {
                    return false;
                }
            }
            return (f_value > other.f_value);
        }

        bool operator == (const SearchNode& other) const {
            return !((*this < other) || (other < *this));
        }

    };

    struct compare {
        bool operator() (const SearchNode* first, const SearchNode* second) const {
            return (*first < *second);
        }
    };

    int start; // (x, y) -> y * width + x - to lower overall memory size
    int finish;
    std::priority_queue<SearchNode*, std::vector<SearchNode*>, compare> OPEN;
    std::unordered_set<int> CLOSED;
    std::unordered_map<int, SearchNode*> USED; // vertexes from OPEN and CLOSED
    const spa::Map& graph;

public:

    A_star(const Map& input_field) : graph(input_field) {

        int start_x = graph.start_vertex.first - 1;
        int start_y = graph.get_height() - graph.start_vertex.second;
        int finish_x = graph.finish_vertex.first - 1;
        int finish_y = graph.get_height() - graph.finish_vertex.second;
        start = start_x + graph.get_width() * start_y;
        finish = finish_x + graph.get_width() * finish_y;
        USED[start] = new SearchNode(this, start_x, start_y, (double(0)));
        OPEN.push(USED[start]);

        while (!OPEN.empty() && OPEN.top()->id != finish) {
            SearchNode* current = OPEN.top();
            OPEN.pop();
            CLOSED.insert(current->id);
            for (int delta_x = -1; delta_x <= 1; ++delta_x) {
                for (int delta_y = -1; delta_y <= 1; ++delta_y) {
                    int x = current->id % graph.get_width() + delta_x;
                    int y = current->id / graph.get_width() + delta_y;
                    int neighbor_id = y * graph.get_width() + x;
                    if (x < 0 || x >= graph.get_width() ||
                        y < 0 || y >= graph.get_height() ||
                        (delta_x == 0 && delta_y == 0) ||
                        !graph.is_free(x, y)) {
                        continue;
                    }
                    // now, this neighbor is correct
                    double add = sqrt(std::abs(delta_x) + std::abs(delta_y));
                    if (USED.find(neighbor_id) == USED.end() || USED[neighbor_id]->g_value > current->g_value + add) {
                        USED[neighbor_id] = new SearchNode(this, x, y, current->g_value + add);
                        USED[neighbor_id]->parent = current;
                        OPEN.push(USED[neighbor_id]);
                    }
                }
            }
        }
    }

    Path build_path() {
        Path answer(graph);
        int current = finish;
        while (current != start) {
            answer.push(current);
            if (USED[current]->parent->id != start) {
                answer.len() += answer.dist(USED[current]->parent->id, current);
            }
            current = USED[current]->parent->id;
        }
        answer.push(start);
        return answer;
    }

    int marked() const {
        return USED.size();
    }

};

void solve_with_A_star(const Map& graph, const std::string& name = "output.txt") {
    A_star solver(graph);
    Path answer = solver.build_path();
    std::ofstream out(name);
    out << "algorithm reviewed " << solver.marked()
        << " free points from " << graph.available() << " available\n";
    print_with_path(graph, answer, out);
    out.close();
}

};

#endif
