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
    vector <int> cc;
    car(string dir, int tim, int nid) {
        direction = dir;
        time = tim;
        id = nid;
        if(direction == "N") {
            cc.push_back(1);
            cc.push_back(3); 
        }
        else if (direction == "NW")
        {
            cc.push_back(3);
            cc.push_back(0); 

        }
        else if (direction == "NE")
        {
            /* code */
            cc.push_back(3);
        }
        else if (direction == "S")
        {
            /* code */
            cc.push_back(0);
            cc.push_back(2); 
        }
        else if (direction == "SW")
        {
            /* code */
            cc.push_back(0);
            cc.push_back(3); 
        }
        else if (direction == "SE")
        {
            /* code */
            cc.push_back(0);
        }
        else if (direction == "W")
        {
            /* code */
            cc.push_back(0);
            cc.push_back(1); 
        }
        else if (direction == "WN")
        {
            /* code */
            cc.push_back(1);
            cc.push_back(2); 
        }
        else if (direction == "WS")
        {
            /* code */
            cc.push_back(1);
        }
        else if (direction == "E")
        {
            /* code */
            cc.push_back(2);
            cc.push_back(3); 
        }
        else if (direction == "EN")
        {
            /* code */
            cc.push_back(2);
            cc.push_back(1); 
        }
        else if (direction == "ES")
        {
            /* code */
            cc.push_back(2);
        }
        
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
vector <bool> readi;
int ggwp = 0;
string releaseDir = "F";

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
    cout <<"Car ID: " << newcar.id << " has arrived " << newcar.direction << endl;
    while(!readi.at(id)) {
        //cout << "IWORKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n";
        cv.wait(lck);
    }
    
    intersection.push(qstar -> top());//posible switch with below
    qstar -> pop();
    //intersection.push(car(dir, time,id));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    //sleep(5);
    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    cout <<"Car ID: " << newcar.id << " is headed " << newcar.direction << endl;
    intersection.pop();




  
}
bool noCollision(priority_queue<car> *qstar) {
    if (intersection.empty()) {
        return true;
    }
    for (size_t i = 0; i < qstar->top().cc.size(); i++)
    {
        for (size_t j = 0; j < intersection.top().cc.size(); j++)
        {
            if (qstar->top().cc.at(i) == intersection.top().cc.at(j)){
                return false;
            }
        }
    }
    return true;
    
}
void releaseHelper() {
             std::unique_lock<std::mutex> lck(mtx);
             cv.notify_one();

    }
void release() {
        priority_queue <car> *qstar;
    while (!nq.empty() && !eq.empty() && !sq.empty() && !wq.empty()) {

        for (size_t i = 0; i < 4; i++) {
            //mtx.lock();
             if ( i == 0) {
                    qstar = &nq;
                }
                else if(i == 1){
                    qstar = &eq;

                }
                else if(i == 2) {
                    qstar = &sq;
                }
                else if (i == 3) {
                    qstar = &wq;
                }
                //noCollision(qstar)
            if (true) { //replace with check for not conflicting cars function

                if ( i == 0) {
                    releaseDir = "N";
                    cout << releaseDir << endl;

                }
                else if(i == 1){
                    releaseDir = "E";
                                        cout << releaseDir << endl;


                }
                else if(i == 2) {
                    releaseDir = "S";
                                        cout << releaseDir << endl;

                }
                else if (i == 3) {
                    releaseDir = "W";
                    cout << releaseDir << endl;
                }
                //add dir = ith direction
                //lck.unlock();
                releaseHelper();
                
                cout << "REEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n";

                //mtx.unlock();
                //sleep(1);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

        }
        
    }
    
  
}


int main() {
    


    int temp;
    string temp2;
    vector<car> carsList;
    ifstream ifs("simple.txt", ifstream::in);
    int i = 0;
    while (ifs >> temp >> temp2) {

        threadList.push_back(thread(go,temp2,temp,i)); //Create a new thread and add it to the list
        readi.push_back(i);
        i++;
    }
    ifs.close();
    thread mainthread(release);

    for(size_t i = 0; i < threadList.size(); i++) {
        threadList[i].join();
    }
    mainthread.join();

    
}
