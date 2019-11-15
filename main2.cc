#include<iostream>
#include <fstream>
#include<queue>
#include<vector>
#include<thread>

using namespace std; 
struct car {
    string direction;
    int time;
    car(string dir, int tim) {
        direction = dir;
        time = tim;
    }
    
};
bool operator<(const car& a, const car& b) {
        return a.time < b.time;
}

int main() {
    car testcar("N",5);
    car *carloc = &testcar;
    priority_queue <car> queue;
    queue.push(car("N",5));
    car idk = queue.top();
    cout << &testcar << endl;
    cout << idk.time << endl;

}
