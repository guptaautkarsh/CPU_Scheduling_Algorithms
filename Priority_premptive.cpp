#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
using namespace std;

/*Assuming lower the priority value higher its priority
  If you want higher the priority value higher its priority then just change '>' sign to 
  '<' sign in the Heap_comparator */

class Process{
    public:
    string process_id;
    int arrival_time;
    int burst_time;
    int priority;
    int waiting_time;
    int turn_around_time;
    int completion_time;

    Process(){
        //default
    }
    
    Process(string id, int at, int bt, int p){
        this->process_id = id;
        this->arrival_time = at;
        this->burst_time = bt;
        this->priority = p;
        this->waiting_time = 0;
    }
};

bool Sort_comparator(Process &p1, Process &p2){
    return p1.arrival_time < p2.arrival_time;
};

class Heap_comparator{
    public:
    bool operator()(pair<int,int> &p1, pair<int,int> &p2){  //<index,priority>
        return p1.second > p2.second;
    }
};

void calculate_priority_times(vector<Process> &process_list, vector<int> &remaining_time){
    int n = process_list.size();
    int completed = 0;
    int time = process_list[0].arrival_time;
    priority_queue<pair<int,int>, vector<pair<int,int>>, Heap_comparator> process_heap;
    int i = 0;

    cout<<endl<<time<<" {";

    while(completed != n){

        while(i<n && process_list[i].arrival_time <= time){
            process_heap.push({i, process_list[i].priority});
            i++;
        }

        if(process_heap.empty()){
            time = process_list[i].arrival_time;
            cout<<"--} "<<time<<" {";
            continue;
        }

        int next_arrival;
        if(i<n)
            next_arrival = process_list[i].arrival_time;
        else
            next_arrival = INT8_MAX;

        int time_lap = next_arrival - time;

        int index = process_heap.top().first;
        int priority_value = process_heap.top().second;
        process_heap.pop();

        if(remaining_time[index]>time_lap){
            remaining_time[index] -= time_lap;
            process_heap.push({index,priority_value});
            time += time_lap;
        }
        else{
            completed++;
            time += remaining_time[index];
            process_list[index].completion_time = time;
            process_list[index].turn_around_time = time - process_list[index].arrival_time;
            process_list[index].waiting_time = process_list[index].turn_around_time - process_list[index].burst_time;
        }
        cout<<process_list[index].process_id<<"} "<<time<<" {";
    }
    cout<<endl;
};

void priority_with_gantt_chart(vector<Process> &process_list){
    int n = process_list.size();
    sort(process_list.begin(), process_list.end(), Sort_comparator);

    vector<int> remaining_time(n);
    for(int i=0; i<n; i++)
        remaining_time[i] = process_list[i].burst_time;

    calculate_priority_times(process_list, remaining_time);
};

void print_priority_table(vector<Process> &tasks_list){
    int n = tasks_list.size();
    cout<<endl<<"Process_id"<<"  "<<"Arrival_time"<<"  "<<"Burst_time"<<"  "
        <<"Priority"<<"  "<<"Waiting_time"<<"  "<<"Turn_around_time"<<"  "
        <<"Completion_time"<<endl;

    for(int i=0; i<n; i++){
        cout<<tasks_list[i].process_id<<setw(13)<<tasks_list[i].arrival_time<<setw(14)
            <<tasks_list[i].burst_time<<setw(12)<<tasks_list[i].priority<<setw(10)
            <<tasks_list[i].waiting_time<<setw(14)<<tasks_list[i].turn_around_time<<setw(19)
            <<tasks_list[i].completion_time<<endl;
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

        int priority;
        cout<<"Enter priority : ";
        cin>>priority;

        Process p(pid, at, bt, priority);
        process_list.push_back(p);
    }

    priority_with_gantt_chart(process_list);
    print_priority_table(process_list);
    print_utils(process_list);

    return 0;
}