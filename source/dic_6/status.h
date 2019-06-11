#ifndef STATUS_H
#define STATUS_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>
class status : public QWidget
{
    Q_OBJECT
public:
    explicit status();
    void changestat(int x);
private:
    QLabel *label;
    QLabel *hint;
    QPixmap *mapa,*mapb,*mapc;
signals:

public slots:
};

#endif // STATUS_H
