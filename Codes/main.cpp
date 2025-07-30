#include <QApplication>
#include "windows.h"

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//     SchedulerWindow window;
//     window.show();
//     return app.exec();
// }

/*=================================================================================================================*/
/*=================================================================================================================*/


// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     QVector<Process> queue = {
//         Process(1, 3,"hiiii"),
//         // Process( 0, 4,"P2"),
//         Process( 5, 4),
//         Process( 13, 1)
//     };


//     QGraphicsScene* scene = new QGraphicsScene();
//     QGraphicsView* view = new QGraphicsView(scene);
//     view->setWindowTitle("Real-Time Gantt Chart");
//     view->resize(800, 200);
//     view->show();

//     GanttChartDrawer* drawer = new GanttChartDrawer(queue, scene);
//     drawer->startDrawing();

//     return app.exec();
// }

/*=================================================================================================================*/
/*=================================================================================================================*/
// #include <QApplication>
// #include "windows.h"

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     ProcessInputWindow window;
//     window.resize(500, 600);
//     window.show();

//     return app.exec();
// }

/*=================================================================================================================*/
/*=================================================================================================================*/
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet(R"(
        QMessageBox QLabel {
            color: white;
            font-size: 14px;
        }
    )");
    controller window;
    window.show();
    return app.exec();
}

