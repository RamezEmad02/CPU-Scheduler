#ifndef SCHEDULERS_H
#define SCHEDULERS_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

class Process;

class Schedulers{

public:
    Schedulers(){}

    //Shortest Job First Functions
    Process* nonPreemptiveSJF(std::vector<Process*> processes);
    Process* PreemptiveSJF(std::vector<Process*> processes);

    // Priority Functions
    Process* nonPreemptivePriority(std::vector<Process*> processes);
    Process* preemptivePriority(std::vector<Process*> processes);

    //FCFS
    Process* FCFS(std::vector<Process*> processes);

    //Round Robin
    Process* roundRobin(std::vector<Process*> processes);
};


#endif // SCHEDULERS_H
