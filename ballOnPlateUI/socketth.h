#ifndef SOCKETTH_H
#define SOCKETTH_H

#include <QThread>
#include <QtCore>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    explicit SocketThread(QObject *parent = 0);
    void run();
signals:
    void startSocket();
public slots:

};

#endif // SOCKETTH_H
