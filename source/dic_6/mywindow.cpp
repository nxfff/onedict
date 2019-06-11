#include "mywindow.h"
#include "ui_mywindow.h"
mywindow::mywindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mywindow)
{
    ui->setupUi(this);

    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("dict");
    if(!db.open())
    {
        cout<<db.lastError().text();
        return;
    }
    //=====================================数据库连接
    ui->gridLayout->setContentsMargins(0,0,0,0);
    ui->splitter->setContentsMargins(0,0,0,0);
    ui->splitter_2->setContentsMargins(0,0,0,0);//布局中的默认边缘空白太大 全部改为0
    //ui->widget->setStyleSheet("QWidget{background-color:rgb(0,255,255);}");//使用css 测试布局边缘空白
    //=====================================将ui中的所有布局的默认边缘空白改为0 默认为11

    dx.createxml("wordsh.xml");//构造函数创建xml
    showxml("wordsh.xml",1);//显示历史记录内容
    //=====================================程序初始显示搜索历史
    dx.createxml("wordsc.xml");//创建收藏栏
    showxml("wordsc.xml",2);
    //=====================================程序初始显示收藏
    inputedit=new searchwidget(this);//使用独立的搜索框类可以更方便的使用输入框的按键事件 不需要过滤
    ui->toolbar->addWidget(inputedit);
    //==================================引入独立输入框
    wpage = new webpage(ui->widget);
    QGridLayout *layout=new QGridLayout(ui->widget);
    layout->addWidget(wpage);
    layout->setContentsMargins(0,0,0,0);//webengine消除边缘空白
    //***********************************************************************************
    wpage->blockSignals(true);
    wpage->load(QUrl("file:///start/welcome.html"));
    //================================================将webview控件加入widget中 记住必须添加布局 否则不会铺满
    clbtn=new QPushButton(QStringLiteral("收藏"));
    ui->toolbar->addWidget(clbtn);
    connect(clbtn,&QPushButton::clicked,
            [=]
    {
        cout<<"shoucang";
        dx.addelement("wordsc.xml",inputedit->nowonsearch);//添加
        showxml("wordsc.xml",2);//显示
    });
    //收藏按键

    backbtn=new QPushButton(QStringLiteral("后退"));
    ui->toolbar->addWidget(backbtn);
    connect(backbtn,&QPushButton::clicked,
            [=]
    {

        for(int j=0;j<inputedit->search->pagenum.length();j++)
        {
            inputedit->search->pagenum[j]--;
        }//子页面后退

        pagenum--;//访问1 在访问2 后退是下次写入的目标页面变为2 若此时前进 下次写入页面变为3 见下面的pagenum++

        inputedit->nowonsearch=inputedit->nowonsearchlist[pagenum-1];//修改当前搜索对象
        disconnect(inputedit,&searchwidget::textChanged,0,0);
        inputedit->setText(inputedit->nowonsearch);//将当前搜索对象放入搜索框

        dictnum=succnum[pagenum-1].toInt();
        wpage->page()->triggerAction(QWebEnginePage::Back);
        cout<<pagenum;
        judgeposi();
    });
    //后退按键
    forwardbtn=new QPushButton(QStringLiteral("前进"));
    ui->toolbar->addWidget(forwardbtn);
    connect(forwardbtn,&QPushButton::clicked,
            [=]
    {
        for(int j=0;j<inputedit->search->pagenum.length();j++)
        {
            inputedit->search->pagenum[j]++;
        }//子页面后退
        pagenum++;

        inputedit->nowonsearch=inputedit->nowonsearchlist[pagenum-1];//修改当前搜索对象
        disconnect(inputedit,&searchwidget::textChanged,0,0);
        inputedit->setText(inputedit->nowonsearch);//将当前搜索对象放入搜索框

        dictnum=succnum[pagenum-1].toInt();
        wpage->page()->triggerAction(QWebEnginePage::Forward);
        cout<<pagenum;
        judgeposi();
    });
    //前进按键
    judgeposi();//初始化前进 后退按键的可用性 必须在按键实例化完成之后进行检查
    //================================================前进、后退、收藏按键功能实现
    //计时线程
    tt=new timethread;
    threadtime=new QThread();
    tt->moveToThread(threadtime);
    threadtime->start();
    connect(inputedit,&searchwidget::startsearch,tt,&timethread::addnum);//指定触发信号
    connect(wpage,&QWebEngineView::loadFinished,tt,&timethread::tEnd);//停止并重置计时器



    //=====================================================================计时线程实例化
    connect(wpage,&QWebEngineView::loadFinished,
            [=](bool x)
    {
        emit loaded(1);//主函数接收 开始进入准备状态
        connect(inputedit,&searchwidget::startsearchinwdg,inputedit->search,&searchthread::searchinwdg);
        cout<<inputedit->enterflag;
        //hotkey触发 断开上述连接 页面加载完成后 重新恢复上述连接
    }); //知识点：：：：同一个信号 多个槽函数 先触发类内函数
    //=========================页面加载完成 开始连接socket（状态：空闲→准备）
    //修改状态栏
    QStatusBar *statubar;
    statubar=statusBar();//添加状态栏
    statu=new status();
    statubar->addWidget(statu);//向状态栏内添加状态控件
    drawtime=new QLabel();
    statubar->addWidget(drawtime);//向状态栏内添加渲染时间控件
    connect(tt,&timethread::sendtime,
            [=](int x)
    {
        if(x!=0)
        drawtime->setText("drawtime:"+QString::number(x));
        else {
            drawtime->setText("");
        }
    });//计时器线程tEnd被触发后 输出时间
    //=========================实例化状态栏组件 并向其中添加自定义状态组件
    at=new audiothread;
    threadaudio=new QThread();
    at->moveToThread(threadaudio);
    threadaudio->start();
    connect(this,&mywindow::audio,at,&audiothread::playaudio);//指定触发音频播放函数的信号
    //===========================音频播放线程
    ui->infosource->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //====================================================禁用infosource双击修改
    ui->lvhistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lvcollection->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //关闭页面历史和收藏的双击编辑功能
    ui->lvhistory->setContextMenuPolicy(Qt::CustomContextMenu);//设定显示菜单的方式
    connect(ui->lvhistory,&QListView::customContextMenuRequested,this,&mywindow::showmenu);//右键对应槽函数
    ui->lvcollection->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lvcollection,&QListView::customContextMenuRequested,this,&mywindow::showmenu);
    //====================================================================添加右键菜单 并指定触发函数
    connect(ui->lvhistory,&QListView::doubleClicked,
            [=]
    {
        QString sword=ui->lvhistory->currentIndex().data().toString();
        disconnect(inputedit,&searchwidget::textChanged,0,0);
        inputedit->setText(sword);

        inputedit->judgeempty(sword);
    });
    connect(ui->lvcollection,&QListView::doubleClicked,
            [=]
    {
        QString sword=ui->lvcollection->currentIndex().data().toString();
        disconnect(inputedit,&searchwidget::textChanged,0,0);
        inputedit->setText(sword);

        inputedit->judgeempty(sword);
    });
    //===============================历史和收藏 双击搜索
    connect(inputedit->search,&searchthread::searchfinish,this,&mywindow::showmultiweb);
    //=============================显示搜索结果
    connect(this,&mywindow::destroyed,
            [=]
    {
        QStringList dictlist;
        dictlist.append("./");//父页面地址
        dictlist.append("./oxford/");
        dictlist.append("./longman/");
        dictlist.append("./21cent/");
        dictlist.append("./coca/");
        dictlist.append("./collins/");
        dictlist.append("./macmillan/mcml/");//6个子页面地址
        for(int j=0;j<dictlist.length();j++)
        {
            cout<<dictlist[j];
            dir=new QDir(dictlist[j]);
            QStringList list=dir->entryList();//获取目录下所有项目的名称
            QStringList listhtml=list.filter(".html");//过滤出后缀为HTML的文件
            cout<<listhtml;
            for(int i=0;i<listhtml.length();i++)
                QFile::remove(dictlist[j]+listhtml[i]);//必须加上前面的路径 否则只有名字（或只能删除当前页面）
        }
        threadtime->quit();
        cout<<"wait timerthread"<<threadtime->wait();
        threadaudio->quit();
        cout<<"wait audiothread"<<threadaudio->wait();
    });
    //=============================程序结束 删除所有临时页面文件结束当前控件开启的线程
    connect(inputedit->search,&searchthread::searchfinish,tt,&timethread::sqltime);

}
void mywindow::judgeposi()
{
    cout<<pagenum<<"//"<<succnum.length();
    if(pagenum==succnum.length())
    {
        forwardbtn->setEnabled(false);
    }
    else
    {
        forwardbtn->setEnabled(true);
    }

    if(pagenum==1||pagenum==0)//设置可用条件
    {
        backbtn->setEnabled(false);
    }
    else
    {
        backbtn->setEnabled(true);
    }
}
void mywindow::showmultiweb(bool emptyflag,QStringList pathlist)
{
    cout<<emptyflag;
    int ttt=0;
    int length=succnum.length()-pagenum;//获取后退的步数
    //for (int z=0;z<succnum.length()-flag;z++)//每一次remove都会造成succnum.length()的改变
    for (int z=0;z<length;z++)//删除flag之后的所有元素 本次搜索将会从flag开始
    {
        cout<<++ttt;//输出计数
        succnum.removeLast();
        inputedit->nowonsearchlist.removeLast();
    }
    cout<<succnum;
    cout<<QStringLiteral("复位");
    inputedit->enterflag=1;//参考值复位 textchange中允许实例化listview
    connect(inputedit,&searchwidget::textChanged,inputedit,&searchwidget::letchanged);
    QString str;//接收写入父页面的文本
    if(emptyflag==0)//首先判断 搜索线程内是否有结果
    {//******************************************************************************************************
        wpage->blockSignals(false);
        //解除屏蔽
        templist.clear();
        for (int i=0;i<pathlist.length();i++)
        {
            QRegExp rx("(\\w*)/");//筛选文本对象
            QString dictname;
            if( rx.indexIn(pathlist[i], 0) != -1) //返回首次匹配出现的位置 若为-1 则没有匹配 参数2为搜索模式 0表示从头搜索
            {
               dictname=rx.cap(1);//获取第一个括号内抓取的内容
            }
            if(i==0)
            {
                str.append(QString("<iframe name=\"%1\" height=\"191px\" id=\"myframe\" src=\"%2\" width=\"100%\" frameborder=\"1\" scrolling=\"no\"></iframe>").arg(dictname).arg(pathlist[i]));
            }//
            else {
                str.append(QString("<iframe name=\"%1\" onload=\"this.height=this.contentWindow.document.body.scrollHeight\" id=\"myframe\" src=\"%2\" width=\"100%\" frameborder=\"1\" scrolling=\"no\"></iframe>").arg(dictname).arg(pathlist[i]));
            }
            //需要跨域 要禁用页面安全
            templist.append(dictname);//获取当前使用词典的名称
        }
        dictnum=templist.length();
        succnum.append(QString::number(dictnum));//将本次成功页面数添加到列表中
    }
    else
    {
        str=QString(QStringLiteral("<b>词库中没有你想要的词条</b>")).toUtf8();
        //******************************************************************************************************
        wpage->blockSignals(true);
        //屏蔽webengine信号 避免loadfinish触发timethread中的tend函数 造成崩溃（关闭未开始的计时器）
        succnum.append("0");
    }

    //将字符串写入文件
    QString temppath=QString("temp%1.html").arg(pagenum);
    cout<<temppath;
    QFile file(temppath);
    //pagenum++;不该在此处增一
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(str.toUtf8());
        file.close();
    }

    //直接使用HTML文件渲染 这样的方法比setHTML要快
    wpage->load(QUrl("file:///"+temppath));
    //file+3×/必须加上 否则不能显示
    pagenum++;//在加载完毕后增一
    //参数1 HTML语句 参数2 外部样式表 注意：必须去除str头部的link语句 必须加file:///
    //=====================================================================
    showxml("wordsh.xml",1);
    //更新搜索历史
    if(emptyflag==0)
    {
        showinfosource();
    }
    cout<<pagenum;
    cout<<succnum;
    judgeposi();
}
void mywindow::showinfosource()
{
//    cout<<list;
//    QStringListModel model;
//    model.setStringList(templist);
//    ui->infosource->setModel(&model);

//model必须使用全局变量 否则无法显示

    cout<<templist;
    dictmodel=new QStringListModel();
    dictmodel->setStringList(templist);
    ui->infosource->setModel(dictmodel);
}
mywindow::~mywindow()
{
    delete ui;
}
void mywindow::showxml(QString filename,int mode)//mode1 history；mode2 collection；
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        bool succ=doc.setContent(&file);
        file.close();
        if(succ)
        {
            QStringList list=dx.readelement(doc);//readelement参数是文档对象
            cout<<list<<filename;
            Model=new QStringListModel(list);
            switch (mode)
            {
            case 1: ui->lvhistory->setModel(Model);break;
            case 2:ui->lvcollection->setModel(Model);break;
            }
        }
        else {
            cout<<filename<<"set content failed in mywindow";
        }
    }
    else
    {
        cout<<filename<<"open failed in mywindow";
    }
}
void mywindow::getMessage(const QString &message)//displaymessage函数被改为getmessage 不在输入框内显示连接完成提醒
{//处理web端发出的字符串 该函数处理超链接
    cout<<message;
    QString head;
    QString body;
    QRegExp rx("(.*)://(.*)");//正则表达式
    if( rx.indexIn(message, 0) != -1) //返回首次匹配出现的位置 若为-1 则没有匹配 参数2为搜索模式 表示从头搜索
    {
        head= rx.cap(1);
        body=rx.cap(2);
        cout<<head<<"====="<<body;
    }
    if(head=="sound")
    {
        emit audio(body);
    }
    else if(head=="bword")
    {
        cout<<"now searching"<<body;
        if(body[0]!="#")
        {
            disconnect(inputedit,&searchwidget::textChanged,0,0);
            inputedit->setText(body);

            //inputedit->judgeempty(body);
            inputedit->judgesame(body);//超链接中存在的内容是字典中一定有的 没有必要judgeempty
        }
        else
        {
            cout<<"position";
        }
    }
}

