#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QDebug>
#define cout qDebug()<<"["<<__FILE__<<__LINE__<<"]"
class webpage : public QWebEngineView
{
    Q_OBJECT
public:
    explicit webpage(QWidget *parent = nullptr);
    QString selectedstr;
    QWebEngineSettings *qws;
signals:
protected:

public slots:
};

#endif // WEBPAGE_H
