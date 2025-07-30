#ifndef WINDOWS_H
#define WINDOWS_H
/*=================================================================================================================*/

#include "ganttchartdrawer.h"
// #include "schedulers.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMovie>
#include <QFont>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableWidget>
#include <QLineEdit>
#include <QFrame>
#include <QStackedWidget>
#include <QMainWindow>
#include <QScrollBar>
#include <QHeaderView>


class controller;
class Reset;

/*=================================================================================================================*/
/*=================================================================================================================*/

class CalculationsWindowPlus : public QWidget
{
    Q_OBJECT

public:
    CalculationsWindowPlus(controller* crt,QWidget *parent = nullptr);

    void setProcessTable();
    void setDrawer(QGraphicsScene* sharedScene);
    void setAvgTurn(float x){avgTurnaround=x;}
    void setAvgWait(float x){avgWaiting=x;}

    void calculate();

signals:
    void back();

private:
    controller* crt;

    QLabel *titleLabel;
    QLabel *ganttChartLabel;
    QTableWidget *processTable;
    QLabel *avgWaitLabel;
    QLabel *avgTurnaroundLabel;
    QPushButton *backButton;

    QGraphicsScene* ganttScene;
    QGraphicsView* view;
    GanttChartDrawer* drawer = nullptr;

    QLabel* responseValue;
    QLabel* waitValue;
    QLabel* turnaroundValue;


    float avgTurnaround;
    float avgWaiting;
    float avgResponse;

    void close();
};


/*=================================================================================================================*/
/*=================================================================================================================*/

class CalculationsWindow : public QWidget
{
    Q_OBJECT

public:
    CalculationsWindow(controller* crt,QWidget *parent = nullptr);

    void setProcessTable();
    void setDrawer(QGraphicsScene* sharedScene);
    void setAvgTurn(float x){avgTurnaround=x;}
    void setAvgWait(float x){avgWaiting=x;}

    void calculate();

signals:
    void back();

private:
    controller* crt;

    QLabel *titleLabel;
    QLabel *ganttChartLabel;
    QTableWidget *processTable;
    QLabel *avgWaitLabel;
    QLabel *avgTurnaroundLabel;
    QPushButton *backButton;

    QGraphicsScene* ganttScene;
    QGraphicsView* view;
    GanttChartDrawer* drawer = nullptr;

    QLabel* responseValue;
    QLabel* waitValue;
    QLabel* turnaroundValue;


    float avgTurnaround;
    float avgWaiting;
    float avgResponse;

    void close();
};


/*=================================================================================================================*/
/*=================================================================================================================*/

class GanttWindowPlus : public QWidget {
    Q_OBJECT

public:
    GanttWindowPlus(controller* crt,QWidget* parent = nullptr);
    controller* crt;

    void updateCurrentTime(int time);
    void scrollToRightEdge();
    void updateProcessTable();

    QTableWidget* getTable(){return processTable;}
    GanttChartDrawer* getDrawer(){return drawer;}
    QGraphicsScene* getChart(){return ganttScene;}
    void startt(){starting();}


signals:
    void forward();

private:
    QLineEdit* nameInput;
    QLineEdit* arrivalInput;
    QLineEdit* burstInput;
    QLineEdit* timeField;
    QLineEdit* priorityInput;
    QPushButton* start;
    QGraphicsScene* ganttScene;
    QGraphicsView* view;
    QVector<Process> queue;
    QTableWidget* processTable;
    QPushButton* pauseButton;
    QPushButton* exitButton;
    QPushButton* addProcess;
    GanttChartDrawer* drawer = nullptr;

    bool isPaused = false;
    bool flag = true;


    friend class Reset;
private slots:
    void starting();
    void terminate();

};


/*=================================================================================================================*/
/*=================================================================================================================*/

class GanttWindow : public QWidget {
    Q_OBJECT

public:
    GanttWindow(controller* crt,QWidget* parent = nullptr);
    controller* crt;
    void updateCurrentTime(int time);
    void scrollToRightEdge();
    void updateProcessTable();

    QTableWidget* getTable(){return processTable;}
    GanttChartDrawer* getDrawer(){return drawer;}
    QGraphicsScene* getChart(){return ganttScene;}



signals:
    void forward();

private:
    QLineEdit* nameInput;
    QLineEdit* arrivalInput;
    QLineEdit* burstInput;
    QLineEdit* timeField;
    QLineEdit* priorityInput;
    QPushButton* start;
    QGraphicsScene* ganttScene;
    QGraphicsView* view;
    QVector<Process> queue;
    QTableWidget* processTable;
    QPushButton* pauseButton;
    QPushButton* exitButton;
    QPushButton* addProcess;
    GanttChartDrawer* drawer = nullptr;

