#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QWebChannelAbstractTransport>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
class serversocket : public QWebChannelAbstractTransport
{
    Q_OBJECT
public:
    //explicit serversocket(QObject *parent = nullptr);
    explicit serversocket(QWebSocket *socket);//构造函数带参数
    virtual ~serversocket();//重写析构函数
    void sendMessage(const QJsonObject &message) override;//重写QWebChannelAbstractTransport内的函数
signals:

private slots:
    void textmessagereceived(const QString &message);//信息接收函数
private:
    QWebSocket *ssocket;
};

#endif // SERVERSOCKET_H
