#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "drawer.h"

#include <QtCore>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

#include "connectionthread.h"
#include "qcustomplot.h"
#include "drawer.h"
#include "scene2d.h"
#include "socketth.h"

#ifdef _WIN32
#include <Windows.h>
#define PORTNUMBER 2
#else
#include <unistd.h>
#define PORTNUMBER 24
#endif
#include "rs232.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ConnectionThread *connectionTh;
    ConnectionThread *serverTh;
private:
    Ui::MainWindow *ui;
    Scene2d *scene2d;
    Drawer *drawer;
    QMutex mutex;
    QTimer dataTimerFirst;
    QTimer dataTimerSecond;
    QTimer timer2d;
    QTcpServer *server;
    QTcpSocket *socket = NULL;
    int xPanel;
    int yPanel;
    int xMotor;
    int yMotor;

    int sentX;
    int sentY;
    char message[64];

public slots:
    void isConnect();
    void socketConnect();

private slots:
    void realtimeDataSlotFirst();
    void realtimeDataSlotSecond();
    void update2DCoordinates();
    void serverFunction();
    void startServer();
    void on_resetBalance_clicked();
    void on_drawCircle_clicked();
    void on_drawSquare_clicked();
    void on_drawRectangle_clicked();

};

#endif // MAINWINDOW_H
