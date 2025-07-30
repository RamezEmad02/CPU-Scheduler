#include "windows.h"


SchedulerWindow::SchedulerWindow(controller* crt,QWidget *parent) : QWidget(parent),crt(crt) {
    setWindowTitle("Scheduler");
    resize(1000, 800);
    setAttribute(Qt::WA_StyledBackground, true);
    // setStyleSheet("QWidget { background-color: lightgrey; }");

    QLabel *title = new QLabel("Choose your scheduler");
    title->setStyleSheet("color: #FFFFFF;");
    QFont font = title->font();
    font.setPointSize(40);
    font.setBold(true);

    title->setFont(font);
    title->setAlignment(Qt::AlignCenter);

    QVBoxLayout *leftColumn = new QVBoxLayout;
    leftColumn->addLayout(createGifButtonBlock("FCFS.gif", "FCFS", SLOT(onButton1Clicked())));
    leftColumn->addSpacing(40);
    leftColumn->addLayout(createGifButtonBlock("RR.gif", "Round Robin", SLOT(onButton3Clicked())));

    QVBoxLayout *rightColumn = new QVBoxLayout;
    rightColumn->addLayout(createGifButtonBlock("SJF.gif", "SJF", SLOT(onButton2Clicked())));
    rightColumn->addSpacing(40);
    rightColumn->addLayout(createGifButtonBlock("Priority.gif", "Priority", SLOT(onButton4Clicked())));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->addLayout(leftColumn);
    mainLayout->addSpacing(300);
    mainLayout->addLayout(rightColumn);
    mainLayout->addStretch();

    QVBoxLayout *finalLayout = new QVBoxLayout;
    finalLayout->addSpacing(20);
    finalLayout->addWidget(title);
    finalLayout->addSpacing(60);
    finalLayout->addLayout(mainLayout);
    finalLayout->addStretch();
    this->setLayout(finalLayout);
}

QVBoxLayout* SchedulerWindow::createGifButtonBlock(const QString &gifPath, const QString &buttonText, const char* slotMethod) {
    QLabel *gifLabel = new QLabel;
    gifLabel->setFixedSize(300, 200);
    gifLabel->setScaledContents(true);
    QMovie *movie = new QMovie(gifPath);
    gifLabel->setMovie(movie);
    movie->start();

    QPushButton *button = new QPushButton(buttonText);
    button->setFixedSize(140, 40);
    button->setStyleSheet(R"(
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

    connect(button, SIGNAL(clicked()), this, slotMethod);

    QVBoxLayout *layout = new QVBoxLayout;

    QWidget *container = new QWidget;
    QVBoxLayout *innerLayout = new QVBoxLayout(container);
    innerLayout->setAlignment(Qt::AlignHCenter);
    innerLayout->addWidget(gifLabel, 0, Qt::AlignHCenter);
    innerLayout->addSpacing(10);
    innerLayout->addWidget(button, 0, Qt::AlignHCenter);

    // Add the container to the outer layout
    layout->addWidget(container, 0, Qt::AlignCenter);

    return layout;
}

QSize SchedulerWindow::sizeHint() const {
    return QSize(1000, 800);
}

void SchedulerWindow::onButton1Clicked() {
    crt->setSchedulerType("FCFS");
    emit input();
}

void SchedulerWindow::onButton2Clicked() {
    crt->setSchedulerType("SJF");
    emit type();
}

void SchedulerWindow::onButton3Clicked() {
    crt->setSchedulerType("Round Robin");
    emit input();
}

void SchedulerWindow::onButton4Clicked() {
    crt->setSchedulerType("Priority");
    emit type();
}
