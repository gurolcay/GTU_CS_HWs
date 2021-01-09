
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //Kaan Ucar
    drawer = new Drawer(this);
    ui->shapeDrawer->setScene(drawer);
    ui->shapeDrawer->setRenderHint(QPainter::Antialiasing);

    scene2d = new Scene2d(this);
    ui->xyCoordinates->setScene(scene2d);
    ui->xyCoordinates->setRenderHint(QPainter::Antialiasing);
    //Kaan Ucar

    connectionTh = new ConnectionThread(this);
    connect(connectionTh, SIGNAL(startConnection()), this, SLOT(isConnect()), Qt::DirectConnection);


    //Mutlu Polatcan
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    ui->customPlotSecond->addGraph(); // blue line
    ui->customPlotSecond->addGraph(); // red line
    ui->customPlotSecond->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlotSecond->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer <QCPAxisTickerTime> timeTickerFirst(new QCPAxisTickerTime);
    QSharedPointer <QCPAxisTickerTime> timeTickerSecond(new QCPAxisTickerTime);
    timeTickerFirst->setTimeFormat("%h:%m:%s");
    timeTickerSecond->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTickerFirst);
    ui->customPlotSecond->xAxis->setTicker(timeTickerSecond);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlotSecond->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(0, 1023);
    ui->customPlotSecond->yAxis->setRange(0, 800);

    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlotSecond->xAxis2,
            SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlotSecond->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlotSecond->yAxis2,
            SLOT(setRange(QCPRange)));

    dataTimerFirst.start();
    dataTimerSecond.start();
    timer2d.start();
    timer2d.setInterval(17);

    server = new QTcpServer();
    if(!server->listen(QHostAddress::Any,1234))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started" ;
    }

    connect(&dataTimerFirst, SIGNAL(timeout()), this, SLOT(realtimeDataSlotFirst()));
    connect(&dataTimerSecond, SIGNAL(timeout()), this, SLOT(realtimeDataSlotSecond()));
    connect(&timer2d, SIGNAL(timeout()), this, SLOT(update2DCoordinates()));
    serverTh = new ConnectionThread(this);
    connect(serverTh, SIGNAL(startConnection()), this, SLOT(startServer()), Qt::DirectConnection);


    sentX= 486;
    sentY= 523;
    sprintf(message, "%4d,%4d",sentX,sentY);
    connectionTh->start();
    serverTh->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::startServer(){
    qDebug("sadasd");
    while(socket == NULL){
        if(server->hasPendingConnections()){
            qDebug("has connection");
            QTcpSocket *socket = server->nextPendingConnection();
            while(socket != NULL){
            char str[64];
                        memset(str,'\0',64);
                        mutex.lock();
                        sprintf(str,"%d,%d,%d,%d",xPanel,yPanel,xMotor,yMotor);
                        mutex.unlock();
                        socket->write(str, 64);
                        socket->flush();
                        socket->waitForBytesWritten(17);
                        qDebug("server falan");
             }
        }
    }
}

void MainWindow::serverFunction(){
    if(socket != NULL){

    }

}

void MainWindow::update2DCoordinates(){
    scene2d->draw();
    drawer->draw();
}

void MainWindow::socketConnect(){

    QTcpServer *server = new QTcpServer();
    char str[64];

    connect(server,SIGNAL(socketConnect()),this,SLOT(socketConnect()));

    if(!server->listen(QHostAddress::Any,1234))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started" ;
    }

    QTcpSocket *socket = server->nextPendingConnection();

    while (1)
    {
        sprintf(str,"%4d,%4d,%4d,%4d\r\n",xPanel,yPanel,xMotor,yMotor);
        qDebug(str);

        socket->write("str");
        socket->flush();

        socket->waitForBytesWritten(3000);
    }
}

void MainWindow::isConnect() {


    int i = 0,
            n,
            cport_nr = PORTNUMBER,        /* /dev/ttyS0 (COM1 on windows) */
            bdrate = 115200;       /* 115200 baud */

    char mode[] = {'8', 'N', '1', 0},
            str[64];

    unsigned char buf[2];

    if (RS232_OpenComport(cport_nr, bdrate, mode)) {
        //connection yoksa bi şey yap.
        qDebug("Can not open comport\n");

    }

    while (1) {
        while (1) {
            n = RS232_PollComport(cport_nr, buf, 1);

            if (n > 0) {
                buf[n] = 0;
                str[i] = buf[0];
                ++i;

                if (strcmp((char *) buf, "-") == 0) {

                    str[i - 1] = 0;
                    i = 0;


                    mutex.lock();
                    xPanel = atoi(strtok(str, ","));
                    yPanel = atoi(strtok(NULL, ","));
                    xMotor = atoi(strtok(NULL, ","));

                    char *temp = strtok(NULL, ",");
                    yMotor = atoi(temp);
                    scene2d->setPanel(xPanel, yPanel);
                    drawer->setPanel(xPanel, yPanel);

                    mutex.unlock();

                    //qDebug("xPanel: %d,\t yPanel: %d,\t xMotor:%d,\t yMotor:%d", xPanel, yPanel, xMotor, yMotor);
                    //qDebug("CONNECT xMotor:%d,\t yMotor:%d", xMotor, yMotor);
                    break;
                }
            }
            /*else{
                //qDebug("hello mutlu");
                scene2d->setBallOnPlate(false);
                drawer->setBallOnPlate(false);
            }*/
        }

        //arduinodan gelen veriler strde

        if(drawer->isPathReady()){
            char str[64];
            QList<QPoint> list = drawer->getPathList();
            sprintf(str,"array-%d-",list.size());
            RS232_cputs(cport_nr, str);
            for(int i=0;i<list.size();++i)
                sprintf(str,"%d,%d-",list.at(i).x(),list.at(i).y());
        }
        else
            RS232_cputs(cport_nr, message);
    }
}


void MainWindow::realtimeDataSlotFirst() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.5) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlot->graph(0)->addData(key, xPanel);
        //    ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
        // rescale value (vertical) axis to fit the current data:
        //ui->customPlot->graph(0)->rescaleValueAxis();
        //ui->customPlot->graph(1)->rescaleValueAxis(true);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key - lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
                        .arg(ui->customPlot->graph(0)->data()->size()/*ui->customPlot->graph(1)->data()->size()*/), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

void MainWindow::realtimeDataSlotSecond() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.5) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlotSecond->graph(0)->addData(key, yPanel);
        //    ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
        // rescale value (vertical) axis to fit the current data:
        //ui->customPlot->graph(0)->rescaleValueAxis();
        //ui->customPlot->graph(1)->rescaleValueAxis(true);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlotSecond->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlotSecond->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key - lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
                        .arg(ui->customPlotSecond->graph(
                                0)->data()->size()/*ui->customPlot->graph(1)->data()->size()*/), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}


void MainWindow::on_resetBalance_clicked()
{
    strcpy(message,"reset");
}

void MainWindow::on_drawCircle_clicked()
{
    strcpy(message,"circle");
}

void MainWindow::on_drawSquare_clicked()
{
    strcpy(message,"square");
}

void MainWindow::on_drawRectangle_clicked()
{
    strcpy(message,"rect");
}
