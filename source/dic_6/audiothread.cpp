#include "audiothread.h"
#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
using namespace libZPlay;
audiothread::audiothread(QObject *parent) : QObject(parent)
{

}
void audiothread::playaudio(QString str)//参数str由mywindow传来 是资源文件内部的相对音频地址如us/hope__us_1.spx
{
    QString audiopath;

    QRegExp rx("\\.(.*)");//获取.之后的内容 即后缀名
    QString tail;
    if( rx.indexIn(str, 0) != -1) //返回首次匹配出现的位置 若为-1 则不匹配匹配 参数2为搜索模式 0表示从头搜索
    {
        tail=rx.cap(1);//获取第一个括号内抓取的内容 获取后缀名
    }
    else
    {
        cout<<"not name.xxx";
        return;
    }
    QString path;//用于存储资源文件路径
    if(tail=="spx"||tail=="wav")//如果后缀名为spx libzplay控件无法播放 需要调用ffmpeg进行转码
    {//根据后缀名指定音频路径 不是通用解 但是在此项目中是可以使用的、很方便的方法
        if(tail=="spx")
        {
            path="oxford/";
        }
        else
        {
            path="Macmillan/";//wav控件提示不支持压缩后的wav
        }
        path.append(str);//资源文件路径+音频在资源文件内部的相对地址=音频文件的相对地址

        //通过调用ffmpeg.exe对path指向的音频进行转码 将转码完成的音频命名为temp.mp3 并存储在构建文件夹中

        QProcess p(0);
        //准备执行cmd 生成语句
        p.start("cmd", QStringList()<<"/c"<<QString("ffmpeg -i %1 %2").arg(path).arg("temp.mp3"));
        //执行cmd语句
        cout<<"start";
        p.waitForStarted();
        p.waitForFinished();
        cout<<"end";
        audiopath="temp.mp3";//将转码后的地址赋予audiopath
    }
    else if(tail=="mp3")//mp3直接播放资源文件内部的音频
    {
        path="longman/";
        audiopath=path.append(str);;//不是spx时 直接将收到的地址赋予audiopath
    }

    //开始播放
    cout<<"play";
    ZPlay *player = CreateZPlay();
    // open file
    int result = player->OpenFile(audiopath.toUtf8(), sfAutodetect);//播放audiopath .utf8将string转为char*
    if(result == 0)//播放失败
    {
        // display error message
        cout<<player->GetError();
        player->Release();
        return;
    }
    // get song length
    TStreamInfo info;
    player->GetStreamInfo(&info);//获取音频信息
    int msec=info.Length.hms.minute*60000+info.Length.hms.second*1000+info.Length.hms.millisecond;
    //获取音频的长度 单位毫秒
    cout<<"audio mtime:"<<msec;
    // start playing
    if(player->Play())//该控件使用独立线程播放音频 在播放时需要当前线程等待 原则上可以像下面那样顺序执行 但是实际执行中独立播放线程似乎会卡顿 所以改成这样 开始播放后再进行休眠计时 确保有足够的播放时间
    {
        QThread::msleep(msec);//休眠线程 等待播放
        cout<<"sleep end";
        //cout<<QFile::remove("temp.mp3");//release之前无法删除 文件还在被使用
        player->Release();
        if(audiopath=="temp.mp3")
        {
            cout<<"delete";
            cout<<QFile::remove("temp.mp3");//不删除 第二次播放音频会卡住
            //wav spx这两种音频在播放时需要删除 mp3不需要删除
        }
    }
//    QThread::msleep(msec);//休眠线程 等待播放
//    cout<<"sleep end";
//    //cout<<QFile::remove("temp.mp3");//release之前无法删除 文件还在被使用
//    player->Release(); cout<<"delete";
//    cout<<QFile::remove("temp.mp3");//不删除 第二次播放音频会卡住
}
