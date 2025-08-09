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

    Process(){
        process_id = "NA";
    }
    
    Process(string id, int at, int bt){
        this->process_id = id;
        this->arrival_time = at;
        this->burst_time = bt;
    }
};

class Gantt{
    public:
    int start;
    Process p;
    int end;

    Gantt(int s, Process p, int e){
        start = s;
        this->p = p;
        end = e;
    }
};

class FCFS_Scheduling{
    private:
    static bool comparator(const Process &p1, const Process &p2){
        return p1.arrival_time < p2.arrival_time;
    };

    public:
    vector<Process> tasks_list;
    vector<Gantt> chart;
    float avg_waiting_time;
    float avg_turn_around_time;
    float scheduling_length;
    float throughput;

    FCFS_Scheduling(vector<Process> &t){
        this->tasks_list = t;
        avg_waiting_time = 0;
        avg_turn_around_time = 0;
        scheduling_length = 0;
        throughput = 0;
    }

    void schedule(){
        int n = tasks_list.size();
        sort(tasks_list.begin(), tasks_list.end(), comparator);
        int time = tasks_list[0].arrival_time;

        for(int i=0; i<n; i++){
            tasks_list[i].waiting_time = time - tasks_list[i].arrival_time;
            if(tasks_list[i].waiting_time < 0)
                tasks_list[i].waiting_time = 0;
            
            tasks_list[i].turn_around_time = tasks_list[i].waiting_time + tasks_list[i].burst_time;
            tasks_list[i].completion_time = tasks_list[i].arrival_time + tasks_list[i].turn_around_time;

            time = tasks_list[i].completion_time;
        }
    }

    void calculate_metrics(){
        int n = tasks_list.size();
        for(int i=0; i<n; i++){
            avg_waiting_time += tasks_list[i].waiting_time;
            avg_turn_around_time += tasks_list[i].turn_around_time;
        }

        avg_turn_around_time /= n;
        avg_waiting_time /= n;
        scheduling_length = tasks_list[n-1].completion_time - tasks_list[0].arrival_time;
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

    void gantt_chart(){
        int n = tasks_list.size();
        int time = 0;

        for(int i=0; i<n; i++){
            if(tasks_list[i].arrival_time > time){
                chart.push_back(Gantt(time,Process(),tasks_list[i].arrival_time));
                time = tasks_list[i].arrival_time;
            }

            chart.push_back(Gantt(time,tasks_list[i],tasks_list[i].completion_time));
            time = tasks_list[i].completion_time;
        }

        print_chart(chart);

        cout<<endl;
    }

    void print_chart(vector<Gantt> &chart){
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
            
            cout<<chart[i].p.process_id;
            
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

    FCFS_Scheduling fcfs_scheduler(process_list);

    fcfs_scheduler.schedule();
    fcfs_scheduler.calculate_metrics();

    fcfs_scheduler.print_table();
    fcfs_scheduler.print_matrices();
    fcfs_scheduler.gantt_chart();

    return 0;
}