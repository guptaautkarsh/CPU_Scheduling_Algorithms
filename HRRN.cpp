#include <iostream>
#include <vector>
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
        //default
    }
    
    Process(string id, int at, int bt){
        this->process_id = id;
        this->arrival_time = at;
        this->burst_time = bt;
    }
};

class HRRN_Scheduling{
    public:
    vector<Process> tasks_list;
    float avg_waiting_time = 0;
    float avg_turn_around_time = 0;
    float scheduling_length = 0;
    float throughput = 0;

    HRRN_Scheduling(vector<Process> &process_list){
        int n = process_list.size();
        int time = process_list[0].arrival_time;
        int j = 0;
        vector<bool> executed(n,false);

        int completed = 0;
        while(completed != n){
            int maxHrrn = -1;
            int maxHrrn_index = -1;

            for(int j=0; j<n; j++){
                if(process_list[j].arrival_time <= time && executed[j]==false){
                    int hrrn = (time - process_list[j].arrival_time + process_list[j].burst_time)/process_list[j].burst_time;
                    if(hrrn > maxHrrn){
                        maxHrrn = hrrn;
                        maxHrrn_index = j;
                    }
                }
            }

            if(maxHrrn == -1){
                int min_arrival_time = INT16_MAX;
                for(int j=0; j<n; j++){
                    if(!executed[j] && process_list[j].arrival_time < min_arrival_time)
                        min_arrival_time = process_list[j].arrival_time;
                }
                
                time = min_arrival_time;
                continue;
            }

            process_list[maxHrrn_index].waiting_time = time - process_list[maxHrrn_index].arrival_time;
            process_list[maxHrrn_index].turn_around_time = process_list[maxHrrn_index].waiting_time + process_list[maxHrrn_index].burst_time;
            process_list[maxHrrn_index].completion_time = time + process_list[maxHrrn_index].burst_time;
            time = process_list[maxHrrn_index].completion_time;
            executed[maxHrrn_index] = true;
            tasks_list.push_back(process_list[maxHrrn_index]);

            avg_waiting_time += process_list[maxHrrn_index].waiting_time;
            avg_turn_around_time += process_list[maxHrrn_index].turn_around_time;

            completed++;
        }

        avg_waiting_time /= n;
        avg_turn_around_time /= n;
        scheduling_length = tasks_list[n-1].completion_time - tasks_list[0].arrival_time;
        throughput = n/scheduling_length;
    }

    void print_hrrn_table(){
        int n = tasks_list.size();
        cout<<endl<<"Process_id"<<"  "<<"Arrival_time"<<"  "<<"Burst_time"<<"  "
            <<"Waiting_time"<<"  "<<"Turn_around_time"<<"  "
            <<"Complition_time"<<endl;

        for(int i=0; i<n; i++){
            cout<<tasks_list[i].process_id<<setw(13)<<tasks_list[i].arrival_time<<setw(14)
                <<tasks_list[i].burst_time<<setw(12)<<tasks_list[i].waiting_time<<setw(14)
                <<tasks_list[i].turn_around_time<<setw(19)<<tasks_list[i].completion_time<<endl;
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

        Process p(pid, at, bt);
        process_list.push_back(p);
    }

    HRRN_Scheduling hrrn1(process_list);
    hrrn1.print_hrrn_table();

    cout<<"Average waiting time = "<<hrrn1.avg_waiting_time<<endl;
    cout<<"Average turn around time = "<<hrrn1.avg_turn_around_time<<endl;
    cout<<"Scheduling Length = "<<hrrn1.scheduling_length<<endl;
    cout<<"Throughput = "<<hrrn1.throughput<<endl;

    hrrn1.gantt_chart();

    return 0;
}