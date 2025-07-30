#include "schedulers.h"
#include "ganttchartdrawer.h"


Process* Schedulers::roundRobin(std::vector<Process*> processes) {
    static std::queue<Process*> readyQueue;
    static int quantumCounter = 0;
    const int quantum = Process::quantum;
    int currentTime = Process::getCurrentTime();

    // Fill readyQueue with new arrivals (only once)
    for (auto p : processes) {
        if (p->getArrival() == currentTime && !p->getFinished()) {
            readyQueue.push(p);
        }
    }

    // If current process finished
    if (Process::stuckProcess && Process::stuckProcess->getFinished()) {
        Process::stuckProcess = nullptr;
        quantumCounter = 0;
    }

    // Pick a new process from readyQueue if needed
    if (!Process::stuckProcess && !readyQueue.empty()) {
        Process::stuckProcess = readyQueue.front();
        readyQueue.pop();
        quantumCounter = 0;
    }

    if (Process::stuckProcess) {
        Process* current = Process::stuckProcess;
        current->runProcessSJF(1);
        quantumCounter++;

        if (current->getRemainingBurstTime() == 0) {
            current->setFinished(true);
            Process::stuckProcess = nullptr;
            quantumCounter = 0;
        } else if (quantumCounter == quantum) {
            // Time slice expired, requeue the process
            readyQueue.push(current);
            Process::stuckProcess = nullptr;
            quantumCounter = 0;
        }

        return current;
    }

    return nullptr;
}


/* ==================================================================================*/
/* ==================================================================================*/

Process* Schedulers::nonPreemptivePriority(std::vector<Process*> processes) {
    int currentTime= Process::getCurrentTime();
    std::priority_queue<Process*, std::vector<Process*>, ComparePriority> readyQueue;
    for (auto p : processes) {
        if (p->getArrival() <= currentTime && !p->getFinished()) {
            readyQueue.push(p);
        }
    }

    // If the current process finished, clear stuckProcess
    if (Process::stuckProcess && Process::stuckProcess->getFinished()) {
        Process::stuckProcess = nullptr;
    }

    // If no stuck process and something is ready, select the next one
    if (!Process::stuckProcess && !readyQueue.empty()) {
        Process::stuckProcess = readyQueue.top();
        readyQueue.pop();
    }

    // Run the stuck process if there is one
    if (Process::stuckProcess) {
        Process* currentProcess = Process::stuckProcess;
        currentProcess->runProcessSJF(1);

        // If it finishes during this run, mark it done
        if (currentProcess->getRemainingBurstTime() == 0) {
            currentProcess->setFinished(true);
            Process::stuckProcess = nullptr;
        }
        return currentProcess;
    }

    // If nothing is ready or running
    return nullptr;
}

Process* Schedulers::preemptivePriority(std::vector<Process*> processes) {
    int currentTime= Process::getCurrentTime();
    std::priority_queue<Process*, std::vector<Process*>, ComparePriority> readyQueue;
        // Add all processes that have arrived to the ready queue
    for (auto p : processes) {
        if (p->getArrival() <= currentTime && !p->getFinished()) {
            readyQueue.push(p);
        }
    }

    // If there are no processes in the ready queue, increment time
    if (readyQueue.empty()) {
        // currentTime++;
        return nullptr;
    }
    // Get the process with the shortest remaining burst time
    Process* currentProcess = readyQueue.top();
    //readyQueue.pop();
    // Run the process for 1 time unit
    currentProcess->runProcessSJF(1); // Run for 1 time unit
    if (currentProcess->getRemainingBurstTime() == 0) {
        currentProcess->setFinished(true);
        // completedProcesses++;
    }
    // else loop again and push the lowest burst time
    return currentProcess;
}


/* ==================================================================================*/
/* ==================================================================================*/

Process* Schedulers::nonPreemptiveSJF(std::vector<Process*> processes) {
    int currentTime = Process::getCurrentTime();
    std::priority_queue<Process*, std::vector<Process*>, CompareBurst> readyQueue;

    // Build ready queue
    for (auto p : processes) {
        if (p->getArrival() <= currentTime && !p->getFinished()) {
            readyQueue.push(p);
        }
    }

    // If the current process finished, clear stuckProcess
    if (Process::stuckProcess && Process::stuckProcess->getFinished()) {
        Process::stuckProcess = nullptr;
    }

    // If no stuck process and something is ready, select the next one
    if (!Process::stuckProcess && !readyQueue.empty()) {
        Process::stuckProcess = readyQueue.top();
        readyQueue.pop();
    }

    // Run the stuck process if there is one
    if (Process::stuckProcess) {
        Process* currentProcess = Process::stuckProcess;
        currentProcess->runProcessSJF(1);

        // If it finishes during this run, mark it done
        if (currentProcess->getRemainingBurstTime() == 0) {
            currentProcess->setFinished(true);
            Process::stuckProcess = nullptr;
        }
        return currentProcess;
    }

    // If nothing is ready or running
    return nullptr;
}

Process* Schedulers::PreemptiveSJF(std::vector<Process*> processes) {
    int currentTime= Process::getCurrentTime();
    // int completedProcesses = 0;
    // to sort it automatically by remaining burst time
    //vector<Process*> ganttChart;  // leave it empty for now
    // while (completedProcesses < processes->size()) {
    std::priority_queue<Process*, std::vector<Process*>, CompareBurst> readyQueue;
        // Add all processes that have arrived to the ready queue
    for (auto p : processes) {
        if (p->getArrival() <= currentTime && !p->getFinished()) {
            readyQueue.push(p);
        }
    }

    // If there are no processes in the ready queue, increment time
    if (readyQueue.empty()) {
        // currentTime++;
    return nullptr;
    }
    // Get the process with the shortest remaining burst time
    Process* currentProcess = readyQueue.top();
    //readyQueue.pop();
    // Run the process for 1 time unit
    currentProcess->runProcessSJF(1); // Run for 1 time unit
    if (currentProcess->getRemainingBurstTime() == 0) {
        currentProcess->setFinished(true);
        // completedProcesses++;
    }
    // else loop again and push the lowest burst time
    return currentProcess;

}

/* ==================================================================================*/
/* ==================================================================================*/
Process* Schedulers::FCFS( std::vector<Process*> processes){
    int currentTime = Process::getCurrentTime();
    std::priority_queue<Process*, std::vector<Process*>, CompareArrival> readyQueue;

    // Build ready queue
    for (auto p : processes) {
        if (p->getArrival() <= currentTime && !p->getFinished()) {
            readyQueue.push(p);
        }
    }

    // Build ready queue
    for (auto p : processes) {
        if (p->getArrival() <= currentTime && !p->getFinished()) {
            readyQueue.push(p);
        }
    }

    // If the current process finished, clear stuckProcess
    if (Process::stuckProcess && Process::stuckProcess->getFinished()) {
        Process::stuckProcess = nullptr;
    }

    // If no stuck process and something is ready, select the next one
    if (!Process::stuckProcess && !readyQueue.empty()) {
        Process::stuckProcess = readyQueue.top();
        readyQueue.pop();
    }

    // Run the stuck process if there is one
    if (Process::stuckProcess) {
        Process* currentProcess = Process::stuckProcess;
        currentProcess->runProcessSJF(1);

        // If it finishes during this run, mark it done
        if (currentProcess->getRemainingBurstTime() == 0) {
            currentProcess->setFinished(true);
            Process::stuckProcess = nullptr;
        }
        return currentProcess;
    }

    // If nothing is ready or running
    return nullptr;
}

