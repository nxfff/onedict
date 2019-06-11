/********************************************************************************
** Form generated from reading UI file 'mywindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWINDOW_H
#define UI_MYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mywindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QWidget *widget;
    QSplitter *splitter;
    QLabel *label;
    QListView *infosource;
    QLabel *label_2;
    QListView *lvcollection;
    QLabel *label_3;
    QListView *lvhistory;
    QMenuBar *menuBar;
    QToolBar *toolbar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mywindow)
    {
        if (mywindow->objectName().isEmpty())
            mywindow->setObjectName(QStringLiteral("mywindow"));
        mywindow->resize(1232, 762);
        centralWidget = new QWidget(mywindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        widget = new QWidget(splitter_2);
        widget->setObjectName(QStringLiteral("widget"));
        splitter_2->addWidget(widget);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        label = new QLabel(splitter);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(0, 25));
        label->setMaximumSize(QSize(225, 25));
        splitter->addWidget(label);
        infosource = new QListView(splitter);
        infosource->setObjectName(QStringLiteral("infosource"));
        infosource->setMinimumSize(QSize(225, 0));
        infosource->setMaximumSize(QSize(225, 16777215));
        splitter->addWidget(infosource);
        label_2 = new QLabel(splitter);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(0, 25));
        label_2->setMaximumSize(QSize(225, 25));
        splitter->addWidget(label_2);
        lvcollection = new QListView(splitter);
        lvcollection->setObjectName(QStringLiteral("lvcollection"));
        lvcollection->setMinimumSize(QSize(225, 0));
        lvcollection->setMaximumSize(QSize(225, 16777215));
        splitter->addWidget(lvcollection);
        label_3 = new QLabel(splitter);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(0, 25));
        label_3->setMaximumSize(QSize(225, 25));
        splitter->addWidget(label_3);
        lvhistory = new QListView(splitter);
        lvhistory->setObjectName(QStringLiteral("lvhistory"));
        lvhistory->setMinimumSize(QSize(225, 0));
        lvhistory->setMaximumSize(QSize(225, 16777215));
        splitter->addWidget(lvhistory);
        splitter_2->addWidget(splitter);

        gridLayout->addWidget(splitter_2, 0, 0, 1, 1);

        mywindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mywindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1232, 25));
        mywindow->setMenuBar(menuBar);
        toolbar = new QToolBar(mywindow);
        toolbar->setObjectName(QStringLiteral("toolbar"));
        mywindow->addToolBar(Qt::TopToolBarArea, toolbar);
        statusBar = new QStatusBar(mywindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mywindow->setStatusBar(statusBar);

        retranslateUi(mywindow);

        QMetaObject::connectSlotsByName(mywindow);
    } // setupUi

    void retranslateUi(QMainWindow *mywindow)
    {
        mywindow->setWindowTitle(QApplication::translate("mywindow", "mywindow", nullptr));
        label->setText(QApplication::translate("mywindow", "\346\225\260\346\215\256\346\235\245\346\272\220", nullptr));
        label_2->setText(QApplication::translate("mywindow", "\346\224\266\350\227\217", nullptr));
        label_3->setText(QApplication::translate("mywindow", "\345\216\206\345\217\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mywindow: public Ui_mywindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWINDOW_H
