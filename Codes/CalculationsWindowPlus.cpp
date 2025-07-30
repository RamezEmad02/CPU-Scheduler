#include "windows.h"

CalculationsWindowPlus::CalculationsWindowPlus(controller* crt,QWidget* parent)
    : QWidget(parent), crt(crt){

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
    QStringList header = {"Name", "Arrival", "Burst", "Remaining", "Status"};;
    if(crt->getSchedulerType()=="Priority"){
        processTable->setColumnCount(6);
        header = {"Name", "Arrival","Priority", "Burst", "Remaining", "Status"};
    }
    else{
        processTable->setColumnCount(5);
        header = {"Name", "Arrival", "Burst", "Remaining", "Status"};
    }
    processTable->setHorizontalHeaderLabels(header);
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


    QLabel* inputLabel = new QLabel("Average calculations");
    inputLabel->setAlignment(Qt::AlignCenter);
    inputLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-top: 10px;");
    mainLayout->addWidget(inputLabel);

    QFont labelFont;
    labelFont.setPointSize(11);

    QHBoxLayout* turnaround = new QHBoxLayout;
    QLabel* turnaroundText = new QLabel("Average Turnaround time:");
    turnaroundText->setFont(labelFont);
    turnaroundValue = new QLabel(QString::number(avgTurnaround));
    turnaroundValue->setFixedSize(100, 40);
    turnaroundValue->setStyleSheet(R"(
        background-color: #176D8C;
        color: white;
        border-radius: 20px;
        font-weight: bold;
        font-size: 18px;
        padding: 6px 12px;
    )");
    turnaroundValue->setAlignment(Qt::AlignCenter);

    turnaround->addStretch();
    turnaround->addWidget(turnaroundText);
    turnaround->addSpacing(10);
    turnaround->addWidget(turnaroundValue);
    turnaround->addStretch();
    mainLayout->addLayout(turnaround);

    QHBoxLayout* wait = new QHBoxLayout;
    QLabel* waitText = new QLabel("Average Waiting time:");
    waitText->setFont(labelFont);
    waitValue = new QLabel(QString::number(avgWaiting));
    waitValue->setFixedSize(100, 40);
    waitValue->setStyleSheet(R"(
        background-color: #176D8C;
        color: white;
        border-radius: 20px;
        font-weight: bold;
        font-size: 18px;
        padding: 6px 12px;
    )");
    waitValue->setAlignment(Qt::AlignCenter);


    wait->addStretch();
    wait->addWidget(waitText);
    wait->addSpacing(10);
    wait->addWidget(waitValue);
    wait->addStretch();
    mainLayout->addLayout(wait);

    QHBoxLayout* response = new QHBoxLayout;
    QLabel* responseText = new QLabel("Average Response time:");
    responseText->setFont(labelFont);
    responseValue = new QLabel(QString::number(avgResponse));
    responseValue->setFixedSize(100, 40);
    responseValue->setStyleSheet(R"(
        background-color: #176D8C;
        color: white;
        border-radius: 20px;
        font-weight: bold;
        font-size: 18px;
        padding: 6px 12px;
    )");
    responseValue->setAlignment(Qt::AlignCenter);

    response->addStretch();
    response->addWidget(responseText);
    response->addSpacing(10);
    response->addWidget(responseValue);
    response->addStretch();
    mainLayout->addLayout(response);

    mainLayout->addSpacing(60);

    backButton = new QPushButton("Back");

    backButton->setStyleSheet(R"(
    QPushButton {
        background-color: #007bff;
        color: white;
        font-weight: bold;
        border: none;
        padding: 6px 16px;
        border-radius: 6px;
    }
    QPushButton:hover:enabled {
        background-color: #3399ff;
    }
    QPushButton:disabled {
        background-color: lightgray;
        color: #666;
    }
)");
    mainLayout->addWidget(backButton, 0, Qt::AlignCenter);

    connect(backButton, &QPushButton::clicked, this, &CalculationsWindowPlus::close);
}

void CalculationsWindowPlus::setDrawer(QGraphicsScene* sharedScene){
    ganttScene = sharedScene;
    view->setScene(ganttScene);  // Replace the scene in this window
    // view->fitInView(ganttScene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->update();
}

