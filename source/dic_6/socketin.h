#ifndef SOCKETIN_H
#define SOCKETIN_H

#include <QObject>
#include "serversocket.h"
#include <QWebSocketServer>
#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
class socketin : public QObject
{
    Q_OBJECT
public:
    explicit socketin(QWebSocketServer *server,QObject *parent = nullptr);
signals:
    void clientConnected(serversocket *client);
    void skconnect(int x);
private slots:
    void handleNewConnection();
private:
    QWebSocketServer *sserver;
};

#endif // SOCKETIN_H
