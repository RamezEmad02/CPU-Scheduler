#include "windows.h"

typeWindow::typeWindow(controller* crt,QWidget *parent) : QWidget(parent),crt(crt) {
    setWindowTitle("Choose your scheduler");
    resize(1000, 800);
    // setAttribute(Qt::WA_StyledBackground, true);
    // setStyleSheet("QWidget { background-color: lightgrey; }");

    QLabel *title = new QLabel("Choose your scheduler");
    title->setStyleSheet("color: #FFFFFF;");
    QFont font = title->font();
    font.setPointSize(40);
    font.setBold(true);
    title->setFont(font);
    title->setAlignment(Qt::AlignCenter);

    QVBoxLayout *leftColumn = new QVBoxLayout;
    leftColumn->addLayout(createGifButtonBlock("PP.gif", "Preemptive", SLOT(onButton1Clicked())));

    QVBoxLayout *rightColumn = new QVBoxLayout;
    rightColumn->addLayout(createGifButtonBlock("NPP.gif", "Non-Preemptive", SLOT(onButton2Clicked())));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->addLayout(leftColumn);
    mainLayout->addSpacing(300);
    mainLayout->addLayout(rightColumn);
    mainLayout->addStretch();

    QPushButton *backButton = new QPushButton("Back");
    backButton->setFixedSize(140, 40);
    backButton->setStyleSheet(R"(
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

    // Create a layout for the Back button that centers it horizontally
    QHBoxLayout *backButtonLayout = new QHBoxLayout;
    backButtonLayout->addStretch();  // Adds space before the button
    backButtonLayout->addWidget(backButton);  // Adds the back button itself
    backButtonLayout->addStretch();  // Adds space after the button to center it
    connect(backButton, &QPushButton::clicked, this, &typeWindow::goBack);

    QVBoxLayout *finalLayout = new QVBoxLayout(this);
    finalLayout->addSpacing(20);
    finalLayout->addWidget(title);
    finalLayout->addSpacing(20);
    finalLayout->addLayout(mainLayout);
    finalLayout->addStretch();  // Takes remaining space
    finalLayout->addLayout(backButtonLayout);  // Add centered Back button at the bottom
    setLayout(finalLayout);
}

QVBoxLayout* typeWindow::createGifButtonBlock(const QString &gifPath, const QString &buttonText, const char* slotMethod) {
    QLabel *gifLabel = new QLabel;
    gifLabel->setFixedSize(300, 500);
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
    return layout;
}

void typeWindow::goBack(){
    emit back();
}

void typeWindow::onButton1Clicked() {
    crt->setIsPreemptive(true);
    emit forward();
}

void typeWindow::onButton2Clicked() {
    crt->setIsPreemptive(false);
    emit forward();
}
