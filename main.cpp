#include<fstream>
#include<iostream>
#include"a_star.h"
#include"field.h"
#include"path.h"
#include"print.h"

using namespace std;

int main() {
    spa::Field f = spa::get_field("input_example.txt");
    spa::solve_with_A_star(f, {50, 100}, {49, 2});
    return 0;
}
