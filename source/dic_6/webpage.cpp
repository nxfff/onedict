#include "webpage.h"
webpage::webpage(QWidget *parent) : QWebEngineView(parent)
{

    //一定要写在第一行
    //开启调试模式 将数据输送到指定端口
    //    QWebEngineSettings* settings = QWebEngineSettings::globalSettings();
    //    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
//    connect(this,&QWebEngineView::selectionChanged,
//            [=]
//    {
//        QThread::msleep(200);
//        cout<<"selectchanged";
//        //if(selectflag==0)
//        {
//            selectedstr=selectedText();
//            if(selectedstr!="")
//            {
//                selectedstr.replace(" ","");//把最后的空格 替换为null---消除空格
//                QRegExp rx("([a-z]+)");
//                if( rx.indexIn(selectedstr, 0) != -1) //返回首次匹配出现的位置 若为-1 则没有匹配 参数2为搜索模式 表示从头搜索
//                {
//                    selectedstr=rx.cap(1);//取出单词部分 否则形似 ˈmotor (单引号开头）搜索不到内容
//                }
//                emit singleword(selectedstr);

//            }
//        }

//    });
}
