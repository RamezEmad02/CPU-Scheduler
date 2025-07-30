#include "windows.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

GanttWindow::GanttWindow(controller* crt,QWidget* parent)
    : QWidget(parent),crt(crt) {
    setWindowTitle("Chart");
    resize(800, 500);
    setStyleSheet("background-color: #202020; font-family: 'Segoe UI'; font-size: 14px; color: white;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* timeLayout = new QHBoxLayout;
    QLabel* timeLabel = new QLabel("Current Time:");
    timeField = new QLineEdit;
    timeField->setFixedWidth(60);
    timeField->setReadOnly(true);
    timeField->setStyleSheet(
        "background: transparent;"
        "border: none;"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
    );
    timeLayout->addStretch();
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeField);
    mainLayout->addLayout(timeLayout);

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
        processTable->setColumnCount(6);
        headers = {"Name", "Arrival","Priority", "Burst", "Remaining", "Status"};
    }
    else{
        processTable->setColumnCount(5);
        headers = {"Name", "Arrival", "Burst", "Remaining", "Status"};
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


    QLabel* inputLabel = new QLabel("Input process");
    inputLabel->setAlignment(Qt::AlignCenter);
    inputLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-top: 10px;");
    mainLayout->addWidget(inputLabel);

    QHBoxLayout* inputLayout = new QHBoxLayout;

    nameInput = new QLineEdit;
    arrivalInput = new QLineEdit;
    burstInput = new QLineEdit;
    priorityInput = new QLineEdit;

    QString fieldStyle = "background-color: #125d74; color: white; border: 1px solid #0c3e4e; padding: 4px 6px; border-radius: 4px;";

    nameInput->setStyleSheet(fieldStyle);
    arrivalInput->setStyleSheet(fieldStyle);
    burstInput->setStyleSheet(fieldStyle);
    priorityInput->setStyleSheet(fieldStyle);


    inputLayout->addWidget(new QLabel("Name (optional):"));
    inputLayout->addWidget(nameInput);
    inputLayout->addSpacing(20);

    inputLayout->addWidget(new QLabel("Arrival (> current time):"));
    inputLayout->addWidget(arrivalInput);
    inputLayout->addSpacing(20);

    inputLayout->addWidget(new QLabel("Burst duration (>0):"));
    inputLayout->addWidget(burstInput);
    inputLayout->addSpacing(20);
    if(crt->getSchedulerType()=="Priority"){
        inputLayout->addWidget(new QLabel("Priority (1 highest):"));
        inputLayout->addWidget(priorityInput);
    }
    mainLayout->addLayout(inputLayout);

    addProcess = new QPushButton("Add Process");
    addProcess->setEnabled(false);
    addProcess->setStyleSheet(R"(
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
    mainLayout->addWidget(addProcess, 0, Qt::AlignCenter);
    connect(addProcess, &QPushButton::clicked, this, &GanttWindow::adding);

    start = new QPushButton("Start");
    start->setEnabled(flag);
    start->setStyleSheet(R"(
    QPushButton {
        background-color: #2ea043;
        color: white;
        font-weight: bold;
        border: 1px solid #1f7a31;
        padding: 6px 16px;
        border-radius: 6px;
    }
    QPushButton:hover:enabled {
        background-color: #38c35c;
    }
    QPushButton:disabled {
        background-color: lightgray;
        color: #666;
        border: 1px solid #aaa;
    }
)");

    mainLayout->addWidget(start, 0, Qt::AlignCenter);

    QHBoxLayout* controlLayout = new QHBoxLayout;

    pauseButton = new QPushButton("Pause");
    pauseButton->setStyleSheet(R"(
    QPushButton {
        background-color: #ffc107; /* amber */
        color: black;
        font-weight: bold;
        border: none;
        padding: 10px 20px;
        border-radius: 6px;
    }
    QPushButton:hover {
        background-color: #ffca2c;
    }
    QPushButton:pressed {
        background-color: #e0a800;
    }
)");


    exitButton = new QPushButton("Stop and Show calculations");
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

    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(exitButton);
    mainLayout->addLayout(controlLayout);

    connect(pauseButton, &QPushButton::clicked, this, [=]() {
        if (drawer) {
            if (isPaused) {
                drawer->resume();
                pauseButton->setText("Pause");
                pauseButton->setStyleSheet(R"(
                QPushButton {
                    background-color: #ffc107;
                    color: black;
                    font-weight: bold;
                    border: none;
                    padding: 10px 20px;
                    border-radius: 6px;
                }
                QPushButton:hover {
                    background-color: #ffca2c;
                }
                QPushButton:pressed {
                    background-color: #e0a800;
                }
            )");
            } else {
                drawer->pause();
                pauseButton->setText("Resume");
                pauseButton->setStyleSheet(R"(
                QPushButton {
                    background-color: #17a2b8;
                    color: white;
                    font-weight: bold;
                    border: none;
                    padding: 10px 20px;
                    border-radius: 6px;
                }
                QPushButton:hover {
                    background-color: #1fc8e6;
                }
                QPushButton:pressed {
                    background-color: #117a8b;
                }
            )");
            }
            isPaused = !isPaused;
        }
    });


    connect(exitButton, &QPushButton::clicked, this, &GanttWindow::terminate);
    connect(arrivalInput, &QLineEdit::textChanged, this, &GanttWindow::validateInputs);
    connect(burstInput, &QLineEdit::textChanged, this, &GanttWindow::validateInputs);
    connect(priorityInput, &QLineEdit::textChanged, this, &GanttWindow::validateInputs);
    connect(start, &QPushButton::clicked, this, &GanttWindow::starting);

}

//TODO: write this
void GanttWindow::validateInputs() {
    bool enable;
    enable = !arrivalInput->text().isEmpty();
    enable = !burstInput->text().isEmpty() && enable;

    if(crt->getSchedulerType()=="Priority")
        enable = !priorityInput->text().isEmpty() && enable;

    addProcess->setEnabled(enable);
}

void GanttWindow::starting() {
    if(queue.empty()){
        QMessageBox::warning(this, "Warning", "No process to start with");
    }
    else{
        flag = false;
        start->setEnabled(flag);
        // drawer = new GanttChartDrawer(queue, ganttScene);

        drawer = new GanttChartDrawer(ganttScene);
        connect(drawer, &GanttChartDrawer::newItemAdded, this, &GanttWindow::scrollToRightEdge);
        connect(drawer, &GanttChartDrawer::currentTimeUpdated, this, &GanttWindow::updateCurrentTime);
        connect(drawer, &GanttChartDrawer::processTableUpdateRequested, this, &GanttWindow::updateProcessTable);
        drawer->crt = crt;
        drawer->startDrawing();
    }
}

void GanttWindow::scrollToRightEdge() {
    QTimer::singleShot(0, this, [this]() {
        if (view && view->horizontalScrollBar()) {
            QScrollBar* hBar = view->horizontalScrollBar();
            hBar->setValue(hBar->maximum());
        }
    });
}

void GanttWindow::updateProcessTable() {
    QStringList headers;
    if(crt->getSchedulerType()=="Priority"){
        processTable->setColumnCount(6);
        headers = {"Name", "Arrival","Priority", "Burst", "Remaining", "Status"};
    }
    else{
        processTable->setColumnCount(5);
        headers = {"Name", "Arrival", "Burst", "Remaining", "Status"};
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

        if (isPriority) {
            processTable->setItem(i, 2, new QTableWidgetItem(QString::number(p.getPriority())));
            processTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.getBurst())));
            processTable->setItem(i, 4, new QTableWidgetItem(QString::number(p.getRemainingBurstTime())));
            processTable->setItem(i, 5, new QTableWidgetItem(p.getFinished() ? "Done ✅" : "Not Finished ⏳"));
        } else {
            processTable->setItem(i, 2, new QTableWidgetItem(QString::number(p.getBurst())));
            processTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.getRemainingBurstTime())));
            processTable->setItem(i, 4, new QTableWidgetItem(p.getFinished() ? "Done ✅" : "Not Finished ⏳"));
        }
    }
}

