#ifndef BRIDGE_H
#define BRIDGE_H
#include "mywindow.h"
#include <QObject>

#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
class bridge : public QObject
{
    Q_OBJECT

public:
    bridge(mywindow *window, QObject *parent = nullptr)
        : QObject(parent), wwindowt(window)
    {
        //connect(window, &mywindow::sendText, this, &bridge::sendtext);信号触发信号 服务器发送功能废弃

    }
signals:
    //void sendtext(const QString &text);//发送信号废弃

public slots:
    void receiveText(const QString &text)
    {
        cout<<"server receive function in bridge";
        wwindowt->getMessage(text);
    }
    void dealselect(const QString &selectedstr)//一定要const+& 才能接收到字符串
    {
        wwindowt->dealselectstr(selectedstr);
    }

private:
    mywindow *wwindowt;
};

#endif // BRIDGE_H
