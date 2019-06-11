#include "searchwidget.h"

searchwidget::searchwidget(QWidget *parent) : QLineEdit(parent)
{
    //qRegisterMetaType<QList<int>>("QList<int>");//注册QList参数 startsearch触发searchthread时 跨线程传参 str可以直接传 数组不可以 必须要注册
    setFixedWidth(200);//锁定当前控件宽度
    listview=new QListView(this);//实例化listview对象 下面备用
    listview->setWindowFlags(Qt::ToolTip);//如果不加这句会出现输入框内没有光标的情况

    model=new QStringListModel(this);//实例化QStringListModel对象 下面备用

    connect(this,&searchwidget::textChanged,this,&searchwidget::letchanged);//控件内的内容发生改变 触发模糊搜索

    enterflag=1;//允许显示listview 下面的listview将依据此变量决定是否显示

    search=new searchthread();
    thread=new QThread();
    search->moveToThread(thread);
    thread->start();
    connect(this,&searchwidget::startsearch,search,&searchthread::dealstr);//触发线程函数
    //实例化搜索线程 并指定触发信号
    connect(this,&searchwidget::startsearchinwdg,search,&searchthread::searchinwdg);//开始模糊搜索
    connect(search,&searchthread::sendset,this,&searchwidget::showlistv);//模糊搜索完成 接收stringlist
    //====================================================
    connect(this,&searchwidget::firstset,this,&searchwidget::showlistv);//预搜索显示
    //====================================================
    connect(this,&searchwidget::destroyed,
            [=]
    {
        thread->quit();
        cout<<"wait searchthread"<<thread->wait();
    });
}
searchwidget::~searchwidget()
{

}
void searchwidget::focusOutEvent(QFocusEvent *e)
{
    listview->hide();//失去焦点 隐藏listview
}

