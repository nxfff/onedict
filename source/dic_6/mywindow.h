#ifndef MYWINDOW_H
#define MYWINDOW_H
#include <QMainWindow>
#include <QPushButton>//按钮
#include <QLineEdit>//单行文本框
//#include <QCompleter>//自动补全 弃用
#include <QDir>//路径对象 用于获取特定路径之下的 文件和路径信息
#include <QLabel>
#include <QLineEdit>
#include <QSqlDatabase>//数据库对象
#include <QSqlError>//数据库报错对象
#include <QSqlQuery>//sql语句对象
#include <QIODevice>//输入输出使用的设备对象
#include <QFile>
#include <QStackedLayout>//布局对象
#include <QMenu>//菜单对象
#include <QStatusBar>//状态栏
#include <QThread>//线程
#include <QRegExp>//正则表达式
#include <QClipboard>//访问剪贴板
#include "searchwidget.h"//搜索控件
#include "webpage.h"//webengine控件
#include "docxml.h"//xml文档控件
#include "socketin.h"//通信套接字控件
#include "status.h"//状态栏自定义组件
#include "timethread.h"//计时线程
#include "audiothread.h"//音频线程

#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
namespace Ui {
class mywindow;
}

class mywindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mywindow(QWidget *parent = nullptr);
    ~mywindow();
    void showxml(QString filename,int mode);//显示xml
    void getMessage(const QString &message);//接收webchannel 超链接内容
    void paintsignal(int color);//绘制状态栏
    void showmenu(const QPoint& pos);//显示历史、收藏的菜单
    void dealselectstr(const QString &selectedstr);//接收webchannel 双击选中内容
    void showmultiweb(bool emptyflag,QStringList pathlist);//加载父页面
    void showinfosource();//显示数据来源
    void activated();//快捷键相应
    void judgeposi();//判断前进 后退键是否可用 避免越界
private:
    Ui::mywindow *ui;
    searchwidget *inputedit;
    QPushButton *forwardbtn;
    QPushButton *backbtn;
    QPushButton *clbtn;
    //QStringList list;
    //QStringList rowlist;
    //QCompleter *com;
    webpage *wpage;
    docxml dx;
    QStringListModel *Model;
    QStringListModel *dictmodel;
    QThread *threadtime;
    timethread *tt;
    status *statu;
    QStringList templist;
    audiothread *at;
    QThread *threadaudio;
    QMenu *rmenu;
    QAction *delitem;
    QLabel *drawtime;
    QStringList succnum;
    QDir *dir;
    int s=0;
    int pagenum=0;
    int dictnum=0;
signals:
     //void sendText(const QString &text);
     //void threadtest();
     //void testthread(int x);
     void loaded(int x);
     void audio(QString path);
};

#endif // MYWINDOW_H
