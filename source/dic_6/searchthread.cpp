#include "searchthread.h"
searchthread::searchthread(QObject *parent) : QObject(parent)
{

}
void searchthread::dealstr(QString sword)
{
    cout<<"whole str"<<sword;
    pathList.clear();//清空pathlist常驻线程 必须清空
    //emptyflag=1;//初始化空值标记 常驻线程 必须初始化
    //ef抛弃
    for (int i=0;i<6;i++)//循环次数==字典数量
    {
        QString dictname;//数据表名
        QString filepath;//存储路径
        switch (i)//根据顺序 分别搜索不同的词典数据库 并指定不同的存储路径
        {
        case 0:dictname="coca";filepath="coca/";break;
        case 1:dictname="oxford";filepath="oxford/";break;
        case 2:dictname="longman";filepath="longman/";break;
        case 3:dictname="macmillan";filepath="macmillan/mcml/";break;
        case 4:dictname="21cent";filepath="21cent/";break;
        case 5:dictname="collins";filepath="collins/";break;
        }//根据不同词典指定词典名 和 路径
        QString id;
        QSqlQuery query;
        query.exec(QString("select id from %1head where head ='%2';").arg(dictname).arg(sword));//查询词条在对应词典中的id
        while(query.next())
        {
            id=query.value("id").toString();//获取id
        }
        if(id=="")//如果id为空 当前词典内未查询到目标词条
        {
            //不执行下面的代码 但是序号仍然加一
            pagenum[i]++;
            continue;
        }
        //emptyflag=0;//只要有一个不为空 比然到此改变emptyflag的值 如果所有词典中都没有搜索到结果 当前语句永远不会被执行
        //判空工作已经在搜索线程之外完成 这里不需要再次判定
        //上面注释对 初始化时ef=1，如果取消这一行ef永远是1……永远是空内容 所以要么取消ef 直接参数上写false 要么留下ef 这里该值
        query.exec(QString("select content from %1body where id ='%2';").arg(dictname).arg(id));//id不为空 在对应词典内执行搜索
        query.first();//数据集指针下移一位 指向数据集首条信息

        QString str;
        if(i==0)
        {
            //获取str 并复原字符串
            str=query.value(0).toString().replace("<>@@@LINK","@@@LINK");
            //cout<<str;
            //将转换过程中的字符转换恢复 仅针对oxford
        }
        else {
            str=query.value(0).toString();//0 代表首个属性 如果查了id 和 head 0=id 1=head
        }
        str.replace(("\\n"),"");//词典中的原码有时会有大量\n直接解析显示 造成页面混乱

        QString tail;//引入js文件
        if(i!=3)
        {
            tail="<script type=\"text/javascript\" src=\"../jquery-3.3.1.min.js\"></script>"
                 "<script type=\"text/javascript\" src=\"../qwebchannel.js\"></script>"
                 "<script type=\"text/javascript\" src=\"../test.js\"></script>";
        }
        else {//马克米兰手动设了2级菜单 提高速度 在此手动重写tail
            tail="<script type=\"text/javascript\" src=\"../../jquery-3.3.1.min.js\"></script>"
                 "<script type=\"text/javascript\" src=\"../../qwebchannel.js\"></script>"
                 "<script type=\"text/javascript\" src=\"../../test.js\"></script>";
        }
        str.append(tail);//引用js 支持web交互能力

        //将字符串写入文件
        //生成词典地址
        QString temppath=QString("%1temp%2.html").arg(filepath).arg(pagenum[i]);//生成地址
        cout<<temppath;
        pathList.append(temppath);//将本次搜索成功的页面写入pathlist供mywindow直接使用

        //开始存储
        QFile file(temppath);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(str.toUtf8());
            file.close();
            pagenum[i]++;//存储成功 词典对应值+1
        }//写入地址
        else
        {
            cout<<"open failed";
            system("PAUSE");
            return;
        }
    }
    emit searchfinish(false,pathList);//发出搜索完成信号 传出空值flag和存储搜索结果的html文件列表
    cout<<"attention"<<pagenum;//输出pagenum数组 供参考
}
void searchthread::searchinwdg(QString text,QString texthead)//对参数text执行模糊搜索
{
    cout<<"fuzzy search"<<text;
    QStringList tempList;
    cout<<"0";
    QSqlQuery query;
    cout<<"1";
    if(text=="^")
    {
        text="\\\\^";//必须用4个
    }
    query.exec(QString("select head from head"+texthead+" where head REGEXP '^%1.*'").arg(text));//从head中搜索相关词条
    cout<<"2";
    while(query.next())
    {
        QString word=query.value("head").toString();
        word.replace("^","\'");//恢复原来字符串的内容
        tempList.append(word);//加空 就==什么都不加
        //遍历set中的数据 分别加入两个list
        cout<<"3";
    }
    cout<<"add finish";
    cout<<tempList;
    if(tempList.length()<=6&&texthead!="")//避免重复搜索head表
    {
        query.exec(QString("select head from head where head REGEXP '^\\(.+\\) "+text+".*' limit 0,5;").arg(text));
        while(query.next())
        {
            QString word=query.value("head").toString();
            word.replace("^","\'");//恢复原来字符串的内容
            tempList.append(word);//加空 就==什么都不加
            //遍历set中的数据 分别加入两个list
            cout<<"4";
        }
    }
    emit sendset(tempList);//将存有相似词条的stringlist发出
    cout<<"finish";
}
