#ifndef RIPEMURATOR_H
#define RIPEMURATOR_H

#include <QtGui/QMainWindow>

#include "ui_ripemurator.h"
#include "ripinfosmodel.h"

class RipCore;
class RipPackageListener;
class RipEmurator : public QMainWindow
{

    Q_OBJECT

public:
    static const QString REPORT_ERR;
    static const QString REPORT_NORMAL;
    static const QString REPORT_IMPORTANT;

    RipEmurator(QWidget *parent = 0);
    ~RipEmurator();

public slots:
    void report(QString msg, QString kind = REPORT_NORMAL);
    void refreshStatus(QList<RowInfo> infos);
    void start();
    void stop();

    void setRecored(bool record){this->record = record;}
private:

    bool record;

    Ui::RipEmuratorClass ui;
    RipInfosModel * model;

    RipCore * core;
    RipPackageListener * listener;
};

#endif // RIPEMURATOR_H
