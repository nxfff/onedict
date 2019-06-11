#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H
#include <QObject>
#include "libzplay.h"//音频播放插件
#include <QThread>//线程
#include <QProcess>//cmd使用类
#include <QFile>//文件读写
class audiothread : public QObject
{
    Q_OBJECT
public:
    explicit audiothread(QObject *parent = nullptr);
    void playaudio(QString str);
signals:
public slots:
};

#endif // AUDIOTHREAD_H
