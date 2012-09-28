/*
 * rippackagelistener.h
 *
 *  Created on: 2010-3-14
 *      Author: Bleastrind
 */

#ifndef RIPPACKAGELISTENER_H_
#define RIPPACKAGELISTENER_H_


#include <QtCore/QtCore>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include "rippackage.h"
#include "ripemurator.h"

class UdpHandler;
class RipPackageListener:public QObject
{
	Q_OBJECT
public:

	static const int RIP_PORT = 520;


	~RipPackageListener();

	bool startListen();
	void stopListen();

	QList<QHostAddress> getInterfaces();

public slots:
	//from upper level
	void sendPackage(RipPackage& package);
	//from udp level
	void recieveudp(RipPackage package);

	signals:
	// to upper level

	void received(RipPackage package);
	void report(QString msg, QString kind);
private:

	void err(QString msg){emit report(msg,RipEmurator::REPORT_ERR);}
	void msg(QString msg){emit report(msg,RipEmurator::REPORT_IMPORTANT);}


	QList<UdpHandler *> udpSocketHandlers;
};

class UdpHandler:public QObject
{
	Q_OBJECT
public:
	UdpHandler(QHostAddress interface,QUdpSocket* socket)
		:interface(interface),socket(socket){}
	~UdpHandler(){ delete socket;}

	QHostAddress interface;
	QUdpSocket* socket;


	void stopSocket(){socket->close();}
public slots:
	void recv_slot();

	signals:
	void receive(RipPackage);
};


#endif /* RIPPACKAGELISTENER_H_ */
