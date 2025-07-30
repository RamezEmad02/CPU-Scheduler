#ifndef GANTTCHARTDRAWER_H
#define GANTTCHARTDRAWER_H

#include <QObject>
#include <QGraphicsScene>
#include <QVector>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <queue>

class Schedulers;
class controller;
class Reset;
/*=================================================================================================================*/
/*=================================================================================================================*/


class TempProcess {
public:
    std::string name;
    int priority;
    int burstTime;
    int arrivalTime;

    static std::vector<TempProcess*> container;

    TempProcess(int arrival, int burst , std::string name = "" , int priority = 1){
        arrivalTime = arrival;
        burstTime = burst;
        name = name;
        this->priority=priority;
        container.push_back(this);
    }
};


/*=================================================================================================================*/
/*=================================================================================================================*/
class Process{
private:

    static int currentTime;
    static int idle;


    std::string name;
    int priority;
    int burstTime;
    int arrivalTime;
    int startTime;
    int turnaroundTime;
    int waitingTime;
    int remainingBurstTime;

    bool finished;
    int processNo;
    bool firstTime;

    static void reset(){
        currentTime = 0;
        idle = 0;
        processNumber = 0;
        processes.clear();
        currentProcess = nullptr;
        stuckProcess = nullptr;
        oldProcess = nullptr;
        quantum = 1;
    }
    friend class Reset;

public:
    static int processNumber;
    static std::vector<Process*> processes;
    static Process* currentProcess;
    static Process* stuckProcess; //non-preemptive use
    static Process* oldProcess;   //drawing use
    static int quantum;

    Process( int arrival, int burst,std::string name = "" , int priority = 1);

    // Getters
    std::string getName() const {return name;}
    int getBurst() const { return burstTime; }
    int getArrival() const { return arrivalTime; }
    int getPriority() const { return priority; }
    bool getFinished() const { return finished; }
    int getRemainingBurstTime() const { return remainingBurstTime; }
    void decrementBurst(int amount) {
        burstTime -= amount;
    }

    int getStartTime() const { return startTime; }
    int getTurnaroundTime() const { return turnaroundTime; }
    int getWaitingTime() const { return waitingTime; }
    int getProcessNo() const { return processNo; }
    bool isFirstTime() const { return firstTime; }

    void static setIdle (int x){idle=x;}
    void static setCurrentTime (int x){currentTime=x;}
    int static getCurrentTime (){return currentTime;}
    void setFinished(bool status) { finished = status; }
    void setFirstTime(bool status) { firstTime = status; }
    void setStartTime(int time) { startTime = time; }
    void setRemainingBurstTime(int time) { remainingBurstTime = time; }

    static void sortProcesses();
    void runProcessSJF(int timeSlice);
    // void runProcess(int time);

};
/*=================================================================================================================*/
/*=================================================================================================================*/

struct CompareBurst {
    bool operator()(Process* a, Process* b) {
        if(a->getRemainingBurstTime() == b->getRemainingBurstTime() && a->getArrival() == b->getArrival()){
            return a->getProcessNo() > b->getProcessNo();
        }
        else if (a->getRemainingBurstTime() == b->getRemainingBurstTime()) {
            // If remaining burst times are the same, use arrival time as a tiebreaker (min heap)
            return a->getArrival() > b->getArrival();
        }
        return a->getRemainingBurstTime() > b->getRemainingBurstTime();
    }
};

// the smaller the priority number the higher the priority (min heap)
struct ComparePriority {
    bool operator()(Process* a, Process* b) {
        //changed heere
        if (a->getPriority() == b->getPriority() && a->getArrival() == b->getArrival() )  {
            return a->getProcessNo() > b->getProcessNo();
        }
        else if (a->getPriority() == b->getPriority()) {
            // If priorities are the same, use arrival time as a tiebreaker (min heap)
            return a->getArrival() > b->getArrival();
        }
        return a->getPriority() > b->getPriority();
    }
};

struct CompareArrival {
    bool operator()(Process* a, Process* b) {
        if (a->getArrival() == b->getPriority() && a->getBurst() == b->getBurst() )  {
            return a->getProcessNo() > b->getProcessNo();
        }
        else if (a->getArrival() == b->getArrival()) {
            // If arrivals are the same, use burst time as a tiebreaker (min heap)
            return a->getBurst() > b->getBurst();
        }
        return a->getArrival() > b->getArrival();
    }
};
/*=================================================================================================================*/
/*=================================================================================================================*/

class GanttChartDrawer : public QObject {
    Q_OBJECT

public:
    GanttChartDrawer(/*const QVector<Process>& processQueue,*/ QGraphicsScene* scene, QObject* parent = nullptr);
    void startDrawing();
    void startFast();

    controller* crt;
    Schedulers* sec;


signals:
    void currentTimeUpdated(int x);
    void newItemAdded();
    void processTableUpdateRequested();

public slots:
    void pause();
    void resume();

private slots:
    void tick();
    void tickFast();

private:
    // QVector<Process> processes;
    QGraphicsScene* scene;
    int currentTime;
    int currentProcessIndex;
    int processProgress;
    bool isIdle;
    bool sorted = false;
    std::queue<Process*> RR;
    QGraphicsRectItem* idleRect;
    int idleStartTime;
    int burstCount = 0;
    int burst = 0;

    QTimer* timer;

    QGraphicsRectItem* currentRect;
    int currentX;

    QMap<Process*, QGraphicsRectItem*> processRects;
    QMap<Process*, int> processStartX;

    void reset(){

    }

    friend class Reset;
};

/*=================================================================================================================*/
/*=================================================================================================================*/

#endif // GANTTCHARTDRAWER_H
