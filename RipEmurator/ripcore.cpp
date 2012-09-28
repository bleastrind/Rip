/*
 * ripcore.cpp
 *
 *  Created on: 2010-3-14
 *      Author: Bleastrind
 */
#include "ripcore.h"

#include <QtCore/QTimer>
namespace {
const QString DIRECT = "LoopBack";


QHostAddress getNet(QHostAddress & addr)
{

	static quint32 maskA = 0xff000000;
	static quint32 maskB = 0xffff0000;
	static quint32 maskC = 0xffffff00;

	quint32 data = addr.toIPv4Address();
	if(data >> 31 == 0) //A类
		return QHostAddress(data & maskA);
	else if(data >> 30 == 2)//B类
		return QHostAddress(data & maskB);
	else if(data >> 29 == 6)//C类
		return QHostAddress(data & maskC);
	else
		return QHostAddress( data);
}


int getRouteD(QList<RouteItem> &routeTable, QHostAddress address , QList<QHostAddress> &localInterfaces) {
	QList<RouteItem>::iterator pitems = routeTable.begin();
	for (int i =0; i < routeTable.count(); i++)
	{
		qDebug()<<pitems[i].target.toString();
		qDebug()<<address.toString();
		if (getNet(pitems[i].target) == getNet(address))
		{
			//pitems[i].miss = 0;
			return i;
		}
	}

	foreach(QHostAddress addr, localInterfaces)
	{
		if(getNet(addr) == getNet(address))
			return -2;
	}
	return -1;
}

bool deleteDR(QList<RouteItem> &routeTable, QHostAddress address,
		QString next) {

	QList<RouteItem>::iterator i = routeTable.begin();
	for (; i != routeTable.end(); i++) {
		qDebug()<<i->target.toString();
		qDebug()<<address.toString();
		qDebug()<<next;
		qDebug()<<i->next;

		if (getNet(i->target) == getNet(address) && i->next == next) {
			routeTable.erase(i);
			return true;
		}
	}
	return false;
}

bool deleteDM(QList<RouteItem> &routeTable, QHostAddress address,
		unsigned char metric) {
	QList<RouteItem>::iterator i = routeTable.begin();
	for (; i != routeTable.end(); i++) {
		if (getNet(i->target) == getNet(address) && i->metric > metric) {
			routeTable.erase(i);
			return true;
		}
	}
	return false;
}


QList<RowInfo> genInfos(QList<RouteItem> &routeTable, QList<QHostAddress> & localInterfaces )
{
	QList<RowInfo> infos;
	foreach(RouteItem item, routeTable)
	{
		RowInfo info;
		info.Interface = item.provider.toString();
		info.Metric = item.metric;
		info.Next = item.next;
		info.Target = item.target.toString();
		infos.append(info);
	}

	foreach(QHostAddress addr, localInterfaces)
	{
		RowInfo info;
		info.Interface = QHostAddress(QHostAddress::LocalHost).toString();
		info.Metric = 0;
		info.Next = DIRECT;
		info.Target = getNet(addr).toString();
		infos.append(info);
	}
	return infos;
}

}//endnamespace

void RipCore::receive(RipPackage package) {
	switch (package.command ) {
	case Response:
	{
		msg("Recived response from "+ package.sender.toString());
		foreach(RipItemInfo info, package.items)
			{
				QHostAddress &D = info.ip;
				QString R = package.sender.toString();
				unsigned char & M = info.metric;

				msg("D:"+D.toString() +" M:"+QString::number(M,10)+" R:"+R);
				qDebug()<<D.toString();
				qDebug()<<R;

				//No route to ip
				if (getRouteD(routeTable, D , localInterfaces) == -1) {
					//add  (D,M,R)
					routeTable.append(RouteItem(D, R, package.interface,M  ,  0));
				}
				//From D R
				else if (deleteDR(routeTable, D, R)) {
					routeTable.append(RouteItem(D, R, package.interface,M  , 0));
				} else if (deleteDM(routeTable, D, M)) {
					routeTable.append(RouteItem(D, R,package.interface, M , 0));
				}
			}

		break;
	}
	case Request:
	{
		//发送报文

		RipPackage newpackage(package.sender);
		newpackage.command = Response;
		newpackage.sender = package.interface;
		newpackage.interface = package.interface;

		foreach(RipItemInfo i , package.items)
		{
			int index = -1;
			if (i.ip == QHostAddress::Any)
			{
				foreach(RouteItem item , routeTable)
				{
					newpackage.items.append(RipItemInfo(item.target, item.metric+1));
				}
				foreach(QHostAddress addr, localInterfaces)
				{
					newpackage.items.append(RipItemInfo(addr,1));
				}
				break;
			}
			else if(( index = getRouteD(routeTable, i.ip , localInterfaces) ) > 0 )
			{
				newpackage.items.append(
						RipItemInfo(routeTable[index].target,
								routeTable[index].metric+1)
								);
			}else
			{
				foreach(QHostAddress addr, localInterfaces)
				{
					if(addr == i.ip)
						newpackage.items.append(RipItemInfo(getNet(addr),1));
				}
			}

		}

		msg("Sended an package at " + package.interface.toString());
		emit sendPackage(newpackage);

		break;
	}
	default:
		;
	}

	emit show(genInfos(routeTable,localInterfaces));
}

