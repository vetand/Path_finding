#include"Collection.h"

namespace spa2 {

    Point Algo::to_point(int vertex) const {
        return Point(vertex % graph.get_width(), vertex / graph.get_width());
    }

    double Algo::heuristic(const Point& A) const {
        return Vector(Point(A.x - to_point(finish).x,
                            A.y - to_point(finish).y), Point(0, 0)).length();
    }

    double Algo::heuristic(int vertex) const {
        return heuristic(to_point(vertex));
    }

    Algo::SearchNode::SearchNode(Algo* input_owner, int x, int y, double dist) :
                            owner(input_owner),
                            id(y * input_owner->graph.get_width() + x),
                            g_value(dist) {
        f_value = owner->heuristic(id) + dist;
    }

    bool Algo::SearchNode::operator < (const SearchNode& other) const {
        if (std::abs(f_value - other.f_value) < eps) {
            if (owner->heuristic(id) - eps < owner->heuristic(other.id)) {
                return true;
            } else {
                return false;
            }
        }
        return (f_value > other.f_value);
    }

    bool Algo::SearchNode::operator == (const SearchNode& other) const {
        return !((*this < other) || (other < *this));
    }

    bool Algo::compare::operator() (const SearchNode* first, const SearchNode* second) const {
        return (*first < *second);
    }

    Algo::Algo(const std::string name, const Map& input_field) :
                                            graph(input_field),
                                            algo_name(name) {
        start = graph.start_vertex.x + graph.get_width() * graph.start_vertex.y;
        finish = graph.finish_vertex.x + graph.get_width() * graph.finish_vertex.y;
        USED[start] = new SearchNode(this, to_point(start).x, to_point(start).y, (double(0)));
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
                    Vector jump = Vector(to_point(current->id), to_point(neighbor_id));
                    if (x < 0 || x >= graph.get_width() ||
                        y < 0 || y > graph.get_height() ||
                        (delta_x == 0 && delta_y == 0) ||
                        !graph.correct_travel(jump)) {
                        continue;
                    }
                    // now, this neighbor is correct
                    double add;
                    SearchNode* set_parent;
                    if (algo_name == "Theta" &&
                        !(current->id == start) && (graph.correct_travel(Vector(
                                                    to_point(current->parent->id),
                                                    to_point(neighbor_id))))) { // from parent of current to it`s child
                        add = Vector(to_point(current->parent->id), to_point(neighbor_id)).length();
                        set_parent = current->parent;
                    } else {
                        add = Vector(to_point(current->id), to_point(neighbor_id)).length();
                        set_parent = current;
                    }
                    if (USED.find(neighbor_id) == USED.end() || USED[neighbor_id]->g_value - eps > set_parent->g_value + add) {
                        USED[neighbor_id] = new SearchNode(this, x, y, set_parent->g_value + add);
                        USED[neighbor_id]->parent = set_parent;
                        OPEN.push(USED[neighbor_id]);
                    }
                }
            }
        }
    }

    Path Algo::build_answer() const {
        Path answer;
        answer.length() = 0;
        int current = finish;
        while (start != current) {
            answer.push(Point(current % graph.get_width(), current / graph.get_width()));
            if (answer.nodes().size() >= 2) {
                answer.length() += Vector(answer.nodes()[answer.nodes().size() - 1],
                                          answer.nodes()[answer.nodes().size() - 2]).length();
            }
            current = (USED.find(current)->second)->parent->id;
        }
        answer.push(Point(start % graph.get_width(), start / graph.get_width()));
        if (answer.nodes().size() >= 2) {
            answer.length() += Vector(answer.nodes()[answer.nodes().size() - 1],
                                      answer.nodes()[answer.nodes().size() - 2]).length();
        }
        answer.flip();
        return answer;
    }

    void solve_with_Theta(const Map& graph, const std::string name_out) {
        std::ofstream out(name_out);
        out << "Theta* algorithm report" << std::endl;
        int start_time = clock();
        Algo solver("Theta", graph);
        Path answer = solver.build_answer();
        int end_time = clock();
        out << "Overall runtime: " << (double)(end_time - start_time) / 1000 << " seconds" << std::endl;
        answer.print(out);
        out.close();
    }

    void solve_with_A_star(const Map& graph, const std::string name_out) {
        std::ofstream out(name_out);
        out << "A* algorithm report" << std::endl;
        int start_time = clock();
        Algo solver("A*", graph);
        Path answer = solver.build_answer();
        int end_time = clock();
        out << "Overall runtime: " << (double)(end_time - start_time) / 1000 << " seconds" << std::endl;
        answer.print(out);
        out.close();
    }

    void solve_both(const Map& graph, const std::string name_out) {
        std::ofstream out(name_out);
        out << "<!DOCTYPE html>\n";
        out << "<html>\n<body>\n\n";
        graph.print_to_visual(out);
        Algo solver_1("A*", graph);
        Algo solver_2("Theta", graph);
        Path answer_1 = solver_1.build_answer();
        Path answer_2 = solver_2.build_answer();
        answer_1.print_to_visual(out, "green");
        answer_2.print_to_visual(out, "orangered");
        out << "\n</body>\n</body>";
        out.close();
    }

};
