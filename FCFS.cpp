#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
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

bool comparator(Process &p1, Process &p2){
    return p1.arrival_time < p2.arrival_time;
};

class FCFS_Scheduling{
    public:
    vector<Process> tasks_list;
    float avg_waiting_time = 0;
    float avg_turn_around_time = 0;
    float scheduling_length = 0;
    float throughput = 0;

    FCFS_Scheduling(vector<Process> &t){
        int n = t.size();
        this->tasks_list = t;
        sort(tasks_list.begin(), tasks_list.end(), comparator);

        if(n>0){
            tasks_list[0].waiting_time = 0;
            tasks_list[0].turn_around_time = tasks_list[0].burst_time;
            tasks_list[0].complition_time = tasks_list[0].arrival_time + tasks_list[0].burst_time;

            avg_turn_around_time = tasks_list[0].turn_around_time;
        }

        for (int i=1; i<n; i++){
            tasks_list[i].waiting_time = tasks_list[i-1].complition_time - tasks_list[i].arrival_time;
            if(tasks_list[i].waiting_time < 0)
                tasks_list[i].waiting_time = 0;
            
            tasks_list[i].turn_around_time = tasks_list[i].waiting_time + tasks_list[i].burst_time;
            tasks_list[i].complition_time = tasks_list[i].arrival_time + tasks_list[i].turn_around_time;

            avg_waiting_time += tasks_list[i].waiting_time;
            avg_turn_around_time += tasks_list[i].turn_around_time;
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
    }

    void gantt_chart(){
        int n = tasks_list.size();
        int time = 0;

        cout<<time<<" |";
        for(int i=0; i<n; i++){
            if(tasks_list[i].arrival_time <= time){
                cout<<tasks_list[i].process_id<<"| "<<tasks_list[i].complition_time<<" |";
                time = tasks_list[i].complition_time;
            }
            else{
                cout<<"--| "<<tasks_list[i].arrival_time<<" |";
                time = tasks_list[i].arrival_time;
                cout<<tasks_list[i].process_id<<"| "<<tasks_list[i].complition_time<<" |";
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

    vector<Process> t;

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
        t.push_back(p);
    }

    FCFS_Scheduling fcfs1(t);
    fcfs1.print_table();

    // cout<<"Average waiting time = "<<fcfs1.avg_waiting_time<<endl;
    // cout<<"Average turn around time = "<<fcfs1.avg_turn_around_time<<endl;
    // cout<<"Scheduling Length = "<<fcfs1.scheduling_length<<endl;
    // cout<<"Throughput = "<<fcfs1.throughput<<endl;

    fcfs1.gantt_chart();

    return 0;
}