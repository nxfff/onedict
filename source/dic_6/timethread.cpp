#include "timethread.h"

timethread::timethread(QObject *parent) : QObject(parent)
{
//    timer=new QTimer();cout<<"start";
//    connect(timer,&QTimer::timeout,this,&timethread::secc);
//构造函数内实例化计时器是在本线程内 下面从新线程内无法调用 必须在新线程内实例化
}//
void timethread::addnum(QString str)
{
    //QTimer *timer=new QTimer(this);这行代码应该出现在connect之前 否则会报错 因为编译到connect是 timer对象还没有实例化
    timer=new QTimer();
    connect(timer,&QTimer::timeout,this,&timethread::secc);
    timer->start(1);//毫秒
    cout<<"timer start";
}
void timethread::secc()
{
    s++;
}
void timethread::tEnd(bool x)
{

    //搜索为空时 不会触发startsearch信号 则不会开始timer 但是loadfinish信号会触发停止timer 这句话
    timer->stop();
    cout<<"计时停止";
    emit sendtime(s);
    s=0;
}
void timethread::sqltime(bool emptyflag, QStringList pathlist)
{
    cout<<"搜索用时"<<s;
}
