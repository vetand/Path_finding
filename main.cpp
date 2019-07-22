#include"algo_body.cpp"

using namespace spa2;

int main() {
    /* usual_report
    field graph = get_field("input_field.txt");
    solve_with_Theta(graph, "input_aim.txt", "output_Theta.txt");
    solve_with_A_star(graph, "input_aim.txt", "output_A_star.txt");
    */
    for (int field_num = 0; field_num < 3; ++field_num) {
        std::string input_field_name = "input_field_";
        input_field_name += ('1' + field_num);
        input_field_name += ".txt";
        Field graph = get_field(input_field_name);
        for (int test = 0; test < 4; ++test) {
            std::string input_aim_name = "input_field_";
            input_aim_name += ('1' + field_num);
            input_aim_name += "_aim_";
            input_aim_name += ('1' + test);
            input_aim_name += ".txt";
            std::string output_name = "visual_output_code_";
            output_name += ('1' + field_num);
            output_name += ('1' + test);
            output_name += ".txt";
            solve_both(graph, input_aim_name, output_name);
        }
    }
    return 0;
}
