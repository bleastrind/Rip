/********************************************************************************
** Form generated from reading ui file 'ripemurator.ui'
**
** Created: Wed Jun 9 00:02:29 2010
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_RIPEMURATOR_H
#define UI_RIPEMURATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RipEmuratorClass
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *BtnStart;
    QPushButton *BtnStop;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *chkPoisonReverse;
    QCheckBox *chkHorizonSplit;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTableView *TblRipInfo;
    QWidget *tab_2;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *chkRecord;
    QPushButton *btnClear;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTextEdit *TxtHistory;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RipEmuratorClass)
    {
    if (RipEmuratorClass->objectName().isEmpty())
        RipEmuratorClass->setObjectName(QString::fromUtf8("RipEmuratorClass"));
    RipEmuratorClass->resize(562, 367);
    RipEmuratorClass->setMinimumSize(QSize(562, 367));
    RipEmuratorClass->setMaximumSize(QSize(562, 367));
    centralwidget = new QWidget(RipEmuratorClass);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayoutWidget = new QWidget(centralwidget);
    verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
    verticalLayoutWidget->setGeometry(QRect(20, 10, 521, 331));
    verticalLayout = new QVBoxLayout(verticalLayoutWidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 3);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    BtnStart = new QPushButton(verticalLayoutWidget);
    BtnStart->setObjectName(QString::fromUtf8("BtnStart"));
    BtnStart->setStyleSheet(QString::fromUtf8(""));

    horizontalLayout->addWidget(BtnStart);

    BtnStop = new QPushButton(verticalLayoutWidget);
    BtnStop->setObjectName(QString::fromUtf8("BtnStop"));
    BtnStop->setEnabled(false);

    horizontalLayout->addWidget(BtnStop);


    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalLayout_2->setContentsMargins(-1, 0, 0, -1);
    chkPoisonReverse = new QCheckBox(verticalLayoutWidget);
    chkPoisonReverse->setObjectName(QString::fromUtf8("chkPoisonReverse"));
    chkPoisonReverse->setChecked(true);

    horizontalLayout_2->addWidget(chkPoisonReverse);

    chkHorizonSplit = new QCheckBox(verticalLayoutWidget);
    chkHorizonSplit->setObjectName(QString::fromUtf8("chkHorizonSplit"));
    chkHorizonSplit->setChecked(true);

    horizontalLayout_2->addWidget(chkHorizonSplit);


    verticalLayout->addLayout(horizontalLayout_2);

    tabWidget = new QTabWidget(verticalLayoutWidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    TblRipInfo = new QTableView(tab);
    TblRipInfo->setObjectName(QString::fromUtf8("TblRipInfo"));
    TblRipInfo->setGeometry(QRect(0, 0, 511, 251));
    TblRipInfo->setStyleSheet(QString::fromUtf8("QTableView {\n"
"	selection-background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0.739, stop:0 rgba(65, 165, 231, 255), stop:1 rgba(255, 255, 255, 255));\n"
"    padding:2px;\n"
"}\n"
""));
    tabWidget->addTab(tab, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    verticalLayoutWidget_2 = new QWidget(tab_2);
    verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
    verticalLayoutWidget_2->setGeometry(QRect(0, 0, 521, 271));
    verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    horizontalLayout_3->setContentsMargins(-1, 0, -1, -1);
    chkRecord = new QCheckBox(verticalLayoutWidget_2);
    chkRecord->setObjectName(QString::fromUtf8("chkRecord"));
    chkRecord->setChecked(true);

    horizontalLayout_3->addWidget(chkRecord);

    btnClear = new QPushButton(verticalLayoutWidget_2);
    btnClear->setObjectName(QString::fromUtf8("btnClear"));

    horizontalLayout_3->addWidget(btnClear);


    verticalLayout_2->addLayout(horizontalLayout_3);

    scrollArea = new QScrollArea(verticalLayoutWidget_2);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 517, 236));
    TxtHistory = new QTextEdit(scrollAreaWidgetContents);
    TxtHistory->setObjectName(QString::fromUtf8("TxtHistory"));
    TxtHistory->setGeometry(QRect(0, 0, 521, 271));
    TxtHistory->setReadOnly(true);
    scrollArea->setWidget(scrollAreaWidgetContents);

    verticalLayout_2->addWidget(scrollArea);

    tabWidget->addTab(tab_2, QString());

    verticalLayout->addWidget(tabWidget);

    RipEmuratorClass->setCentralWidget(centralwidget);
    statusbar = new QStatusBar(RipEmuratorClass);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    RipEmuratorClass->setStatusBar(statusbar);

    retranslateUi(RipEmuratorClass);
    QObject::connect(btnClear, SIGNAL(clicked()), TxtHistory, SLOT(clear()));

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(RipEmuratorClass);
    } // setupUi

    void retranslateUi(QMainWindow *RipEmuratorClass)
    {
    RipEmuratorClass->setWindowTitle(QApplication::translate("RipEmuratorClass", "MainWindow", 0, QApplication::UnicodeUTF8));
    BtnStart->setText(QApplication::translate("RipEmuratorClass", "Start", 0, QApplication::UnicodeUTF8));
    BtnStop->setText(QApplication::translate("RipEmuratorClass", "Stop", 0, QApplication::UnicodeUTF8));
    chkPoisonReverse->setText(QApplication::translate("RipEmuratorClass", "PoisonReverse", 0, QApplication::UnicodeUTF8));
    chkHorizonSplit->setText(QApplication::translate("RipEmuratorClass", "HorizonSplit", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("RipEmuratorClass", "RipInfo", 0, QApplication::UnicodeUTF8));
    chkRecord->setText(QApplication::translate("RipEmuratorClass", "Record", 0, QApplication::UnicodeUTF8));
    btnClear->setText(QApplication::translate("RipEmuratorClass", "Clear", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("RipEmuratorClass", "History", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(RipEmuratorClass);
    } // retranslateUi

};

namespace Ui {
    class RipEmuratorClass: public Ui_RipEmuratorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RIPEMURATOR_H
