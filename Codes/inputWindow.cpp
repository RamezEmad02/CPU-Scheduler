#include "windows.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPalette>
#include <QDebug>

ProcessInputWindow::ProcessInputWindow(controller* crt,QWidget* parent) : QWidget(parent),crt(crt) {
    setWindowTitle("Process Input");
    // setAttribute(Qt::WA_StyledBackground, true);
    // setStyleSheet("QWidget { background-color: black; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Input processes");
    titleLabel->setStyleSheet("color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    QFormLayout* formLayout = new QFormLayout;
    QString fieldStyle = "background-color: #125d74; color: white; border: 1px solid #0c3e4e; padding: 4px 6px; border-radius: 4px;";

    nameInput = new QLineEdit;
    arrivalInput = new QLineEdit;
    burstInput = new QLineEdit;
    idleStartInput = new QLineEdit;
    //depends on type

    priorityinput = new QLineEdit;
    quantumInput = new QLineEdit;

    nameInput->setStyleSheet(fieldStyle);
    arrivalInput->setStyleSheet(fieldStyle);
    burstInput->setStyleSheet(fieldStyle);
    priorityinput->setStyleSheet(fieldStyle);
    quantumInput->setStyleSheet(fieldStyle);

    QLabel* name = new QLabel("Name (optional):");
    name->setStyleSheet("color: white;");
    QLabel* arr = new QLabel("Arrival (>=0):");
    arr->setStyleSheet("color: white;");
    QLabel* bur = new QLabel("Burst duration (>0):");
    bur->setStyleSheet("color: white;");
    QLabel* pr = new QLabel("Priority (1 highest):");
    pr->setStyleSheet("color: white;");
    QLabel* quantum = new QLabel("Quantum time (>= 1):");
    quantum->setStyleSheet("color: white;");

    formLayout->addRow(name, nameInput);
    formLayout->addRow(arr, arrivalInput);
    formLayout->addRow(bur , burstInput);
    if(crt->getSchedulerType()=="Priority")
        formLayout->addRow(pr, priorityinput);
    if(crt->getSchedulerType()=="Round Robin")
        formLayout->addRow(quantum, quantumInput);
    mainLayout->addLayout(formLayout);

    enterButton = new QPushButton("Enter");
    enterButton->setFixedSize(140, 40);
    enterButton->setStyleSheet(R"(
        QPushButton {
            background-color: #19647E;
            color: white;
            border-radius: 8px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #28AFB0;
        }
    )");

    enterButton->setEnabled(false);
    mainLayout->addWidget(enterButton, 0, Qt::AlignCenter);

    // add idle feature
    // QFormLayout* idleLayout = new QFormLayout;
    // idleLayout->addRow("Processor starts at idle for (default = 0):", idleStartInput);
    // mainLayout->addLayout(idleLayout);

    QLabel* enteredLabel = new QLabel("Processes Entered:");
    enteredLabel->setStyleSheet("color: white;");
    enteredLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(enteredLabel);
    mainLayout->setSpacing(10);

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

    startSimButton = new QPushButton("Start Simulation");
    back = new QPushButton("Back");
    hoba = new QPushButton("No Live");

    startSimButton->setFixedSize(140, 40);
    startSimButton->setStyleSheet(R"(
        QPushButton {
            background-color: #19647E;
            color: white;
            border-radius: 8px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #28AFB0;
        }
    )");
    back->setFixedSize(140, 40);
    back->setStyleSheet(R"(
        QPushButton {
            background-color: #19647E;
            color: white;
            border-radius: 8px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #28AFB0;
        }
    )");
    hoba->setFixedSize(140, 40);
    hoba->setStyleSheet(R"(
        QPushButton {
            background-color: #19647E;
            color: white;
            border-radius: 8px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #28AFB0;
        }
    )");
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addStretch();
    lay->addWidget(back,0, Qt::AlignHCenter);
    lay->addSpacing(30);
    lay->addWidget(hoba,0, Qt::AlignHCenter);
    lay->addSpacing(30);
    lay->addWidget(startSimButton,0, Qt::AlignHCenter);
    lay->addStretch();

    mainLayout->addLayout(lay);

    connect(arrivalInput, &QLineEdit::textChanged, this, &ProcessInputWindow::validateInputs);
    connect(burstInput, &QLineEdit::textChanged, this, &ProcessInputWindow::validateInputs);
    connect(priorityinput, &QLineEdit::textChanged, this, &ProcessInputWindow::validateInputs);
    connect(quantumInput, &QLineEdit::textChanged, this, &ProcessInputWindow::validateInputs);
    connect(enterButton, &QPushButton::clicked, this, &ProcessInputWindow::addProcess);
    connect(startSimButton, &QPushButton::clicked, this, &ProcessInputWindow::startSimulation);
    connect(back, &QPushButton::clicked, this, &ProcessInputWindow::backward);
    connect(hoba, &QPushButton::clicked, this, &ProcessInputWindow::fast);
}

void ProcessInputWindow::validateInputs() {
    bool enable;
    enable = !arrivalInput->text().isEmpty();
    enable = !burstInput->text().isEmpty() && enable;

    if(crt->getSchedulerType()=="Priority")
        enable = !priorityinput->text().isEmpty() && enable;

    if(crt->getSchedulerType()=="Round Robin")
        enable = !quantumInput->text().isEmpty() && enable;

    enterButton->setEnabled(enable);
}

void ProcessInputWindow::addProcess() {
    bool arrivalOk, burstOk, priorityOk, quantumOk;

    QString arrivalText = arrivalInput->text();
    QString burstText = burstInput->text();
    QString priorityText = priorityinput->text();
    QString quantumText = quantumInput->text();

    int arrival = arrivalText.toInt(&arrivalOk);
    int burst = burstText.toInt(&burstOk);
    int priority;
    int quantum;


    if (!arrivalOk || arrivalText.contains('.')) {
        QMessageBox::warning(this, "Invalid Input", "Arrival time must be a whole number with no letters or decimal points.");
        return;
    }
    if (arrival < 0) {
        QMessageBox::warning(this, "Invalid Input", "Arrival time must be 0 or more.");
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
    if(crt->getSchedulerType()=="Round Robin"){
        quantum = quantumText.toInt(&quantumOk);

        if (!quantumOk || quantumText.contains('.')) {
            QMessageBox::warning(this, "Invalid Input", "Quantum must be a whole number with no letters or decimal points.");
            return;
        }
        if (quantum < 1) {
            QMessageBox::warning(this, "Invalid Input", "Quantum must be 1 or more.");
            return;
        }
    }
    if(crt->getSchedulerType()=="Round Robin")
        Process::quantum = quantumInput->text().toInt();

    QString name = nameInput->text().isEmpty()
        ? "P" + QString::number(processTable->rowCount() + 1)
        : nameInput->text();

        int row = processTable->rowCount();
        processTable->insertRow(row);
        processTable->setItem(row, 0, new QTableWidgetItem(name));
        processTable->setItem(row, 1, new QTableWidgetItem(arrivalText));
        processTable->setItem(row, 2, new QTableWidgetItem(burstText));
        processTable->setItem(row, 3, new QTableWidgetItem(priorityText));

        nameInput->clear();
        arrivalInput->clear();
        burstInput->clear();
        priorityinput->clear();
}


void ProcessInputWindow::startSimulation() {
    extractProcessesFromTable();
    emit front();
}

void ProcessInputWindow::extractProcessesFromTable() const {
    bool flag = crt->getSchedulerType() == "Priority";
    for (int row = 0; row < processTable->rowCount(); ++row) {
        QString qName = processTable->item(row, 0)->text();
        int arrival = processTable->item(row, 1)->text().toInt();
        int burst = processTable->item(row, 2)->text().toInt();

        std::string name = qName.toStdString();
        if(flag){
            int priority = processTable->item(row, 3)->text().toInt();
            new Process(arrival, burst, name, priority);
        }
        else
            new Process(arrival, burst, name);
    }
}

void ProcessInputWindow::backward(){
    emit goBack();
}
void ProcessInputWindow::fast(){
    extractProcessesFromTable();
    emit backFast();
}
