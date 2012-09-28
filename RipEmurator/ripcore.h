/*
 * ripcore.h
 *
 *  Created on: 2010-3-14
 *      Author: Bleastrind
 */

#ifndef RIPCORE_H_
#define RIPCORE_H_

#include <QtCore/QtCore>
#include "rippackage.h"
#include "RipEmurator.h"
#include "ripinfosmodel.h"
#include "rippackagelistener.h"
struct RouteItem;

class RipCore:public QObject
{
	Q_OBJECT
public:

	RipCore(){ispoisonReverse = true;splitHorizon=true;}

	static const int INTERVAL = 3000; //3s
	static const int DISCARDMISS = 10;
	static const int FARMISS = 4;

	void start(RipPackageListener * listener);
	void start();
	void stop();
	void triggerUpdate(QHostAddress &);

	void checkInterfaces();
	void routineWork();
public slots:
	void taskDispatcher();

	void receive(RipPackage package);


	void setPoisonReverse(bool poisonreverse){ ispoisonReverse = poisonreverse; }
	void setSplitHorizon(bool splithorizon){ splitHorizon = splithorizon; }


	signals:
	void sendPackage(RipPackage& package);
	void show(QList<RowInfo> infos);
	void report(QString msg, QString kind);
private:
	void err(QString msg){emit report(msg,RipEmurator::REPORT_ERR);}
	void msg(QString msg){emit report(msg,RipEmurator::REPORT_NORMAL);}

	int timercounter;
	bool ispoisonReverse;
	bool splitHorizon;

	QList<RouteItem> routeTable;
	QList<QHostAddress> localInterfaces;

	QTimer * timer;
	RipPackageListener * listener;
};

struct RouteItem
{
public:
	RouteItem(QHostAddress target,
			QString next,
			QHostAddress provider,
			unsigned char metric = 0,
			int miss = 0)
			:target(target),
			 next(next),
			 provider(provider),
			 metric(metric),
			 miss(miss)
	{}
	QHostAddress target;
	QString next;
	unsigned char metric;
	int miss;					//For check link
	QHostAddress provider;		//For horizon split
};

#endif /* RIPCORE_H_ */
