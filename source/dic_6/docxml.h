#ifndef DOCXML_H
#define DOCXML_H
#include <QString>
#include <QFile>
#include <QDomDocument>//文档节点
#include <QDomProcessingInstruction>//文档介绍节点
#include <QStringList>
#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
class docxml
{
public:
    docxml();
    static void createxml(QString path);
    static void addelement(QString path,QString cont);
    static QStringList readelement(QDomDocument doc);
    void delele(QString path,int posit);
};

#endif // DOCXML_H
