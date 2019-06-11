#include "mywindow.h"
#include <QApplication>
#include "searchwidget.h"
#include "docxml.h"
#include "socketin.h"
#include "bridge.h"
#include "status.h"
#include <QWebSocketServer>
#include <QWebChannel>
#include <QThread>
#include <QDesktopServices>
#include<MyGlobalShortCut/MyGlobalShortCut.h>
int main(int argc, char *argv[])
{

    //================================================禁用网页安全，这样允许浏览器加载本地资源（图片、音频）
    //QNetworkProxyFactory::setUseSystemConfiguration(false);
    char ARG_DISABLE_WEB_SECURITY[] = "--disable-web-security";
    int newArgc = argc+1+1;
    char** newArgv = new char*[newArgc];
    for(int i=0; i<argc; i++) {
        newArgv[i] = argv[i];
    }
    newArgv[argc] = ARG_DISABLE_WEB_SECURITY;
    newArgv[argc+1] = nullptr;

    QApplication myApplication(newArgc, newArgv);

    //=================================================socket channel 相关设定
    QWebSocketServer server(QStringLiteral("servername"),QWebSocketServer::NonSecureMode);
    //实例化server对象 指定server名称和安全模式
    if(!server.listen(QHostAddress::LocalHost,12345))//设置监听 如果不成功
    {
        qFatal("Failed to open web socket server.");
        //调用windows警告窗口 显示警告内容 比较不友好 可以改为cout
        return 0;
    }
    socketin serversocketready(&server);//实际上是把server的功能封装到socketin类中
    //创建server套接字对象
    QWebChannel channel;//创建channel对象
    QObject::connect(&serversocketready,&socketin::clientConnected,&channel,&QWebChannel::connectTo);
    //如果serversocketready对象收到新连接 则触发预制函数connectTo 将channel连接到传输对象（参数）socketin传来的socket对象
    //webchannel绑定socket 底层通过socket进行通信 socket被channel包裹 channel完成两者页面的桥接 两页面之间直接使用socket通信会因为j两个语言不同 无法直接解析
    mywindow wa;
    bridge bridge(&wa);//创建bridge对象 并和本窗口关联
    channel.registerObject(QStringLiteral("bridge"),&bridge);
    //注册 将bridge暴露给webchannel id为bridge

    //通信依赖实质是通过socket进行 但是webchannel是通信的重要外围设备
    //将webchannel连接到socket 将mywindow对象连接到bridge 最后将bridge在webchannel注册
    //注册这一步使得webchannel能够检测bridge的行为，并配合外围qwebchannel.js完成数据传输 socket作为更加底层的东西存在
    //webchannel完成了程序和socket的整合


    MyGlobalShortCut *shortcut = new MyGlobalShortCut("Alt+c",&wa);
    QObject::connect(shortcut,&MyGlobalShortCut::activated,&wa,&mywindow::activated);
    //注册快捷键 这里只支持 fun+w/fun+fun+w 不支持fun+w+w

//    //================================================================================================
//    QUrl url = QUrl::fromLocalFile("./index.html");//获取本地文件的URL
//    QDesktopServices::openUrl(url);//使用桌面上的默认浏览器打开URL
//    //================================================================================================

    //======================================================修改状态栏全部集中在此
    QObject::connect(&serversocketready,&socketin::skconnect,&wa,&mywindow::paintsignal);//连接成功 就绪
    QObject::connect(&wa,&mywindow::loaded,&wa,&mywindow::paintsignal);//=开始连接 准备
    wa.show();
    return myApplication.exec();
}