    bool isPaused = false;
    bool flag = true;

    friend class Reset;
private slots:
    void adding();
    void validateInputs();
    void starting();
    void terminate();

};


/*=================================================================================================================*/
/*=================================================================================================================*/


class ProcessInputWindow : public QWidget {
    Q_OBJECT

public:
    ProcessInputWindow(controller* crt,QWidget* parent = nullptr);
    controller* crt;

signals:
    void front();
    void goBack();
    void backFast();

private slots:
    void validateInputs();
    void addProcess();
    void startSimulation();
    void backward();
    void fast();

private:
    QLineEdit *nameInput, *arrivalInput, *burstInput, *idleStartInput, *priorityinput,*quantumInput;
    QPushButton *enterButton, *startSimButton, *back, *hoba;
    QTableWidget *processTable;

    void extractProcessesFromTable() const;


    friend class Reset;
};

/*=================================================================================================================*/
/*=================================================================================================================*/


class typeWindow : public QWidget {
    Q_OBJECT

public:
    typeWindow(controller* crt,QWidget *parent = nullptr);
    controller* crt;

signals:
    void back();
    void forward();

private:
    std::string type;
    QVBoxLayout* createGifButtonBlock(const QString &gifPath, const QString &buttonText, const char* slotMethod);

    void reset (){}
    friend class Reset;

private slots:
    void onButton1Clicked();
    void onButton2Clicked();
    void goBack();
};


/*=================================================================================================================*/
/*=================================================================================================================*/

class SchedulerWindow : public QWidget {
    Q_OBJECT

public:
    SchedulerWindow(controller* crt,QWidget *parent = nullptr);
    controller* crt;

signals:
    void type();
    void input();
    void sechdularType(QString x);

private:
    QVBoxLayout* createGifButtonBlock(const QString &gifPath, const QString &buttonText, const char* slotMethod);
    QSize sizeHint() const;

    void reset (){}
    friend class Reset;
private slots:
    void onButton1Clicked();
    void onButton2Clicked();
    void onButton3Clicked();
    void onButton4Clicked();
};

/*=================================================================================================================*/
/*=================================================================================================================*/

class controller : public QMainWindow {
    Q_OBJECT
public:
    controller(QWidget *parent = nullptr);
    Reset* re;

    std::string getSchedulerType(){return schedulerType;}
    bool getIsPreemptive(){return isPreemptive;}
    void setSchedulerType(std::string x){schedulerType = x; }
    void setIsPreemptive(bool const &x){isPreemptive = x; }
    CalculationsWindow* getCalcWindow(){return calculationsWindow;}

private:
    QStackedWidget *stackedWidget;

    GanttWindowPlus *ganttWindowPlus;
    SchedulerWindow *schedulerWindow;
    typeWindow *typeWin;
    ProcessInputWindow *processInputWindow;
    GanttWindow *ganttWindow;
    CalculationsWindow *calculationsWindow;
    CalculationsWindowPlus *calculationsWindowPlus;

    std::string schedulerType;
    bool isPreemptive;

    void reset (){
        schedulerType="";
        isPreemptive = false;
    }

    friend class Reset;

private slots:
    void type();
    void input();
    void gantt();
    void scheduler();
    void calculations();
    void ganttFast();
    void calcFast();
};
/*=================================================================================================================*/
/*=================================================================================================================*/
class Reset {
public:
    controller* crt;

    Reset(controller* crt) {
        this->crt = crt;
    }

    void fullReset() {
        if (!crt) return;

        crt->reset();
        Process::reset();

        resetProcessInputWindow(crt->processInputWindow);
        resetGanttWindow(crt->ganttWindow);
    }

private:

    void resetProcessInputWindow(ProcessInputWindow* win) {
        if (!win) return;

        win->nameInput->clear();
        win->arrivalInput->clear();
        win->burstInput->clear();
        win->idleStartInput->clear();
        win->priorityinput->clear();
        win->quantumInput->clear();
        win->processTable->setRowCount(0);
    }

    void resetGanttWindow(GanttWindow* win) {
        if (!win) return;
        win->nameInput->clear();
        win->arrivalInput->clear();
        win->burstInput->clear();
        win->timeField->clear();
        win->processTable->setRowCount(0);
        win->queue.clear();
        win->isPaused = false;
        win->flag = true;
        if (win->drawer) {
            win->drawer->reset();
        }
    }
};








#endif // WINDOWS_H
