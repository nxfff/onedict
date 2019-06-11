#include "docxml.h"

docxml::docxml()
{

}
void docxml::createxml(QString path)
{
    QFile file(path);
    if(file.exists())
    {
        cout<<path<<" xml file already exits";
        return;
    }
    else
    {
       bool succ=file.open(QIODevice::WriteOnly);
       if(succ)
       {
          QDomDocument doc;//文档节点
          QDomProcessingInstruction ins;//说明节点
          ins=doc.createProcessingInstruction("xml","version=\'1.0\' encoding=\'utf-8\'");
          //在文档内创建说明节点 参数1：文件类型 参数2：指明文档信息
          doc.appendChild(ins);//将说明节点追加到文档中

          QDomElement root=doc.createElement("path");//创建节点 名为根节点
          doc.appendChild(root);//文档追加根节点

          QTextStream stream(&file);
          doc.save(stream,4);//保存 缩进4个字符
          file.close();
          //保存
          cout<<path<<"created successfully";
       }
       else
       {
           cout<<"open "<<path<<" failed";
       }
    }
}
void docxml::delele(QString path,int posit)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        bool succ=doc.setContent(&file);//将文件中的内容转移到新创建的doc中
        file.close();//关闭文件对象
        if(succ)
        {
            QDomElement root=doc.documentElement();//获取根节点
            QDomNodeList nlist=root.childNodes();//获取根节点的所有子节点 并存入节点列表
            int listlength=nlist.length();//获取子节点长度
            root.removeChild(root.childNodes().at(listlength-1-posit));
            //传来的参数是从逆序表中获取的 所以这里执行删除的是对称位置的元素

            //保存
            file.open(QIODevice::WriteOnly);
            QTextStream stream(&file);
            doc.save(stream,4);
            file.close();
        }
    }
}
void docxml::addelement(QString path,QString cont)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        bool succ=doc.setContent(&file);//将文件中的内容转移到新创建的doc中
        file.close();//关闭文件对象
        if(succ)
        {
            QDomElement root=doc.documentElement();//获取根节点

            QDomElement content=doc.createElement("wordx");//创建root的子节点 名为wordx

            QDomText text=doc.createTextNode(cont);//创建文本节点 内容为cont 传来的参数
            //=======================删除重复
            QStringList slist=readelement(doc);//调用readelement函数 逆序读取文档中的所有节点内容
            int listlength=slist.length();//获取文档中的节点个数
            for (int i=0;i<listlength;i++)//查看文档中所用节点的内容 删除和准备添加内容相同的节点
            {
                if(slist[i]==cont)
                {
                    cout<<"in"<<path<<"already have"<<i<<"."<<slist[i];
                    root.removeChild(root.childNodes().at(listlength-1-i));//直接对root节点进行操作
                    //readelement从下向上读 是反着的 所以应该删除对称位置
                }
            }
            //=======================删除多余
            QDomNodeList nlist=root.childNodes();//删除多余节点
            if(nlist.length()>=5)//限制xml节点数量
            {
                cout<<path<<"is full";
                root.removeChild(root.childNodes().at(0));//直接对root节点进行操作 删除首个节点（最先添加进去的）
            }

            content.appendChild(text);//子节点添加文本节点
            root.appendChild(content);//根节点添加新的子节点

            //save
            file.open(QIODevice::WriteOnly);
            QTextStream stream(&file);
            doc.save(stream,4);
            file.close();

        }
        else {
            cout<<path<<"set content failed in add";
        }
    }
    else {
        cout<<path<<"open failed in add";
    }
}
QStringList docxml::readelement(QDomDocument doc)
{
    QStringList list;
    QDomElement root=doc.documentElement();
    QDomNodeList doclist=root.childNodes();
    //获取根节点下的所有子节点 放入list中
    QString text;
    for(int i=doclist.length()-1;i>=0;i--)
    {
        text=doclist.at(i).toElement().text();
        //获取子节点 并转为文字
        list.append(text);
        //将文字添加到文本列表中
    }//从下向上反着读取！！！
    return  list;

}
