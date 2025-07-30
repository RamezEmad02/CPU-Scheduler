#include "ganttchartdrawer.h"
#include "schedulers.h"
#include "windows.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
/*=================================================================================================================*/
/*=================================================================================================================*/

std::vector<TempProcess*> TempProcess::container;

int Process::processNumber;
int Process::currentTime;
int Process::idle;
Process* Process::currentProcess = nullptr;
Process* Process::oldProcess = nullptr;
Process* Process::stuckProcess = nullptr;
int Process::quantum = 1;
std::vector<Process*> Process::processes;

Process::Process( int arrival, int burst,std::string name, int priority) {
    this->name = name;
    this->arrivalTime = arrival;
    this->burstTime = burst;
    this->priority = priority;
    this->finished = false;
    this->processNo = processNumber++;
    processes.push_back(this);
    this->remainingBurstTime = burst;
    this->firstTime=true;
    this->turnaroundTime=-1;
    this->waitingTime=-1;
    this->startTime=-1;
}

void Process::sortProcesses() {
    // Sort processes by arrival time using a lambda function
    sort(processes.begin(), processes.end(), [](Process* a, Process* b) {
        return a->arrivalTime < b->arrivalTime;
    });
}

void Process::runProcessSJF(int timeSlice) {
    // any time slice but it will processed by 1 sec each
    for (int i = 0; i < timeSlice; ++i) {
        if (remainingBurstTime <= 0) break;
        if(remainingBurstTime == burstTime)
        startTime = currentTime;
        //std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate 1 second
        remainingBurstTime--;
        // currentTime++;

        if (remainingBurstTime == 0) {
            finished = true;
            turnaroundTime = currentTime - arrivalTime +1;
            waitingTime = turnaroundTime - burstTime;
            break;
        }

    }
}

/*=================================================================================================================*/
/*=================================================================================================================*/


GanttChartDrawer::GanttChartDrawer(/*const QVector<Process>& processQueue,*/ QGraphicsScene* scene, QObject* parent)
    : QObject(parent), /*processes(processQueue), */ scene(scene),
    currentTime(0), currentProcessIndex(0), processProgress(0) {
    isIdle = false;
    idleRect = nullptr;
    idleStartTime = 0;
    currentProcessIndex=0;
}

void GanttChartDrawer::startDrawing() {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GanttChartDrawer::tick);
    timer->start(1000);  // Tick every second
}

void GanttChartDrawer::startFast() {
    burst=0;
    if(!Process::processes.empty()){
        for(int i = 0;i<Process::processes.size();i++){
            burst += Process::processes[i]->getBurst();
        }
    }
    else{
        QMessageBox::warning(crt,"Warning","No processes to start with");
        return;
    }

    for (;burstCount<burst; burstCount++){
        tickFast();
    }
    const int unitWidth = 40;
    const int height = 50;
    const int y = 50;

    if (Process::oldProcess && processRects.contains(Process::oldProcess)) {
        int endX = currentTime * unitWidth;

        QGraphicsTextItem* finalTimeLabel = scene->addText(QString::number(currentTime));
        finalTimeLabel->setPos(endX, y + height + 5);
    }

    emit processTableUpdateRequested();
}
/*=================================================================================================================*/


