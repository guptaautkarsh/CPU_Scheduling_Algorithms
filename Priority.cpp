#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
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
    }
};

bool Sort_comparator(Process &p1, Process &p2){
    return p1.arrival_time < p2.arrival_time;
};

class Heap_comparator{
    public:
    bool operator()(Process &p1, Process &p2){
        return p1.priority > p2.priority;
    }
};

class Priority_Scheduling{
    public:
    vector<Process> tasks_list;
    float avg_waiting_time = 0;
    float avg_turn_around_time = 0;
    float scheduling_length = 0;
    float throughput = 0;

    Priority_Scheduling(vector<Process> &process_list){
        int n = process_list.size();
        sort(process_list.begin(), process_list.end(), Sort_comparator);

        priority_queue<Process, vector<Process>, Heap_comparator> maxHeap;

        int time = process_list[0].arrival_time;
        int j = 0;
        int i = 0;
        while(i != n){
            while(j<n && process_list[j].arrival_time <= time){
                maxHeap.push(process_list[j]);
                j++;
            }

            if(maxHeap.empty()){
                time = process_list[j].arrival_time;
                continue;
            }
            else{
                tasks_list.push_back(maxHeap.top());
                maxHeap.pop();  
            }

            tasks_list[i].waiting_time = time - tasks_list[i].arrival_time;
            if(tasks_list[i].waiting_time < 0)
                tasks_list[i].waiting_time = 0;
            
            tasks_list[i].turn_around_time = tasks_list[i].waiting_time + tasks_list[i].burst_time;
            tasks_list[i].completion_time = tasks_list[i].arrival_time + tasks_list[i].turn_around_time;

            avg_waiting_time += tasks_list[i].waiting_time;
            avg_turn_around_time += tasks_list[i].turn_around_time;

            time = tasks_list[i].completion_time;
            i++;
        }

        avg_turn_around_time /= n;
        avg_waiting_time /= n;
        scheduling_length = tasks_list[n-1].completion_time - tasks_list[0].arrival_time;
        throughput = n/scheduling_length;
    }

    void print_table(){
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
    }

    void gantt_chart(){
        int n = tasks_list.size();
        int time = 0;

        cout<<endl<<time<<" {";
        for(int i=0; i<n; i++){
            if(tasks_list[i].arrival_time > time){
                cout<<"--} "<<tasks_list[i].arrival_time<<" {";
                time = tasks_list[i].arrival_time;
            }

            cout<<tasks_list[i].process_id<<"} "<<tasks_list[i].completion_time<<" {";
            time = tasks_list[i].completion_time;
        }
        cout<<endl;
    }
};

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

    Priority_Scheduling p1(process_list);
    p1.print_table();

    cout<<"Average waiting time = "<<p1.avg_waiting_time<<endl;
    cout<<"Average turn around time = "<<p1.avg_turn_around_time<<endl;
    cout<<"Scheduling Length = "<<p1.scheduling_length<<endl;
    cout<<"Throughput = "<<p1.throughput<<endl;

    p1.gantt_chart();

    return 0;
}