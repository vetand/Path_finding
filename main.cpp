#include<fstream>
#include<iostream>
#include"Collection.h"

using namespace std;

int main(int argc, char* argv[]) {
    spa::Map f;
    f.get_field(argv[1]);
    spa::solve_with_A_star(f);
    return 0;
}
