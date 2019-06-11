#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H
#include <QWidget>
#include <QLineEdit>
#include <QSqlQuery>
#include <QStringListModel>
#include <QListView>
#include "docxml.h"
#include "webpage.h"
#include "searchthread.h"
#include <QThread>
#include <QKeyEvent>
#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
#include <QMetaType>
class searchwidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit searchwidget(QWidget *parent = nullptr);
    ~searchwidget();
    void letchanged();
    void judgesame(QString word);
    void judgeempty(QString sword);
    void emptystr(QString str);
    void showlistv(QStringList templist);
public:
    int findflag=1;
    QStringList list;
    QStringListModel *model;
    QListView *listview;
    int key;
    bool enterflag=0;
    docxml dx;
    QString nowonsearch;
    QStringList nowonsearchlist;
    searchthread *search;
    QThread *thread;
    QString texthead;
signals:
    //void sendset(QSqlQuery query);
    void showempty();
    void startsearch(QString str);
    void startsearchinwdg(QString text,QString texthead);
    void firstset(QStringList);
public slots:
protected:
    void keyPressEvent(QKeyEvent *e);
    void focusOutEvent(QFocusEvent *e);
};

#endif // SEARCHWIDGET_H