void GanttWindow::adding() {
    bool arrivalOk, burstOk, priorityOk;

    QString arrivalText = arrivalInput->text();
    QString burstText = burstInput->text();
    QString priorityText = priorityInput->text();

    int arrival = arrivalText.toInt(&arrivalOk);
    int burst = burstText.toInt(&burstOk);
    int priority;


    if (!arrivalOk || arrivalText.contains('.')) {
        QMessageBox::warning(this, "Invalid Input", "Arrival time must be a whole number with no letters or decimal points.");
        return;
    }
    if (arrival <= Process::getCurrentTime()) {
        QMessageBox::warning(this, "Invalid Input", "Arrival time must be greater than current time.");
        return;
    }

    if (!burstOk || burstText.contains('.')) {
        QMessageBox::warning(this, "Invalid Input", "Burst time must be a whole number with no letters or decimal points.");
        return;
    }
    if (burst <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Burst time must be greater than 0.");
        return;
    }
    if(crt->getSchedulerType()=="Priority"){
        priority = priorityText.toInt(&priorityOk);

        if (!priorityOk || priorityText.contains('.')) {
            QMessageBox::warning(this, "Invalid Input", "Priority must be a whole number with no letters or decimal points.");
            return;
        }
        if (priority < 1) {
            QMessageBox::warning(this, "Invalid Input", "Priority must be 1 or more.");
            return;
        }
    }

    // Process name (optional)
    QString name = nameInput->text().isEmpty()
                       ? "P" + QString::number(Process::processNumber + 1)
                       : nameInput->text();
    if(crt->getSchedulerType()=="Priority")
        new /*Temp*/Process(arrival, burst, name.toStdString(), priority);
    else
        new /*Temp*/Process(arrival, burst, name.toStdString());

    // if(Process::getCurrentTime()==0){
    //     for (TempProcess* p : TempProcess::container) {
    //         //
    //         if (crt->getSchedulerType()== "Priority")
    //             new Process(p->arrivalTime, p->burstTime, p->name, p->priority);
    //         else
    //             new Process(p->arrivalTime, p->burstTime, p->name);
    //     }
    //     TempProcess::container.clear();
    //
    // }
    // Clear input fields

    updateProcessTable();
    updateProcessTable();

    nameInput->clear();
    arrivalInput->clear();
    burstInput->clear();
    priorityInput->clear();
}


void GanttWindow::updateCurrentTime(int time){
    Process::setCurrentTime(time);
    timeField->setText(QString::number(time));
}

void GanttWindow::terminate(){
    if(drawer)drawer->pause();
    emit forward();
    // CalculationsWindow *calculationsWindow = crt->getCalcWindow();
    // calculationsWindow->setDrawer(drawer);
    // calculationsWindow->setProcessTable(processTable);
}