void GanttChartDrawer::tick() {
    Process* process = nullptr;

    std::string x = crt->getSchedulerType();
    bool f = crt->getIsPreemptive();

    timer->stop();

    // Sort and select process
    if (x == "SJF" && f) {
        std::priority_queue<Process*, std::vector<Process*>, CompareBurst> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->PreemptiveSJF(Process::processes);
    }
    else if (x == "SJF" && !f) {
        std::priority_queue<Process*, std::vector<Process*>, CompareBurst> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->nonPreemptiveSJF(Process::processes);
    }
    else if (x == "Priority" && f) {
        std::priority_queue<Process*, std::vector<Process*>, ComparePriority> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->preemptivePriority(Process::processes);
    }
    else if (x == "Priority" && !f) {
        std::priority_queue<Process*, std::vector<Process*>, ComparePriority> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->nonPreemptivePriority(Process::processes);
    }
    else if (x == "FCFS") {
        std::priority_queue<Process*, std::vector<Process*>, CompareArrival> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->FCFS(Process::processes);
    }
    else if (x == "Round Robin") {
        if (!sorted) {
            sorted = true;
            std::priority_queue<Process*, std::vector<Process*>, CompareArrival> readyQueue;
            for (auto* p : Process::processes) readyQueue.push(p);
            Process::processes.clear();
            while (!readyQueue.empty()) {
                Process::processes.push_back(readyQueue.top());
                readyQueue.pop();
            }
        }
        process = sec->roundRobin(Process::processes);
    }
    else {
        timer->stop();
        QMessageBox::warning(crt, "", "error");
        return;
    }

    const int unitWidth = 40;
    const int height = 50;
    const int y = 50;

    if (process == nullptr) {
        if (!isIdle) {
            isIdle = true;
            idleStartTime = currentTime;
            currentX = currentTime * unitWidth;

            idleRect = scene->addRect(currentX, y, 0, height, QPen(Qt::black), QBrush(Qt::red));
            QGraphicsTextItem* label = scene->addText("Idle");
            label->setFont(QFont("Segoe UI", 10, QFont::Bold));
            label->setDefaultTextColor(Qt::white);
            label->setPos(currentX + 5, y + 5);
            QGraphicsTextItem* timeLabel = scene->addText(QString::number(currentTime));
            timeLabel->setPos(currentX, y + height + 5);
        }

        int idleWidth = (currentTime - idleStartTime + 1) * unitWidth;
        idleRect->setRect(idleStartTime * unitWidth, y, idleWidth, height);
        emit newItemAdded();

        currentTime++;
        emit currentTimeUpdated(currentTime);
        emit processTableUpdateRequested();
        timer->start(1000);
        return;
    }

    if (process != Process::oldProcess) {
        if (Process::oldProcess && processRects.contains(Process::oldProcess)) {
            int endX = currentTime * unitWidth;
            QGraphicsTextItem* endTimeLabel = scene->addText(QString::number(currentTime));
            endTimeLabel->setPos(endX, y + height + 5);
        }

        currentX = currentTime * unitWidth;

        // Always create a new visual block even if the process was already drawn before (for resumed processes)
        QGraphicsRectItem* rect = scene->addRect(currentX, y, 0, height, QPen(Qt::black), QBrush(Qt::green));
        processRects[process] = rect;
        processStartX[process] = currentX;

        // Add label
        QString labelStr = process->getName().empty()
                               ? "P" + QString::number(process->getProcessNo() + 1)
                               : QString::fromStdString(process->getName());

        QGraphicsTextItem* label = scene->addText(labelStr);
        label->setFont(QFont("Segoe UI", 10, QFont::Bold));
        label->setDefaultTextColor(Qt::black);
        label->setPos(currentX + 5, y + 5);

        QGraphicsTextItem* timeLabel = scene->addText(QString::number(currentTime));
        timeLabel->setPos(currentX, y + height + 5);

        processProgress = 0;
    }

    if (processRects.contains(process)) {
        int startX = processStartX[process];
        int width = (currentTime + 1 - startX / unitWidth) * unitWidth;
        processRects[process]->setRect(startX, y, width, height);
    }

    isIdle = false;
    Process::oldProcess = process;

    emit newItemAdded();
    currentTime++;
    emit currentTimeUpdated(currentTime);
    emit processTableUpdateRequested();
    timer->start(1000);
}



