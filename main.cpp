#include<fstream>
#include<iostream>
#include"Collection.h"

using namespace std;

int main(int argc, char* argv[]) {
    spa::Map f;
    f.get_field(argv[1]);
    spa::A_star solver(f);
    solver.solve();
    return 0;
}
