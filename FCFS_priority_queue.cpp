#include <iostream>
#include <queue>
#include <iomanip>
using namespace std;

// priority queue of vector of size 3
// v[0] contain process id
// v[1] contain arrival time
// v[2] contain burst time

class Comparator{
    public:
    bool operator()(vector<int> &a, vector<int> &b){
        return a[1] > b[1];
    }
};

class Fcfs{
    public:
    priority_queue<vector<int>, vector<vector<int>>, Comparator > process_queue;
    int number_of_process = 0;
    float avg_waiting_time = 0;
    float avg_turn_around_time = 0;
    int start_time = -1;
    float scheduling_length = 0;
    float throughput = 0;

    Fcfs(int n){
        number_of_process = n;
        for(int i=1; i<=n; i++){
            vector<int> process(3);

            cout<<"PROCESS "<<i<<endl;

            cout<<"Enter process id : ";
            cin>>process[0];

            cout<<"Enter arrival time : ";
            cin>>process[1];

            cout<<"Enter burst time : ";
            cin>>process[2];

            process_queue.push(process);
            cout<<endl;
        }
    }

    void print_fcfs_table(){

        cout<<endl<<"Process_id"<<"  "<<"Arrival_time"<<"  "<<"Burst_time"<<"  "
            <<"Waiting_time"<<"  "<<"Turn_around_time"<<"  "
            <<"Complition_time"<<endl;

        int complition_time = 0;
        int waiting_time = 0;
        int turn_around_time = 0;

        while(!process_queue.empty()){
            int process_id = process_queue.top()[0];
            int arrival_time = process_queue.top()[1];
            int burst_time = process_queue.top()[2];

            process_queue.pop();

            if(start_time<0)
                start_time = arrival_time;

            waiting_time = complition_time - arrival_time;
            if(waiting_time<0)
                waiting_time = 0;
            avg_waiting_time += waiting_time;

            turn_around_time = waiting_time + burst_time;
            avg_turn_around_time += turn_around_time;

            complition_time = arrival_time + turn_around_time;

            cout<<process_id<<setw(14)<<arrival_time<<setw(14)<<burst_time<<setw(12)
                <<waiting_time<<setw(14)<<turn_around_time<<setw(19)
                <<complition_time<<endl;
        }

        cout<<endl;

        avg_waiting_time /= number_of_process;
        avg_turn_around_time /= number_of_process;
        scheduling_length = complition_time - start_time;
        throughput = number_of_process/scheduling_length;
    }

    void grand_chart(){
        
    }
};

int main(){
    
    Fcfs f(3);
    f.print_fcfs_table();

    cout<<"Average waiting time = "<<f.avg_waiting_time<<endl;
    cout<<"Average turn around time = "<<f.avg_turn_around_time<<endl;
    cout<<"Scheduling Length = "<<f.scheduling_length<<endl;
    cout<<"Throughput = "<<f.throughput<<endl;

    return 0;
}