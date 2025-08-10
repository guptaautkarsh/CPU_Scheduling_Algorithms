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
    int completion_time;
    
    Process(string id, int at, int bt){
        this->process_id = id;
        this->arrival_time = at;
        this->burst_time = bt;
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

class HRRN_Scheduling{
    private:
    static bool Sort_comparator(const Process &p1, const Process &p2){
        return p1.arrival_time < p2.arrival_time;
    };

    public:
    vector<Process> tasks_list;
    vector<Gantt> chart;
    float avg_waiting_time;
    float avg_turn_around_time;
    float scheduling_length;
    float throughput;

    HRRN_Scheduling(vector<Process> &process_list){
        this->tasks_list = process_list;
        avg_waiting_time = 0;
        avg_turn_around_time = 0;
        scheduling_length = 0;
        throughput = 0;
    }

    void schedule(){
        int n = tasks_list.size();
        sort(tasks_list.begin(), tasks_list.end(), Sort_comparator);

        vector<Process*> ready_queue;

        int time = tasks_list[0].arrival_time;
        if(time != 0){
            chart.push_back(Gantt(0,"NA",time));
        }
        int nextPushIdx = 0;
        int completed = 0;

        while(completed != n){
            while(nextPushIdx<n && tasks_list[nextPushIdx].arrival_time <= time){
                ready_queue.push_back(&tasks_list[nextPushIdx]);
                nextPushIdx++;
            }

            if(ready_queue.empty()){
                chart.push_back(Gantt(time,"NA",tasks_list[nextPushIdx].arrival_time));
                time = tasks_list[nextPushIdx].arrival_time;
                continue;
            }

            float max_response_ratio = -1.0;
            int max_response_ratio_index = -1;
            for(int i=0; i<ready_queue.size(); i++){
                int wait = time - ready_queue[i]->arrival_time;
                float response_ratio = 1.0*(wait + ready_queue[i]->burst_time)/ready_queue[i]->burst_time;
                if(response_ratio > max_response_ratio){
                    max_response_ratio = response_ratio;
                    max_response_ratio_index = i;
                }
            }

            Process* proc = ready_queue[max_response_ratio_index];

            proc->waiting_time = time - proc->arrival_time;
            proc->turn_around_time = proc->waiting_time + proc->burst_time;
            proc->completion_time = proc->arrival_time + proc->turn_around_time;

            chart.push_back(Gantt(time,proc->process_id,proc->completion_time));
            
            ready_queue.erase(ready_queue.begin() + max_response_ratio_index);

            time = proc->completion_time;
            completed++;
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

    HRRN_Scheduling hrrn_scheduler(process_list);

    hrrn_scheduler.schedule();
    hrrn_scheduler.calculate_metrics();

    hrrn_scheduler.print_table();
    hrrn_scheduler.print_matrices();
    hrrn_scheduler.print_gantt_chart();

    return 0;
}