void CalculationsWindowPlus::setProcessTable() {
    if(!Process::processes.empty()){
        QStringList headers;
        std::vector<Process*> temp = Process::processes;
        if(crt->getSchedulerType()=="Priority"){
            processTable->setColumnCount(9);
            headers = {"Name", "Arrival","Priority", "Burst", "Remaining", "Status","Turnaround","Waiting","Response"};
        }
        else{
            processTable->setColumnCount(8);
            headers = {"Name", "Arrival", "Burst", "Remaining", "Status","Turnaround","Waiting","Response"};
        }
        processTable->setHorizontalHeaderLabels(headers);
        processTable->setRowCount(temp.size());

        bool isPriority = (crt->getSchedulerType() == "Priority");

        for (int i = 0; i < temp.size(); ++i) {
            const Process* p = temp[i];
            processTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(p->getName().empty() ? "P" + std::to_string(p->getProcessNo() + 1) : p->getName())));
            processTable->setItem(i, 1, new QTableWidgetItem(QString::number(p->getArrival())));

            if (isPriority) {
                processTable->setItem(i, 2, new QTableWidgetItem(QString::number(p->getPriority())));
                processTable->setItem(i, 3, new QTableWidgetItem(QString::number(p->getBurst())));
                processTable->setItem(i, 4, new QTableWidgetItem(QString::number(p->getRemainingBurstTime())));
                processTable->setItem(i, 5, new QTableWidgetItem(p->getFinished() ? "Done ✅" : "Running ⏳"));
                processTable->setItem(i, 6, new QTableWidgetItem(QString::number(p->getTurnaroundTime())));
                processTable->setItem(i, 7, new QTableWidgetItem(QString::number(p->getWaitingTime())));
                processTable->setItem(i, 8, new QTableWidgetItem(QString::number(p->getStartTime() - p->getArrival())));
            } else {
                processTable->setItem(i, 2, new QTableWidgetItem(QString::number(p->getBurst())));
                processTable->setItem(i, 3, new QTableWidgetItem(QString::number(p->getRemainingBurstTime())));
                processTable->setItem(i, 4, new QTableWidgetItem(p->getFinished() ? "Done ✅" : "Running ⏳"));
                processTable->setItem(i, 5, new QTableWidgetItem(QString::number(p->getTurnaroundTime())));
                processTable->setItem(i, 6, new QTableWidgetItem(QString::number(p->getWaitingTime())));
                processTable->setItem(i, 7, new QTableWidgetItem(QString::number(p->getStartTime() - p->getArrival())));
            }
        }
    }
}

void CalculationsWindowPlus::calculate(){
    std::vector<Process*> temp = Process::processes;

    float totalWait = 0;
    float totalTurn = 0;
    float totalResponse = 0;

    int size = temp.size();
    int sizeResponse = size;

    bool notFini = false , notStart = false;
    bool noProcessStarted = true , noProcessFinished = true;



    for(auto p: temp){
        if(p->getStartTime()<0) sizeResponse--;
        else {
            noProcessStarted = false;
            totalResponse += p->getStartTime() - p->getArrival();
        }

        if(p->getFinished()){
            noProcessFinished = false;
            totalTurn += p->getTurnaroundTime();
            totalWait += p->getWaitingTime();
        }
        else size -= 1;
    }

    if(noProcessFinished){
        avgTurnaround = -1;
        avgWaiting = -1;
        notFini = true;
        QMessageBox::warning(crt,"Warning","No process finished its burst");
    }
    else{
        if(size < temp.size()) QMessageBox::information(crt,"calculation","not finished processes aren't included in turnaround and waiting calculations");
        avgTurnaround = totalTurn / size ;
        avgWaiting = totalWait / size ;
    }

    if(noProcessStarted){
        avgResponse = -1;
        notStart = true;
        QMessageBox::warning(crt,"Warning","No process has started");
    }
    else{
        if(sizeResponse < temp.size())QMessageBox::information(crt,"calculation","processes that didn't start aren't included in response calculations");
        avgResponse = totalResponse / sizeResponse;
    }
    turnaroundValue->setText(QString::number(avgTurnaround));
    waitValue->setText(QString::number(avgWaiting));
    responseValue->setText(QString::number(avgResponse));
}

void CalculationsWindowPlus::close(){
    emit back();
}
