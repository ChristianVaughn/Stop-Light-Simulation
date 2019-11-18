#include<iostream>
#include <fstream>
#include<queue>
#include<vector>
#include<thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <unistd.h>



using namespace std; 

//struct to hold the Direction and Time read in from the input file.
struct car {
    string direction;
    int time;
    int id;
    car(string dir, int tim, int nid) {
        direction = dir;
        time = tim;
        id = nid;
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
mutex mtx;
condition_variable cv;

void go(string dir, int time, int id) {
     std::unique_lock<std::mutex> lck(mtx);
    //Add car to proper direction priority queue
    priority_queue <car> *qstar;
    car newcar(dir,time,id);
    if (dir.at(0) == 'N') {
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
        cv.wait(lck);
    }
    intersection.push(qstar -> top());//posible switch with below
    qstar -> pop();
    intersection.push(car(dir, time,id));
    sleep(1);
    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    cout <<"Car ID: " << newcar.id << " is headed " << newcar.direction << endl;
    intersection.pop();




  
}
void release() {
    while (!nq.empty() && !eq.empty() && !sq.empty() && !wq.empty()) {
        for (size_t i = 0; i < 4; i++) {
            mtx.lock();
            if (true) { //replace with check for not conflicting cars function
                if ( i == 1) {
                    releaseDir = "N";
                }
                else if(i == 2){
                    releaseDir = "E";

                }
                else if(i == 3) {
                    releaseDir = "S";
                }
                else if (i == 4) {
                    releaseDir = "W";
                }
                //add dir = ith direction
                cv.notify_one();
                mtx.unlock();
                sleep(1);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
        
    }
    
  
}


int main() {
    


    int temp;
    string temp2;
    vector<car> carsList;
    ifstream ifs("medium.txt", ifstream::in);
    thread mainthread(release);
    int i = 0;
    while (ifs >> temp >> temp2) {

        threadList.push_back(thread(go,temp2,temp,i)); //Create a new thread and add it to the list
        i++;
    }
    ifs.close();
    for(size_t i = 0; i < threadList.size(); i++) {
        threadList[i].join();
    }
    mainthread.join();

    
}