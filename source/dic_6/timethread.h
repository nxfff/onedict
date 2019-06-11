#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
class timethread : public QObject
{
    Q_OBJECT
public:
    explicit timethread(QObject *parent = nullptr);
    void addnum(QString str);
    void secc();
    void tEnd(bool x);
    void sqltime(bool emptyflag,QStringList pathlist);
    QTimer *timer;
    int s=0;
signals:
    void sendtime(int x);
public slots:
};

#endif // TIMETHREAD_H
