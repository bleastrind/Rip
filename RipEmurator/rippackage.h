/*
 * rippackage.h
 *
 *  Created on: 2010-3-14
 *      Author: Bleastrind
 */

#ifndef RIPPACKAGE_H_
#define RIPPACKAGE_H_

#include <QtCore/QtCore>
#include <QtNetwork/QHostAddress>

struct RipItemInfo;

enum RipCommand{ Bad,Request,Response};

struct RipPackage
{
public:
	RipPackage(QHostAddress target = QHostAddress::Broadcast)
	:target(target){}
	RipCommand command;
	QHostAddress target;
	QHostAddress sender;
	QList<RipItemInfo> items;


	//For local use, not datagram
	QHostAddress interface;
};

struct RipItemInfo
{
public:
	RipItemInfo(QHostAddress ip,unsigned char metric)
	:ip(ip),metric(metric){}
	QHostAddress ip;
	unsigned char metric;
};
#endif /* RIPPACKAGE_H_ */