void searchwidget::letchanged()
{
    list.clear();
    //清除list内容
    QString text=this->text();//获取文本框内部数据
    texthead=text.at(0);//如果输入法接入 输入框一次接受到完整的词条 将无法进入length=1函数下指定搜索用表
    cout<<"changed"<<enterflag;
    if(text!="")
    {
        text.replace("'","^");//处理输入字符（在数据导入数据库时，将单引号转为^)
        cout<<text;
        if(text.length()==1)
        {
            cout<<"length=1";
            texthead=text.at(0);//获取受字符
            QRegExp rx("[a-z]");//正则表达式 字母
            if(rx.indexIn(texthead)==-1)//首字符不为字母
            {
                texthead="";//置空texthead 到下面对head表发起查询
            }
            else//只有一个字母时直接显示预搜索页面
            {
                text=text.toLower();//转小写
                QFile file("firstset/"+text+".xml");

                if(file.open(QIODevice::ReadOnly))
                {
                    QDomDocument doc;
                    bool succ=doc.setContent(&file);
                    file.close();
                    if(succ)
                    {
                        QStringList list=dx.readelement(doc);//readelement参数是文档对象
                        emit firstset(list);
                        return;
                    }
                    else
                    {
                        cout<<"set content failed";
                    }
                }
                else
                {
                    cout<<"open failed";
                }
            }
        }
        //============================================

        cout<<"changed!!!!!"<<texthead;
        emit startsearchinwdg(text,texthead);//触发搜索线程 执行模糊搜索
    }
    else {
        listview->hide();
    }
}
void searchwidget::showlistv(QStringList templist)
{
    //connect(this,&searchwidget::startsearchinwdg,search,&searchthread::searchinwdg);//重连
    cout<<enterflag;
    list=templist;//获取模糊搜索结果
    //通过上下键选择元素 enter键执行搜索 指针默认指向listview首个元素 直接回车或者选择后回车都会触发内容修改函数
    //而内容修改函数会重新实例化一个listview 空置显示 为避免这样的情况 创建一个enterflag变量 按下enter后不再实例化listview
    //完成结果渲染后 将flag初始化
    if(enterflag==1)
    {
        model->setStringList(list);
        listview->setModel(model);
        if(model->rowCount()==0)
        {
            cout<<"result empty";
            listview->hide();
            //在此处隐藏listview 这样就不会进入下面的键盘事件 否则在enter事件后没有机会复位enterflag 在if(list[0]==str)处不符合 一路向下 出现非预期情况
            return;
        }
        listview->setMaximumWidth(width());
        listview->setMinimumWidth(width());
        QPoint p(0,height());//希望listview的左上角位于窗口内该点 listview独立添加在lineedit下面
        int x=mapToGlobal(p).x();
        int y=mapToGlobal(p).y()+1;
        //将目标点转为全局坐标
        listview->move(x,y);
        //cout<<"showlistview";
        listview->show();
    }
    else
    {
        cout<<"searching!!";
    }

}
void searchwidget::judgeempty(QString sword)
{
    QSqlQuery query;
    query.exec(QString("select head from head%1 where head='%2'").arg(sword.at(0)).arg(sword));//不能用模糊搜索
    query.first();
    if(query.value("head").toString()=="")
    {
        cout<<"judegempty notpass";
        emptystr(sword);
    }
    else
    {
        cout<<"judegempty pass";
        judgesame(sword);
    }
}
void searchwidget::judgesame(QString sword)
{
    cout<<"before"<<nowonsearch<<"now"<<sword;
    if(sword==nowonsearch)
    {
        cout<<"donot search again";
        return;
    }
    //==========================================上交测试代码的补充保护机制，避免对数据库中没有的元素发起搜索
    if(sword[0]!="a")
    {
        cout<<"not start with a";
        enterflag=1;//复位enterflag
        return;
    }//上传代码中只允许搜索a开头的词条
    //==========================================
    nowonsearch=sword;
    nowonsearchlist.append(nowonsearch);
    dx.addelement("wordsh.xml",sword);//添加历史
    emit startsearch(sword);//发起搜索
}
void searchwidget::keyPressEvent(QKeyEvent *e)
{
    if(!listview->isHidden())//在list没有显示出来前无法获取listview的属性 会报错（文本变化函数 实例化后显示，文本变化函数在按键处理函数之后）
    {
        QModelIndex currentrow=listview->currentIndex();
        //当前下标
        int count=listview->model()->rowCount();
        //列表中的行总数
        //======================================获取上面两个参数供下面使用

        //获取listview当前选中的元素下标
        key=e->key();
        if(key==Qt::Key_Enter|| key==Qt::Key_Return)//enter对应小键盘回车+return对应主键盘回车
        {
            enterflag=0;//回车一瞬间 禁用listview
            cout<<"huiche";
            //==============================================
            QString str;
            if(currentrow.isValid())//按上下键选择后currentindex才会赋值(选择后才有item被选中 获取的是item序号） 否则该指针空置
            {
                str=listview->currentIndex().data().toString();
                cout<<str<<"test";
                emit judgesame(str);//列表中出现的词条一定不为空
                disconnect(this,&searchwidget::textChanged,0,0);//断连
                setText(str);
                cout<<"b";
            }
            else
            {//否则直接使用文本框中的内容
                str=this->text();
                //------------------------


                //for (int i=0;i<list.length();i++)只要看第一个元素 下面的if判断都可以省略
                if(list[0]==str)
                {
                    judgesame(str);
                    cout<<"a";
                }
                else {
                    cout<<QStringLiteral("执行搜索词条与联系列表中的首个词条不符合");
                    enterflag=1;
                }
            }
            cout<<str<<"12345";
            listview->close();//执行搜索任务前将listview关闭
        }
        else if(key==Qt::Key_Down)
        {
            int row=currentrow.row()+1;//新建一个值=当前行数加1
            if(row>=count)
            {
                row=0;
            }//超出后重新指向第一个元素
            QModelIndex index=listview->model()->index(row,0);//获取该位置元素的QModelIndex对象
            //设定modelindex对象 内容为 指定行 0列//modelindex中包含行 列两个属性 要单独提取请采用此种方法
            listview->setCurrentIndex(index);
            //修改listview的行指向
        }
        else if(key==Qt::Key_Up)
        {
            int row=currentrow.row()-1;//新建一个值=当前行数减1
            if(row<0)
            {
                row=count-1;
            }//超出后重新指向第一个元素
            QModelIndex index=listview->model()->index(row,0);
            //设定modelindex对象 内容为指定行 0列//modelindex中包含行 列两个属性 要单独提取请采用此种方法
            listview->setCurrentIndex(index);
            //修改listview的行指向
        }
        else
        {
            QLineEdit::keyPressEvent(e);
        }
    }
    else
    {
        key=e->key();
        if(key==Qt::Key_Enter|| key==Qt::Key_Return)
        {
            emptystr(this->text());
        }
        else
        {
            QLineEdit::keyPressEvent(e);
        }
    }
}
void searchwidget::emptystr(QString str)
{
    cout<<"not search";
    QRegExp rx("\\S");//不为空 \S非空字符
    if(rx.indexIn(str)!=-1)
    {
        dx.addelement("wordsh.xml",str);
        nowonsearch=str;
        nowonsearchlist.append(nowonsearch);
        emit search->searchfinish(true,search->pathList);//参数2为空值
    }
}

