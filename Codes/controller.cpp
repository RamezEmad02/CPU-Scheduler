#include "windows.h"

controller::controller(QWidget *parent) : QMainWindow(parent) {
    // setAttribute(Qt::WA_StyledBackground, true);
    // setStyleSheet("QWidget { background-color: black; }");
    setStyleSheet("QWidget { background-color: #1D1D1D; }");
    re = new Reset(this);
    stackedWidget = new QStackedWidget(this);

    schedulerWindow = new SchedulerWindow(this);
    typeWin = new typeWindow(this);
    processInputWindow = new ProcessInputWindow(this);
    ganttWindow = new GanttWindow(this);
    calculationsWindow = new CalculationsWindow(this);
    ganttWindowPlus = new GanttWindowPlus(this);
    calculationsWindowPlus = new CalculationsWindowPlus(this);

    stackedWidget->addWidget(schedulerWindow);       // index 0
    stackedWidget->setCurrentWidget(schedulerWindow);

    stackedWidget->addWidget(typeWin);               // index 1
    stackedWidget->addWidget(processInputWindow);    // index 2
    stackedWidget->addWidget(ganttWindow);           // index 3
    stackedWidget->addWidget(calculationsWindow);
    stackedWidget->addWidget(ganttWindowPlus);
    stackedWidget->addWidget(calculationsWindowPlus);

    setCentralWidget(stackedWidget);

    /*    Sechedular connections     */
    connect(schedulerWindow, &SchedulerWindow::type, this, &controller::type);
    connect(schedulerWindow, &SchedulerWindow::input, this, &controller::input);


    /*    type connections     */
    connect(typeWin, &typeWindow::back, this, &controller::scheduler);
    connect(typeWin, &typeWindow::forward, this, &controller::input);


    /*    process connections     */
    connect(processInputWindow, &ProcessInputWindow::goBack, this, &controller::scheduler);
    connect(processInputWindow, &ProcessInputWindow::front, this, &controller::gantt);
    connect(processInputWindow, &ProcessInputWindow::backFast, this, &controller::ganttFast);

    /*    GanttWindow connecrions */
    connect(ganttWindow, &GanttWindow::forward, this, &controller::calculations);


    /*     Calculations connections */
    connect(calculationsWindow, &CalculationsWindow::back, this, &controller::scheduler);
}


void controller::scheduler() {
    re->fullReset();
    stackedWidget->setCurrentIndex(0);
}


void controller::type() {
    stackedWidget->setCurrentIndex(1);
}

void controller::input() {
    int index = stackedWidget->indexOf(processInputWindow);
    if (index != -1) {
        stackedWidget->removeWidget(processInputWindow);
        delete processInputWindow;

        processInputWindow = new ProcessInputWindow(this);
        stackedWidget->insertWidget(index, processInputWindow);

        //signals
        connect(processInputWindow, &ProcessInputWindow::goBack, this, &controller::scheduler);
        connect(processInputWindow, &ProcessInputWindow::front, this, &controller::gantt);
         connect(processInputWindow, &ProcessInputWindow::backFast, this, &controller::ganttFast);
    }
    stackedWidget->setCurrentIndex(2);
}

void controller::gantt() {
    int index = stackedWidget->indexOf(ganttWindow);
    if (index != -1) {
        stackedWidget->removeWidget(ganttWindow);
        delete ganttWindow;

        ganttWindow = new GanttWindow(this);
        stackedWidget->insertWidget(index, ganttWindow);
        ganttWindow->updateProcessTable();

        //signals
        connect(ganttWindow, &GanttWindow::forward, this, &controller::calculations);
    }
    ganttWindow->updateCurrentTime(0);
    ganttWindow->updateProcessTable();
    stackedWidget->setCurrentIndex(3);
}



void controller::calculations(){
    int index = stackedWidget->indexOf(calculationsWindow);
    if (index != -1) {
        stackedWidget->removeWidget(calculationsWindow);
        delete calculationsWindow;

        calculationsWindow = new CalculationsWindow(this);
        stackedWidget->insertWidget(index, calculationsWindow);

        //signals
        connect(calculationsWindow, &CalculationsWindow::back, this, &controller::scheduler);
    }
    calculationsWindow->setProcessTable();
    calculationsWindow->setDrawer(ganttWindow->getChart());
    calculationsWindow->calculate();
    stackedWidget->setCurrentIndex(4);
}

void controller::ganttFast(){
    int index = stackedWidget->indexOf(ganttWindowPlus);
    if (index != -1) {
        stackedWidget->removeWidget(ganttWindowPlus);
        delete ganttWindowPlus;

        ganttWindowPlus = new GanttWindowPlus(this);
        stackedWidget->insertWidget(index, ganttWindowPlus);
        ganttWindowPlus->updateProcessTable();

        //signals
        connect(ganttWindowPlus, &GanttWindowPlus::forward, this, &controller::calcFast);
    }
    ganttWindowPlus->updateProcessTable();
    stackedWidget->setCurrentIndex(5);
    ganttWindowPlus->startt();
}

void controller::calcFast(){
    int index = stackedWidget->indexOf(calculationsWindowPlus);
    if (index != -1) {
        stackedWidget->removeWidget(calculationsWindowPlus);
        delete calculationsWindowPlus;

        calculationsWindowPlus = new CalculationsWindowPlus(this);
        stackedWidget->insertWidget(index, calculationsWindowPlus);

        //signals
        connect(calculationsWindowPlus, &CalculationsWindowPlus::back, this, &controller::scheduler);
    }
    calculationsWindowPlus->setProcessTable();
    calculationsWindowPlus->setDrawer(ganttWindowPlus->getChart());
    calculationsWindowPlus->calculate();
    stackedWidget->setCurrentIndex(6);
}
