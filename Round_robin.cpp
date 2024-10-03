#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
using namespace std;

class Process{
    public:
    string process_id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turn_around_time;
    int completion_time;

    Process(){
        //default
    }
    
    Process(string id, int at, int bt){
        this->process_id = id;
        this->arrival_time = at;
        this->burst_time = bt;
        this->waiting_time = 0;
    }
};

bool Sort_comparator(Process &p1, Process &p2){
    return p1.arrival_time < p2.arrival_time;
};

void calculate_round_robin_times(vector<Process> &process_list, vector<int> remaining_time, int timeQuantum){
    int n = process_list.size();
    int completed = 0;
    int time = process_list[0].arrival_time;
    queue<int> ready_queue; //store index of process
    int to_be_added = -1;
    int i = 0;

    cout<<endl<<time<<" {";

    while(completed != n){

        while(i<n && process_list[i].arrival_time <= time){
            ready_queue.push(i);
            i++;
        }

        if(to_be_added != -1)
            ready_queue.push(to_be_added);

        if(ready_queue.empty()){
            time = process_list[i].arrival_time;
            cout<<"--} "<<time<<" {";
            continue;
        }

        int index = ready_queue.front();
        ready_queue.pop();

        if(remaining_time[index] > timeQuantum){
            remaining_time[index] -= timeQuantum;
            to_be_added = index;
            time += timeQuantum;
        }
        else{
            completed++;
            time += remaining_time[index];
            to_be_added = -1;
            process_list[index].completion_time = time;
            process_list[index].turn_around_time = time - process_list[index].arrival_time;
            process_list[index].waiting_time = process_list[index].turn_around_time - process_list[index].burst_time;
        }
        cout<<process_list[index].process_id<<"} "<<time<<" {";
    }
    cout<<endl;
};

void round_robin_with_gantt_chart(vector<Process> &process_list, int timeQuantum){
    int n = process_list.size();
    sort(process_list.begin(), process_list.end(), Sort_comparator);

    vector<int> remaining_time(n);
    for(int i=0; i<n; i++)
        remaining_time[i] = process_list[i].burst_time;

    calculate_round_robin_times(process_list, remaining_time, timeQuantum);
};

void print_round_robin_table(vector<Process> &tasks_list){
    int n = tasks_list.size();
    cout<<endl<<"Process_id"<<"  "<<"Arrival_time"<<"  "<<"Burst_time"<<"  "
        <<"Waiting_time"<<"  "<<"Turn_around_time"<<"  "
        <<"Completion_time"<<endl;

    for(int i=0; i<n; i++){
        cout<<tasks_list[i].process_id<<setw(13)<<tasks_list[i].arrival_time<<setw(14)
            <<tasks_list[i].burst_time<<setw(12)<<tasks_list[i].waiting_time<<setw(14)
            <<tasks_list[i].turn_around_time<<setw(19)<<tasks_list[i].completion_time<<endl;
    }
    cout<<endl;
};

void print_utils(vector<Process> &process_list){
    int n = process_list.size();
    float avg_waiting_time = 0;
    float avg_turn_around_time = 0;
    float scheduling_length;
    float throughput;

    for(int i = 0; i<n; i++){
        avg_waiting_time += process_list[i].waiting_time;
        avg_turn_around_time += process_list[i].turn_around_time;
    }

    avg_waiting_time /= n;
    avg_turn_around_time /= n;
    scheduling_length = process_list[n-1].completion_time - process_list[0].arrival_time;
    throughput = n/scheduling_length;

    cout<<"Average waiting time = "<<avg_waiting_time<<endl;
    cout<<"Average turn around time = "<<avg_turn_around_time<<endl;
    cout<<"Scheduling Length = "<<scheduling_length<<endl;
    cout<<"Throughput = "<<throughput<<endl;
}

int main(){
    
    int n;
    cout<<"Enter number of processes : ";
    cin>>n;

    int timeQuantum;
    cout<<"Enter Time Quantum : ";
    cin>>timeQuantum;

    vector<Process> process_list;

    for(int i=1; i<=n; i++){
        cout<<"PROCESS "<<i<<endl;

        string pid;
        cout<<"Enter process id : ";
        cin>>pid;
        
        int at;
        cout<<"Enter arrival time : ";
        cin>>at;

        int bt;
        cout<<"Enter burst time : ";
        cin>>bt;

        Process p(pid, at, bt);
        process_list.push_back(p);
    }

    round_robin_with_gantt_chart(process_list, timeQuantum);
    print_round_robin_table(process_list);
    print_utils(process_list);

    return 0;
}