#include "socketin.h"

socketin::socketin(QWebSocketServer *server, QObject *parent) : QObject(parent),sserver(server)//算是一个初始化 sserver=server
{
    cout<<sserver->serverPort();
    connect(server, &QWebSocketServer::newConnection,this, &socketin::handleNewConnection);
    //有新连接时 触发handleNewConnection函数
}

void socketin::handleNewConnection()
{

    emit skconnect(2);//发送信号  修改任务栏状态
    emit clientConnected(new serversocket(sserver->nextPendingConnection()));cout<<"连接成功";
    //m_server->nextPendingConnection()可理解为从新连接中获取对方套接字的信息 这是一个套接字对象
}
