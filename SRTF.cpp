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
    int complition_time;

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

class Heap_comparator{
    public:
    bool operator()(pair<int,int> &p1, pair<int,int> &p2){  //<index,remaining time>
        return p1.second > p2.second;
    }
};

void calculate_srtf_times(vector<Process> &process_list){
    int n = process_list.size();
    int completed = 0;
    int time = process_list[0].arrival_time;
    priority_queue<pair<int,int>, vector<pair<int,int>>, Heap_comparator> process_heap;
    int i = 0;

    cout<<endl<<time<<" {";

    while(completed != n){

        while(i<n && process_list[i].arrival_time <= time){
            process_heap.push({i, process_list[i].burst_time});
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
        int rt = process_heap.top().second;
        process_heap.pop();

        if(rt>time_lap){
            rt -= time_lap;
            process_heap.push({index,rt});
            time += time_lap;
        }
        else{
            completed++;
            time += rt;
            process_list[index].complition_time = time;
            process_list[index].turn_around_time = time - process_list[index].arrival_time;
            process_list[index].waiting_time = process_list[index].turn_around_time - process_list[index].burst_time;
        }
        cout<<process_list[index].process_id<<"} "<<time<<" {";
    }
    cout<<endl;
};

void srtf_with_gantt_chart(vector<Process> &process_list){
    int n = process_list.size();
    sort(process_list.begin(), process_list.end(), Sort_comparator);

    calculate_srtf_times(process_list);
};

void print_srtf_table(vector<Process> &tasks_list){
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

    srtf_with_gantt_chart(process_list);
    print_srtf_table(process_list);

    return 0;
}