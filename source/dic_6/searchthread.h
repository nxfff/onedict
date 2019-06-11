#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QObject>
#include <QSqlQuery>
#include <QFile>
#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
class searchthread : public QObject
{
    Q_OBJECT
public:
    explicit searchthread(QObject *parent = nullptr);
    void dealstr(QString str);
    void searchinwdg(QString text,QString texthead);
public:
    QStringList pathList;
    //QStringList strlist;
    //bool emptyflag=1;
    QList<int> pagenum={0,0,0,0,0,0};
signals:
    void searchfinish(bool emptyflag,QStringList pathlist);
    void sendset(QStringList slist);
private:


public slots:
};

#endif // SEARCHTHREAD_H
