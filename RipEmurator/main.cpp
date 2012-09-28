#include "ripemurator.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	qDebug()<<"Begin";
    QApplication a(argc, argv);
    RipEmurator w;
    w.show();
    return a.exec();
}

void NetWorkTest()
{

}
