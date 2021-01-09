#include "connectionthread.h"

ConnectionThread::ConnectionThread(QObject *parent):
     QThread(parent)
{

}

void ConnectionThread::run(){

    emit startConnection();
}
