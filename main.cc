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
        return a.time > b.time;
}


//Priority Queues to hold the Cars(threads) comming to the intersection 
priority_queue <car> nq;
priority_queue <car> eq;
priority_queue <car> sq;
priority_queue <car> wq;
queue <car> intersection;
std::ofstream outputfile;


vector <bool> readi;
int ggwp = 0;
vector <thread> threadList;
mutex mtx;
condition_variable cv;

void go(string dir, int time, int id) {
     std::unique_lock<std::mutex> lck(mtx);
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

    while(!readi.at(newcar.id)) {
        //cout << "car_" << newcar.id << " waiting" << endl;
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
    outputfile << newcar.time << " " << newcar.direction << endl;

    qstar = NULL;
    delete qstar;


  
}
bool noCollision(car thiscar) {
    if (intersection.empty()) {
        return true;
    }
    queue<car> internet = intersection;

    while(!(internet.empty())) {
    for (size_t i = 0; i < thiscar.cc.size(); i++)
    {
        for (size_t j = 0; j < internet.front().cc.size(); j++)
        {
            if (thiscar.cc.at(i) == internet.front().cc.at(j)){
                return false;
            }
        }
    }
    }
    return true;

    
}
void releaseHelper(int id) {
             std::unique_lock<std::mutex> lck(mtx);
             readi.at(id) = true;
             cv.notify_one();

    }
void release() {

        priority_queue <car> *qstar;
    while (!(nq.empty() && eq.empty() && sq.empty() && wq.empty())) {


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
                //
            if (noCollision(qstar->top())) { //replace with check for not conflicting cars function

        
                //add dir = ith direction
                //lck.unlock();
                releaseHelper(qstar->top().id);
                
                //mtx.unlock();
               // sleep(1);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

        }
        
    }
    qstar = NULL;
    delete qstar;

  
}


int main(int argc,char* argv[]) {
    //car newcar("N",1000,1000);
    //nq.push(newcar);
    //sq.push(newcar);
    //eq.push(newcar);
    //wq.push(newcar);
    if (argc != 2) {
        cout << "Improper use.\nPlease use: ./a.out filename\n";
        exit(EXIT_FAILURE);
    }
    std::chrono::time_point<std::chrono::system_clock> start, end; 

    int temp;
    string temp2;
    ifstream ifs(argv[1], ifstream::in);
    outputfile.open("Output.txt", ofstream::out);
    int i = 0;
    start = std::chrono::system_clock::now(); 
    while (ifs >> temp >> temp2) {

        threadList.push_back(thread(go,temp2,temp,i)); //Create a new thread and add it to the list
        readi.push_back(false);
        i++;
    }
    ifs.close();
    thread mainthread(release);

    for(size_t i = 0; i < threadList.size(); i++) {
        threadList[i].join();
    }
    mainthread.join();
    end = std::chrono::system_clock::now(); 
    outputfile.close();
        std::chrono::duration<double> elapsed_seconds = end - start; 
        std::cout <<"elapsed time: " << elapsed_seconds.count() << "s\n"; 

    
}
