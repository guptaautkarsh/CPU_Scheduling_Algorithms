#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <queue>
using namespace std;

class Process{
    public:
    string process_id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turn_around_time;
    int complition_time;

    Process(){
        //default
    }
    
    Process(string id, int at, int bt){
        this->process_id = id;
        this->arrival_time = at;
        this->burst_time = bt;
    }
};

bool Sort_comparator(Process &p1, Process &p2){
    return p1.arrival_time < p2.arrival_time;
};

class Heap_comparator{
    public:
    bool operator()(Process &p1, Process &p2){
        return p1.burst_time > p2.burst_time;
    }
};

class SJF_Scheduling{
    public:
    vector<Process> tasks_list;
    float avg_waiting_time = 0;
    float avg_turn_around_time = 0;
    float scheduling_length = 0;
    float throughput = 0;

    SJF_Scheduling(vector<Process> &process_list){
        int n = process_list.size();
        sort(process_list.begin(), process_list.end(), Sort_comparator);

        priority_queue<Process, vector<Process>, Heap_comparator> minHeap;

        int time = process_list[0].arrival_time;
        int j = 0;
        for(int i=0; i<n; i++){
            while(j<n && process_list[j].arrival_time <= time){
                minHeap.push(process_list[j]);
                j++;
            }

            if(minHeap.empty()){
                tasks_list.push_back(process_list[j]);
                j++;
            }
            else{
                tasks_list.push_back(minHeap.top());
                minHeap.pop();  
            }

            if(i==0){
                tasks_list[0].waiting_time = 0;
                tasks_list[0].turn_around_time = tasks_list[0].burst_time;
                tasks_list[0].complition_time = tasks_list[0].arrival_time + tasks_list[0].turn_around_time;

                avg_turn_around_time = tasks_list[0].turn_around_time;
            }
            else{
                tasks_list[i].waiting_time = tasks_list[i-1].complition_time - tasks_list[i].arrival_time;
                if(tasks_list[i].waiting_time < 0)
                    tasks_list[i].waiting_time = 0;
                
                tasks_list[i].turn_around_time = tasks_list[i].waiting_time + tasks_list[i].burst_time;
                tasks_list[i].complition_time = tasks_list[i].arrival_time + tasks_list[i].turn_around_time;

                avg_waiting_time += tasks_list[i].waiting_time;
                avg_turn_around_time += tasks_list[i].turn_around_time;
            }

            time = tasks_list[i].complition_time;
        }

        avg_turn_around_time /= n;
        avg_waiting_time /= n;
        scheduling_length = tasks_list[n-1].complition_time - tasks_list[0].arrival_time;
        throughput = n/scheduling_length;
    }

    void print_table(){
        int n = tasks_list.size();
        cout<<endl<<"Process_id"<<"  "<<"Arrival_time"<<"  "<<"Burst_time"<<"  "
            <<"Waiting_time"<<"  "<<"Turn_around_time"<<"  "
            <<"Complition_time"<<endl;

        for(int i=0; i<n; i++){
            cout<<tasks_list[i].process_id<<setw(13)<<tasks_list[i].arrival_time<<setw(14)
                <<tasks_list[i].burst_time<<setw(12)<<tasks_list[i].waiting_time<<setw(14)
                <<tasks_list[i].turn_around_time<<setw(19)<<tasks_list[i].complition_time<<endl;
        }
        cout<<endl;
    }

    void gantt_chart(){
        int n = tasks_list.size();
        int time = 0;

        cout<<endl<<time<<" {";
        for(int i=0; i<n; i++){
            if(tasks_list[i].arrival_time <= time){
                cout<<tasks_list[i].process_id<<"} "<<tasks_list[i].complition_time<<" {";
                time = tasks_list[i].complition_time;
            }
            else{
                cout<<"--} "<<tasks_list[i].arrival_time<<" {";
                time = tasks_list[i].arrival_time;
                cout<<tasks_list[i].process_id<<"} "<<tasks_list[i].complition_time<<" {";
                time = tasks_list[i].complition_time;
            }
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

        Process p(pid, at, bt);
        process_list.push_back(p);
    }

    SJF_Scheduling sjf1(process_list);
    sjf1.print_table();

    cout<<"Average waiting time = "<<sjf1.avg_waiting_time<<endl;
    cout<<"Average turn around time = "<<sjf1.avg_turn_around_time<<endl;
    cout<<"Scheduling Length = "<<sjf1.scheduling_length<<endl;
    cout<<"Throughput = "<<sjf1.throughput<<endl;

    sjf1.gantt_chart();

    return 0;
}