void mywindow::paintsignal(int color)//状态栏桥接函数
{
    cout<<color<<"==="<<dictnum;
    if(color!=1)
    {
        dictnum--;//转为就绪状态时 必须满足该条件才会执行：6本词典查询到结果 六次连接成功信号才能转换
        if(dictnum==0)
        {
            statu->changestat(color);
        }
    }
    else {
        statu->changestat(color);
    }

}
void mywindow::showmenu(const QPoint &pos)//显示右键菜单
{
    int w=ui->widget->width();
    //cout<<ui->lvhistory->geometry();//得知geometry错误
    QPoint p=QCursor::pos();
    int x=p.rx();
    p.setX(x-w);
    //此处lvhistory->geometry获取的是相对于其所在的上下分裂布局的位置信息 QCursor::pos()在mapFromGlobal转换后获取的是相对于mywindow左上角的坐标 需要转换
    //主要变化是x轴 减去widget组件的宽度即可
    //    cout<<this->mapFromGlobal(p);
    //    if((ui->lvhistory->geometry().contains(this->mapFromGlobal(p))))
    //    {cout<<"a";}
    //    if((ui->lvcollection->geometry().contains(this->mapFromGlobal(p))))
    //    {cout<<"b";}
    QListView *temp;//必须也要是指针
    QString filename;
    int model;//用于区分两个表 修改后重新显示修改完毕的表
    if(ui->lvhistory->geometry().contains(this->mapFromGlobal(p)))//如果在lvhistory范围内点击  则对lvhistory进行操作
    {
        temp=ui->lvhistory;
        filename="wordsh.xml";
        model=1;
    }
    else
    {
        temp=ui->lvcollection;
        filename="wordsc.xml";
        model=2;
    }
    //为历史列表添加菜单
    if(!((temp->selectionModel()->selectedIndexes()).empty()))//如果右键所在地方不是空
    {
        rmenu=new QMenu(temp);//新建菜单对象
        delitem=rmenu->addAction(QStringLiteral("删除"));//菜单中添加action
        connect(delitem,&QAction::triggered,//删除键被触发后
                [=]
        {
            int row=temp->currentIndex().row();//获取当前对象行数
            dx.delele(filename,row);//删除对应xml内的元素
            showxml(filename,model);//重新显示xml内容
        });
        connect(rmenu,&QMenu::destroyed,[=]{cout<<"menu deleted"<<s++;});//确认对象已被删除
        connect(rmenu,&QMenu::aboutToHide,[=]{rmenu->deleteLater();});//delete失败 deletelater成功
        //在鼠标当前位置执行
        rmenu->exec(QCursor::pos());//开始运行菜单对象
    }
}
void mywindow::dealselectstr(const QString &str)
{//处理web端发出的字符串 该函数处理双击选中
    //cout<<"shuangji";
    //wpage->selectflag=0;
    if(str!="")
    {
        cout<<str;
        QString selectedstr=str;//必须要经过这样的转换 才能执行replace
        //selectedstr.replace(" ","");//把最后的空格 替换为null---消除空格
        QRegExp rx("([a-z]+)");
        if( rx.indexIn(selectedstr, 0) != -1) //返回首次匹配出现的位置 若为-1 则没有匹配 参数2为搜索模式 表示从头搜索
        {
            selectedstr=rx.cap(1);//取出单词部分 否则形似 ˈmotor (单引号开头）搜索不到内容
            disconnect(inputedit,&searchwidget::textChanged,0,0);
            inputedit->setText(selectedstr);

            inputedit->judgeempty(selectedstr);
        }
        else
        {
            cout<<"not a word";
        }
    }
}
void mywindow::activated()
{
    cout<<"shortcut activated";
    QClipboard *board = QApplication::clipboard();//使用 QApplication::clipboard() 函数获得系统剪贴板对象。这个函数的返回值是 QClipboard 指针。
    //board->setText("Text from Qt Application"); //通过 setText()，setImage() 或者 setPixmap() 函数可以将数据放置到剪贴板内，对应文本 图片
    QString sword = board->text(); //使用text()，image() 或者 pixmap() 函数则可以从剪贴板获得数据，对应文本 图片。
    inputedit->judgeempty(sword);
    cout<<"settext";

    disconnect(inputedit,&searchwidget::textChanged,0,0);//断开二者信号连接
    inputedit->setText(sword);
    cout<<sword;
}
