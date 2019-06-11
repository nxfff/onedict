#include "serversocket.h"

serversocket::serversocket(QWebSocket *socket) : QWebChannelAbstractTransport(socket),ssocket(socket)
{//括号中的socket都是参数
    connect(socket, &QWebSocket::textMessageReceived,this, &serversocket::textmessagereceived);
    //收到信息后 调用textmessagereceived槽函数
    connect(socket, &QWebSocket::disconnected,this, &serversocket::deleteLater);
    //收到断开连接信号后 调用类中的函数清除this object
}

serversocket::~serversocket()
{
    ssocket->deleteLater();//程序结束后清除ssocket对象
}
void serversocket::sendMessage(const QJsonObject &message)
{
    //cout<<"server send"<<message;
    QJsonDocument doc(message);
    ssocket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
}//通过socket发送信息时自动调用此函数

void serversocket::textmessagereceived(const QString &messageData)
{
    //cout<<"server reciver";
    QJsonParseError error;//用于汇报js的错误的对象
    QJsonDocument message = QJsonDocument::fromJson(messageData.toUtf8(), &error);
    //用于读写json文件对象 参数1 json数据对象 参数2 转换失败后报错信息的接收对象
    //QJsonDocument对象能够方便的读写json对象fromJson是把json对象转为utf8格式的文件对象
    if (error.error) {//如果有错误
        qWarning() << "Failed to parse text message as JSON object:" << messageData
                   << "Error is:" << error.errorString();//错误的信息内容
        return;
    } else if (!message.isObject()) {//如果不包含对象
        qWarning() << "Received JSON message that is not an object: " << messageData;
        return;
    }
    emit messageReceived(message.object(), this);//qt默认从客户端收到信息后必须做的事
    //把message中包含的对象发送到socket中
}
