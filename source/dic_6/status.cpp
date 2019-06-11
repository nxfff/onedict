#include "status.h"
status::status()//注意这里为构造函数添加参数的方法 参数在前
{
    hint=new QLabel();

    mapc=new QPixmap(10,10);
    QPainter pc(mapc);
    pc.fillRect(0,0,mapc->width(),mapc->height(),QBrush(Qt::green));
    //参数1 左上角坐标 宽 高 brush工具

    mapb=new QPixmap(10,10);
    QPainter pb(mapb);
    pb.fillRect(0,0,mapb->width(),mapb->height(),QBrush(Qt::red));

    mapa=new QPixmap(10,10);
    QPainter pa(mapa);
    pa.fillRect(0,0,mapa->width(),mapa->height(),QBrush(Qt::yellow));

    hint->setText("空闲");
    label=new QLabel();
    label->setPixmap(*mapa);

    QHBoxLayout *layout=new QHBoxLayout;//可以在后面括号内写this指定父对象 那样的话最后一行不用写
    layout->addWidget(label);
    layout->addWidget(hint);
    layout->setContentsMargins(0,0,0,0);//布局中的默认边缘空白太大 全部改为0
    setLayout(layout);//看27行

}
void status::changestat(int x)
{
    switch (x)
    {
    case 1:label->setPixmap(*mapb);hint->setText("准备");break;
    case 2:label->setPixmap(*mapc);hint->setText("就绪");break;
    }
}
