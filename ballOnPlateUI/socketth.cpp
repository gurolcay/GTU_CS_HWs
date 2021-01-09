#include "socketth.h"

SocketThread::SocketThread(QObject *parent):
    QThread(parent)
{

}

void SocketThread::run(){

    emit startSocket();
}
