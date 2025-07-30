#include "windows.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

GanttWindowPlus::GanttWindowPlus(controller* crt,QWidget* parent)
    : QWidget(parent),crt(crt) {
    setWindowTitle("Chart");
    resize(800, 500);
    setStyleSheet("background-color: #202020; font-family: 'Segoe UI'; font-size: 14px; color: white;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFrame* ganttFrame = new QFrame;
    ganttFrame->setMinimumHeight(200);
    ganttFrame->setStyleSheet(
        "background-color: #19647E;"
        "border-radius: 30px;"
        "border: 1px solid #0d2d3a;"
        "box-shadow: 0px 4px 8px rgba(0,0,0,0.1);"
        );
    QVBoxLayout* ganttLayout = new QVBoxLayout(ganttFrame);


    QLabel* chartLabel = new QLabel("Gantt chart");
    chartLabel->setAlignment(Qt::AlignCenter);
    chartLabel->setStyleSheet("color: white; font-size: 18px; font-weight: 500;");
    ganttLayout->addWidget(chartLabel);



    ganttScene = new QGraphicsScene(this);
    view = new QGraphicsView(ganttScene);
    view->setMinimumHeight(120);  // adjust as needed
    view->setStyleSheet("background-color: #1a3a4a; border: none;");
    view->setRenderHint(QPainter::Antialiasing);
    view->fitInView(ganttScene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ganttLayout->addWidget(view);

    mainLayout->addWidget(ganttFrame);


    processTable = new QTableWidget(this);
    QStringList headers;
    if(crt->getSchedulerType()=="Priority"){
        processTable->setColumnCount(4);
        headers = {"Name", "Arrival","Burst", "Priority"};
    }
    else{
        processTable->setColumnCount(3);
        headers = {"Name", "Arrival", "Burst"};
    }
    processTable->setHorizontalHeaderLabels(headers);
    processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    processTable->setStyleSheet(
        "QTableWidget {"
        "    color: white;"
        "    background-color: #1e1e1e;"
        "    border: none;"
        "}"
        "QHeaderView::section {"
        "    background-color: #2d2d2d;"
        "    color: white;"
        "    border: none;"
        "}"
        );
    processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(processTable);



    QHBoxLayout* controlLayout = new QHBoxLayout;

    exitButton = new QPushButton("Show Calculations");
    exitButton->setStyleSheet(R"(
    QPushButton {
        background-color: #af1c1c;
        color: white;
        font-weight: bold;
        border: none;
        padding: 10px 20px;
        border-radius: 6px;
    }
    QPushButton:hover {
        background-color: #ff1c1c;
    }
)");

    controlLayout->addWidget(exitButton);
    mainLayout->addLayout(controlLayout);

    connect(exitButton, &QPushButton::clicked, this, &GanttWindowPlus::terminate);

}

void GanttWindowPlus::updateCurrentTime(int time){
    Process::setCurrentTime(time);
}

void GanttWindowPlus::starting() {
    drawer = new GanttChartDrawer(ganttScene);
    connect(drawer, &GanttChartDrawer::newItemAdded, this, &GanttWindowPlus::scrollToRightEdge);
    connect(drawer, &GanttChartDrawer::processTableUpdateRequested, this, &GanttWindowPlus::updateProcessTable);
    connect(drawer, &GanttChartDrawer::currentTimeUpdated, this, &GanttWindowPlus::updateCurrentTime);
    drawer->crt = crt;
    drawer->startFast();
}

void GanttWindowPlus::scrollToRightEdge() {
    QTimer::singleShot(0, this, [this]() {
        if (view && view->horizontalScrollBar()) {
            QScrollBar* hBar = view->horizontalScrollBar();
            hBar->setValue(hBar->maximum());
        }
    });
}

void GanttWindowPlus::updateProcessTable() {
    QStringList headers;
    if(crt->getSchedulerType()=="Priority"){
        processTable->setColumnCount(4);
        headers = {"Name", "Arrival","Burst", "Priority"};
    }
    else{
        processTable->setColumnCount(3);
        headers = {"Name", "Arrival", "Burst"};
    }
    processTable->setHorizontalHeaderLabels(headers);
    processTable->setRowCount(queue.size());

    bool isPriority = (crt->getSchedulerType() == "Priority");
    queue.clear();
    for (auto* p : Process::processes) {
        queue.append(*p);  // dereference to copy the Process object
    }
    for (int i = 0; i < queue.size(); ++i) {
        const Process& p = queue[i];
        processTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(p.getName().empty() ? "P" + std::to_string(p.getProcessNo() + 1) : p.getName())));
        processTable->setItem(i, 1, new QTableWidgetItem(QString::number(p.getArrival())));
        processTable->setItem(i, 2, new QTableWidgetItem(QString::number(p.getBurst())));
        if (isPriority)
            processTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.getPriority())));
    }
}

void GanttWindowPlus::terminate(){
    emit forward();  
}