void GanttChartDrawer::tickFast(){
    Process* process = nullptr;

    std::string x = crt->getSchedulerType();
    bool f = crt->getIsPreemptive();

    // Sort and select process
    if (x == "SJF" && f) {
        std::priority_queue<Process*, std::vector<Process*>, CompareBurst> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->PreemptiveSJF(Process::processes);
    }
    else if (x == "SJF" && !f) {
        std::priority_queue<Process*, std::vector<Process*>, CompareBurst> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->nonPreemptiveSJF(Process::processes);
    }
    else if (x == "Priority" && f) {
        std::priority_queue<Process*, std::vector<Process*>, ComparePriority> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->preemptivePriority(Process::processes);
    }
    else if (x == "Priority" && !f) {
        std::priority_queue<Process*, std::vector<Process*>, ComparePriority> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->nonPreemptivePriority(Process::processes);
    }
    else if (x == "FCFS") {
        std::priority_queue<Process*, std::vector<Process*>, CompareArrival> readyQueue;
        for (auto* p : Process::processes) readyQueue.push(p);
        Process::processes.clear();
        while (!readyQueue.empty()) {
            Process::processes.push_back(readyQueue.top());
            readyQueue.pop();
        }
        process = sec->FCFS(Process::processes);
    }
    else if (x == "Round Robin") {
        if (!sorted) {
            sorted = true;
            std::priority_queue<Process*, std::vector<Process*>, CompareArrival> readyQueue;
            for (auto* p : Process::processes) readyQueue.push(p);
            Process::processes.clear();
            while (!readyQueue.empty()) {
                Process::processes.push_back(readyQueue.top());
                readyQueue.pop();
            }
        }
        process = sec->roundRobin(Process::processes);
    }
    else {
        QMessageBox::warning(crt, "Warning", "error");
        return;
    }

    const int unitWidth = 40;
    const int height = 50;
    const int y = 50;

    if (process == nullptr) {
        if (!isIdle) {
            isIdle = true;
            idleStartTime = currentTime;
            currentX = currentTime * unitWidth;

            idleRect = scene->addRect(currentX, y, 0, height, QPen(Qt::black), QBrush(Qt::red));
            QGraphicsTextItem* label = scene->addText("Idle");
            label->setFont(QFont("Segoe UI", 10, QFont::Bold));
            label->setDefaultTextColor(Qt::white);
            label->setPos(currentX + 5, y + 5);
            QGraphicsTextItem* timeLabel = scene->addText(QString::number(currentTime));
            timeLabel->setPos(currentX, y + height + 5);
        }

        int idleWidth = (currentTime - idleStartTime + 1) * unitWidth;
        idleRect->setRect(idleStartTime * unitWidth, y, idleWidth, height);
        emit newItemAdded();
        burstCount--;
        currentTime++;
        emit currentTimeUpdated(currentTime);
        emit processTableUpdateRequested();
        return;
    }

    if (process != Process::oldProcess) {
        if (Process::oldProcess && processRects.contains(Process::oldProcess)) {
            int endX = currentTime * unitWidth;
            QGraphicsTextItem* endTimeLabel = scene->addText(QString::number(currentTime));
            endTimeLabel->setPos(endX, y + height + 5);
        }

        currentX = currentTime * unitWidth;

        // Always create a new visual block even if the process was already drawn before (for resumed processes)
        QGraphicsRectItem* rect = scene->addRect(currentX, y, 0, height, QPen(Qt::black), QBrush(Qt::green));
        processRects[process] = rect;
        processStartX[process] = currentX;

        // Add label
        QString labelStr = process->getName().empty()
                               ? "P" + QString::number(process->getProcessNo() + 1)
                               : QString::fromStdString(process->getName());

        QGraphicsTextItem* label = scene->addText(labelStr);
        label->setFont(QFont("Segoe UI", 10, QFont::Bold));
        label->setDefaultTextColor(Qt::black);
        label->setPos(currentX + 5, y + 5);

        QGraphicsTextItem* timeLabel = scene->addText(QString::number(currentTime));
        timeLabel->setPos(currentX, y + height + 5);

        processProgress = 0;
    }

    if (processRects.contains(process)) {
        int startX = processStartX[process];
        int width = (currentTime + 1 - startX / unitWidth) * unitWidth;
        processRects[process]->setRect(startX, y, width, height);
    }

    isIdle = false;
    Process::oldProcess = process;

    emit newItemAdded();
    currentTime++;
    emit currentTimeUpdated(currentTime);
    emit processTableUpdateRequested();
}

void GanttChartDrawer::pause() {
    if (timer) timer->stop();
}

void GanttChartDrawer::resume() {
    if (timer) timer->start(1000);
}

/*=================================================================================================================*/
/*=================================================================================================================*/
