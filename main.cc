#include<iostream>
#include <fstream>
#include<queue>
#include<vector>
#include<thread>

using namespace std; 

//struct to hold the Direction and Time read in from the input file.
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


//Priority Queues to hold the Cars(threads) comming to the intersection 
priority_queue <car> nq;
priority_queue <car> eq;
priority_queue <car> sq;
priority_queue <car> wq;
priority_queue <car> intersection;

vector <thread> threadList;

void go(string dir, int time) {
    //Add car to proper direction priority queue
    priority_queue <car> *qstar;
    if (dir.at(0) == 'N') {
        nq.push(car(dir, time));
        qstar = &nq;
    }
    else if (dir.at(0) == 'E') {
        eq.push(car(dir, time));
        qstar = &eq;
    }
    else if (dir.at(0) == 'S') {
        sq.push(car(dir, time));
         qstar = &sq;
    }
    else if (dir.at(0) == 'W') {
        wq.push(car(dir, time)); 
        qstar = &wq;
    }

    // While car is not at top of queue and the release direction is == direction then cv.wait(lock)
    intersection.push(qstar -> top());//posible switch with below
    qstar -> pop();
    //intersection.push(car(dir, time));
    //sleep(5);
    cout <<"Car x is headed directionhere";
    intersection.pop();




  
}
void release() {
    //While all queues are not empty
  
}


int main() {
    


    int temp;
    string temp2;
    vector<car> carsList;
    ifstream ifs("simple.txt", ifstream::in);
    while (ifs >> temp >> temp2) {
        /*
        Outdated Code when i was testing reading in a file.
        car tempcar(temp2, temp);
        carsList.push_back(tempcar);
        */
        threadList.push_back(thread(go,temp2,temp)); //Create a new thread and add it to the list
        
        //CREATE THREADS HERE AND REMOVE ABOVE 2 LINES
    }
    ifs.close();


    
}