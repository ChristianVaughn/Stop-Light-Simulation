#include<iostream>
#include <fstream>
#include<queue>
#include<vector>
#include<thread>
#include <condition_variable>
#include <chrono>


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
string releaseDir;

vector <thread> threadList;
condition_variable cv;

void go(string dir, int time) {
    //Add car to proper direction priority queue
    priority_queue <car> *qstar;
    car newcar(dir,time);
    if (dir.at(0) == 'N') {
        //nq.push(car(dir, time));
        nq.push(newcar);
        qstar = &nq;
    }
    else if (dir.at(0) == 'E') {
        eq.push(newcar);
        qstar = &eq;
    }
    else if (dir.at(0) == 'S') {
        sq.push(newcar);
         qstar = &sq;
    }
    else if (dir.at(0) == 'W') {
        wq.push(newcar); 
        qstar = &wq;
    }
    while((&(qstar->top()) != &newcar) && dir==releaseDir) {
        //cv.wait(&lock);
    }
    intersection.push(qstar -> top());//posible switch with below
    qstar -> pop();
    //intersection.push(car(dir, time));
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    cout <<"Car x is headed directionhere";
    intersection.pop();




  
}
void release() {
    while (!nq.empty() && !eq.empty() && !sq.empty() && !wq.empty()) {
        for (size_t i = 0; i < 4; i++) {
            if (true) { //replace with check for not conflicting cars function
                //add dir = ith direction
                cv.notify_all();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
        
    }
    
  
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