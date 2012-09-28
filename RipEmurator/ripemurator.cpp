#include "ripemurator.h"
#include "ripcore.h"
#include "rippackage.h"
#include "rippackagelistener.h"
RipEmurator::RipEmurator(QWidget *parent) :
	QMainWindow(parent) {
	ui.setupUi(this);

	model = new RipInfosModel( this);
	core = new RipCore();
	listener = new RipPackageListener();

	ui.TblRipInfo->setModel(model);


	connect(ui.BtnStart, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui.BtnStop, SIGNAL(clicked()), this, SLOT(stop()));

	connect(ui.chkRecord, SIGNAL(clicked(bool)), this, SLOT(setRecored(bool)));

	connect(ui.chkPoisonReverse, SIGNAL(clicked(bool)), core, SLOT(setPoisonReverse(bool)));
	connect(ui.chkHorizonSplit, SIGNAL(clicked(bool)), core, SLOT(setSplitHorizon(bool)));




	connect(core, SIGNAL(sendPackage(RipPackage& )) ,
			listener, SLOT(sendPackage(RipPackage& )));
	connect(listener,SIGNAL(received(RipPackage )),
			core,SLOT(receive(RipPackage )));

	connect(listener,SIGNAL(report(QString , QString )),
			this, SLOT(report(QString , QString )));
	connect(core,SIGNAL(report(QString , QString )),
			this, SLOT(report(QString , QString )));

	connect(core,SIGNAL(show(QList<RowInfo> )),
			this, SLOT(refreshStatus(QList<RowInfo> )));

}
RipEmurator::~RipEmurator()
{
	delete core;
	delete listener;
	delete model;
}

const QString RipEmurator::REPORT_ERR = "<font face='Verdana' color='red' >";
const QString RipEmurator::REPORT_NORMAL = "<font face='Times' color='black'>";
const QString RipEmurator::REPORT_IMPORTANT = "<font face='Verdana' color='blue'>";


void RipEmurator::report(QString msg, QString kind)
{
	if(record)
	{
		ui.TxtHistory->insertHtml("<p>"+ kind + msg + "</font></p><br/>");
		ui.TxtHistory->append("\n");
	}
}

void RipEmurator::refreshStatus(QList<RowInfo> infos)
{
	model->setModel(infos);
}


void RipEmurator::start()
{

	report("hello");

	qDebug()<<"Hello";
	ui.BtnStart->setEnabled(false);




	if(listener->startListen())
	{
		report("Network listener started!");
		core->start( listener );
		ui.BtnStop->setEnabled(true);

	}else
	{
		ui.BtnStart->setEnabled(true);
		report("Network listener start failed!", REPORT_ERR);
	}
}
void RipEmurator::stop()
{
	ui.BtnStop->setEnabled(false);
	ui.BtnStart->setEnabled(true);
	listener->stopListen();
	core->stop();
}
