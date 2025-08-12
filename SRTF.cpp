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
    
    Process(string id, int at, int bt){
        this->process_id = id;
        this->arrival_time = at;
        this->burst_time = bt;
        this->waiting_time = 0;
    }
};

class Gantt{
    public:
    int start;
    string pid;
    int end;

    Gantt(int s, string p, int e){
        start = s;
        pid = p;
        end = e;
    }
};

class Heap_comparator{
    public:
    bool operator()(const pair<int,int> &p1, const pair<int,int> &p2){  //<index,remaining time>
        if(p1.second == p2.second){
            return p1.first > p2.first;
        }
        return p1.second > p2.second;
    }
};

class SRTF_Scheduling{
    private:
    static bool Sort_comparator(const Process &p1, const Process &p2){
        if(p1.arrival_time == p2.arrival_time){
            return p1.process_id < p2.process_id;
        }
        return p1.arrival_time < p2.arrival_time;
    };

    public:
    vector<Process> tasks_list;
    vector<Gantt> chart;
    float avg_waiting_time;
    float avg_turn_around_time;
    float scheduling_length;
    float throughput;

    SRTF_Scheduling(vector<Process> &process_list){
        this->tasks_list = process_list;
        avg_waiting_time = 0;
        avg_turn_around_time = 0;
        scheduling_length = 0;
        throughput = 0;
    }

    void schedule(){
        int n = tasks_list.size();
        sort(tasks_list.begin(), tasks_list.end(), Sort_comparator);

        priority_queue<pair<int,int>, vector<pair<int,int>>, Heap_comparator> ready_queue;

        int time = tasks_list[0].arrival_time;
        if(time != 0){
            chart.push_back(Gantt(0,"NA",time));
        }
        int nextPushIdx = 0;
        int completed = 0;

        while(completed != n){
            while(nextPushIdx<n && tasks_list[nextPushIdx].arrival_time <= time){
                ready_queue.push({nextPushIdx, tasks_list[nextPushIdx].burst_time});
                nextPushIdx++;
            }

            if(ready_queue.empty()){
                chart.push_back(Gantt(time,"NA",tasks_list[nextPushIdx].arrival_time));
                time = tasks_list[nextPushIdx].arrival_time;
                continue;
            }

            int next_arrival;
            if(nextPushIdx < n)
                next_arrival = tasks_list[nextPushIdx].arrival_time;
            else
                next_arrival = INT8_MAX;

            int time_lap = next_arrival - time;

            int index = ready_queue.top().first;
            int remaining_time = ready_queue.top().second;
            ready_queue.pop();

            if(remaining_time > time_lap){
                remaining_time -= time_lap;
                ready_queue.push({index,remaining_time});
                if(!chart.empty() && chart.back().pid == tasks_list[index].process_id){
                    chart.back().end = time+time_lap;
                }
                else{
                    chart.push_back(Gantt(time,tasks_list[index].process_id,time+time_lap));
                }
                time += time_lap;
            }
            else{
                completed++;
                if(!chart.empty() && chart.back().pid == tasks_list[index].process_id){
                    chart.back().end = time+remaining_time;
                }
                else{
                    chart.push_back(Gantt(time,tasks_list[index].process_id,time+remaining_time));
                }
                time += remaining_time;
                tasks_list[index].completion_time = time;
                tasks_list[index].turn_around_time = time - tasks_list[index].arrival_time;
                tasks_list[index].waiting_time = tasks_list[index].turn_around_time - tasks_list[index].burst_time;
            }
        }
    }

    void calculate_metrics(){
        int n = tasks_list.size();
        int max_comp_time = 0;
        for(int i=0; i<n; i++){
            avg_waiting_time += tasks_list[i].waiting_time;
            avg_turn_around_time += tasks_list[i].turn_around_time;
            max_comp_time = max(max_comp_time,tasks_list[i].completion_time);
        }

        avg_turn_around_time /= n;
        avg_waiting_time /= n;
        scheduling_length = max_comp_time - tasks_list[0].arrival_time;
        throughput = n/scheduling_length;
    }

    void print_matrices(){
        cout<<"Average waiting time = "<<avg_waiting_time<<endl;
        cout<<"Average turn around time = "<<avg_turn_around_time<<endl;
        cout<<"Scheduling Length = "<<scheduling_length<<endl;
        cout<<"Throughput = "<<throughput<<endl;
        cout<<endl;
    }

    void print_table(){
        int n = tasks_list.size();
        cout<<endl<<' ';
        fill('-',86);
        cout<<endl;

        cout<<left;
        cout << "|";
        cout<< setw(11) << "Process_id" << "|"
            << setw(13) << "Arrival_time" << "|"
            << setw(11) << "Burst_time" << "|"
            << setw(13) << "Waiting_time"  << "|"
            << setw(17) << "Turn_around_time" << "|"
            << setw(16) << "Completion_time" << "|" <<endl;

        for(int i=0; i<n; i++){
            cout << "|";
            cout<< setw(11) << tasks_list[i].process_id << "|"
                << setw(13) << tasks_list[i].arrival_time << "|" 
                << setw(11) << tasks_list[i].burst_time << "|"
                << setw(13) << tasks_list[i].waiting_time << "|"
                << setw(17) << tasks_list[i].turn_around_time << "|"
                << setw(16) << tasks_list[i].completion_time  << "|" <<endl;
        }

        cout<<' ';
        fill('-',86);
        cout<<endl;
    }

    void print_gantt_chart(){
        int n = chart.size();

        cout<<' ';
        for(int i=0; i<n; i++){
            int t = chart[i].end - chart[i].start;
            fill('-',4*t);
            cout<<' ';
        };

        cout<<endl;

        cout<<'|';
        for(int i=0; i<n; i++){
            int t = chart[i].end - chart[i].start;
            fill(' ',(4*t-2)/2);
            
            cout<<chart[i].pid;
            
            fill(' ',(4*t-2)/2);

            cout<<'|';
        }

        cout<<endl;

        cout<<0;
        for(int i=0; i<n; i++){
            int t = chart[i].end - chart[i].start;

            if(chart[i].start>99)
                fill('-',4*t-2);
            else if(chart[i].start>9)
                fill('-',4*t-1);
            else
                fill('-',4*t);

            cout<<chart[i].end;
        };

        cout<<endl;
    }

    void fill(char c, int freq){
        for(int i=0; i<freq; i++){
            cout<<c;
        }
    }
};

int main(){
    
    int n;
    cout<<"Enter number of processes : ";
    cin>>n;

    vector<Process> process_list;

    for(int i=1; i<=n; i++){
        cout<<"PROCESS "<<i<<endl;

        string pid = 'P' + to_string(i);
        
        int at;
        cout<<"Enter arrival time : ";
        cin>>at;

        int bt;
        cout<<"Enter burst time : ";
        cin>>bt;

        process_list.push_back(Process(pid, at, bt));
    }

    SRTF_Scheduling srtf_scheduler(process_list);

    srtf_scheduler.schedule();
    srtf_scheduler.calculate_metrics();

    srtf_scheduler.print_table();
    srtf_scheduler.print_matrices();
    srtf_scheduler.print_gantt_chart();

    return 0;
}