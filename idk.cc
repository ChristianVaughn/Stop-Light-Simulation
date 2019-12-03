#include <iostream>
#include <vector>

using namespace std;

struct Car {
    int i;
};

int main() {
    Car bob;
    vector <Car> vec;
    bob.i = 1;
    vec.push_back(bob);

    if (bob == vec.at(0)) {
        cout << "TEST\n";
    }
}