void RipCore::triggerUpdate(QHostAddress &poisonaddr)
{

	foreach(QHostAddress addr , localInterfaces)
	{
		RipPackage package;
		package.command = Response;
		package.sender = addr;
		package.interface = addr;

		qDebug()<<addr.toString();

		package.items.append(RipItemInfo(poisonaddr,16));
		msg("Sended an package at " + addr.toString());
		emit sendPackage(package);
	}

}

void RipCore::taskDispatcher()
{
	//检查本地链接
	checkInterfaces();

	if(!(this->timercounter++ % 10))
		routineWork();
}




void RipCore::routineWork() {



	//超时处理
	QList<RouteItem>::iterator i = routeTable.begin();
	for (; i != routeTable.end(); i++) {
		i->miss++;

		qDebug()<<i->miss;
		if(i->miss > DISCARDMISS)
		{
			msg("Drop net: " + i->target.toString());
			routeTable.erase(i);
		}
		else if( i->miss > FARMISS)
		{
			i->metric = 16;
		}

	}

	//发送报文
	foreach(QHostAddress addr , localInterfaces)
		{
			RipPackage package;
			package.command = Response;
			package.sender = addr;
			package.interface = addr;

			qDebug()<<addr.toString();

			foreach(RouteItem item , routeTable)
				{
					if (splitHorizon && item.provider == addr)
					{
						if(ispoisonReverse)
							package.items.append(RipItemInfo(item.target, 16));
						else
							continue;
					}else
						package.items.append(RipItemInfo(item.target, item.metric + 1));
				}

			foreach(QHostAddress localaddr, localInterfaces)
			{
				if(splitHorizon && localaddr == addr)
				{
					continue;
				}else
					package.items.append(RipItemInfo(getNet(localaddr) ,1));
			}
			msg("Sended an package at " + addr.toString());
			emit sendPackage(package);
		}




}

void RipCore::start(RipPackageListener * l)
{
	this->listener = l;
	timercounter = 1;
	start();
}

void RipCore::checkInterfaces()
{

	QList<QHostAddress> addrs;
	foreach(QHostAddress addr, listener->getInterfaces())
	{
		addrs.append(addr);
	}

	foreach(QHostAddress oldaddr, localInterfaces)
	{
		if(!addrs.contains(oldaddr))
			triggerUpdate(oldaddr);
	}
	localInterfaces.clear();
	localInterfaces = addrs;
}

void RipCore::start() {
	routeTable.clear();
//	localInterfaces.clear();
//	foreach(QHostAddress addr, directs)
//		{
//			//RouteItem routeitem(addr, DIRECT,QHostAddress::LocalHost, 0, 0);
//
//			//routeTable.append(routeitem);
//			localInterfaces.append(addr);
//		}
	checkInterfaces();

	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(taskDispatcher()));
	timer->start(INTERVAL);

	//send request
	foreach(QHostAddress addr , localInterfaces)
	{
		RipPackage newpackage;
		newpackage.command = Request;
		newpackage.sender = addr;
		newpackage.interface = addr;

		newpackage.items.append(RipItemInfo(QHostAddress::Any,16));

		emit sendPackage(newpackage);
	}

	routineWork();
}

void RipCore::stop() {


	routeTable.clear();
	localInterfaces.clear();
	timer->stop();
	delete timer;
}

