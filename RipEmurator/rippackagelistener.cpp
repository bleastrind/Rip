/*
 * rippackagelistener.cpp
 *
 *  Created on: 2010-3-14
 *      Author: Bleastrind
 */

#include "rippackagelistener.h"

#include <QtNetwork/QHostInfo>
#include "assert.h"
#include "winpcapsender.h"
#include "rawsender.h"


namespace {
const unsigned char RIP_COMMAND_REQUEST = 1;
const unsigned char RIP_COMMAND_RESPONSE = 2;


#ifdef WIN32
const unsigned short RIP_FAMILY_IP = 0x0200;


#endif
#define NOT_IMPLY_IN_THE_OS 0


typedef struct _RIPHEADER {
	//  Command(1)   |  Version(1)   |   must be zero(2)
	unsigned char command;
	unsigned char version;
	unsigned short _reserve1;


} RIPHEADER;
typedef struct _RIPITEM {

	// Address family identifier(2)  |   must be zero(2)
	unsigned short af_id;
	unsigned short _reserve2;

	// IP address(4)
	unsigned long ip_addr;

	// Must be zero(4)
	unsigned long _reserve3;
	// Must be zero(4)
	unsigned long _reserve4;

	//  Metric(4)
	unsigned short _reserve5;
	unsigned char _reserve6;
	unsigned char metric;
} RIPITEM;

void endingChange(char* integer)
{
	integer[0] ^= integer[3];
	integer[3] ^= integer[0];
	integer[0] ^= integer[3];

	integer[1] ^= integer[2];
	integer[2] ^= integer[1];
	integer[1] ^= integer[2];
}

RipPackage bytesToPackage(char bytes[], qint64 len) {
	RIPHEADER * header = (RIPHEADER *)bytes;
	RIPITEM * item = (RIPITEM*)( bytes + sizeof(RIPHEADER));
	RipPackage package;


	len -= sizeof(RIPHEADER);
	//check
	if(header->version != 1 ||   		// RIP version 1 only
			header->_reserve1  || 		// must be 0
			len  <= 0 ||  				//have body
			len % sizeof(RIPITEM) != 0 )//aligned
	{
		package.command = Bad;
		return package;
	}

	//Command
	if(header->command == 1)
		package.command = Request;
	else if (header->command == 2)
		package.command = Response;
	else
	{
		package.command = Bad;
		return package;
	}

	//body
	while(len)
	{
		len -= sizeof(RIPITEM);
		if( (item->_reserve2 | item->_reserve3 | item->_reserve4 | item->_reserve5 | item->_reserve6)||	// must be 0
			//item->af_id != RIP_FAMILY_IP|| 		// IP protocol only
			item->metric > 16)		// can't bigger than 16
		{
			package.command = Bad;
			return package;
		}
#ifdef WIN32
		endingChange((char *)&item->ip_addr);
#else
		assert(NOT_IMPLY_IN_THE_OS);
#endif
		RipItemInfo ritem(QHostAddress(item->ip_addr) , item->metric);

		item++;

		qDebug()<<ritem.ip.toString();
		package.items.append(ritem);
	}
	return package;
}

qint64 packageToBytes(RipPackage & package, char bytes[])
{
	qint64 size = sizeof(RIPHEADER);
	RIPHEADER * header = (RIPHEADER *)bytes;
	header->_reserve1 = 0;

	header->command = package.command;
	header->version = 1;


	RIPITEM * item = (RIPITEM*)( bytes + sizeof(RIPHEADER));
	foreach(RipItemInfo ritem , package.items)
	{
		item->_reserve3 = item->_reserve4 = 0;
		item->_reserve5 = item->_reserve2 = 0;
		item->_reserve6 = 0;
		item->af_id = (package.command == Request && ritem.ip == QHostAddress::Any) ? 0 : RIP_FAMILY_IP;
		item->ip_addr = ritem.ip.toIPv4Address();
#ifdef WIN32
		endingChange((char *)&item->ip_addr);
#else
		assert(NOT_IMPLY_IN_THE_OS);
#endif
		item->metric = ritem.metric > 16 ? 16 : ritem.metric;

		size += sizeof(RIPITEM);
		item++;
	}
	return size;
}
}

RipPackageListener::~RipPackageListener() {
	stopListen();

	foreach(UdpHandler* handler, udpSocketHandlers)
			delete handler;
}

bool RipPackageListener::startListen() {
	QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());


	foreach(QHostAddress address, info.addresses()) //在in.addresses()找主机的地址
		{

			if (address.protocol() != QAbstractSocket::IPv4Protocol)
				continue;

			QUdpSocket* udpsocket = new QUdpSocket( );

			//record the socket
			UdpHandler * handler = new UdpHandler(address, udpsocket);
			udpSocketHandlers.append(handler);

			if (!udpsocket->bind(address, RIP_PORT)) {
				err("Interface: " + address.toString() + " UDP bind failed!");
				return false;
			}

			connect(udpsocket, SIGNAL(readyRead()), handler, SLOT(recv_slot()));
			connect(handler, SIGNAL(receive(RipPackage )),
					this, SLOT(recieveudp(RipPackage )));


			msg("Interface: " + address.toString() + " Is Listening!");
		}

	return true;
}

void RipPackageListener::stopListen() {
	foreach(UdpHandler* handler, udpSocketHandlers)
		{
			handler->stopSocket();
			delete handler;
		}
	udpSocketHandlers.clear();
	msg("Listening stopped!");

}

bool checkIn(QHostAddress address,QList<UdpHandler *> &handlers)
{

	foreach(UdpHandler* handler, handlers)
	{
		if(handler->interface == address)
			return true;
	}
	return false;
}

QList<QHostAddress> RipPackageListener::getInterfaces()
{
	QList<QHostAddress> ans;
	QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());


	foreach(QHostAddress address, info.addresses()) //在in.addresses()找主机的地址
		{

			if (address.protocol() != QAbstractSocket::IPv4Protocol)
				continue;
			ans.append(address);


			if(checkIn(address,udpSocketHandlers))
				continue;

			QUdpSocket* udpsocket = new QUdpSocket( );

			//record the socket
			UdpHandler * handler = new UdpHandler(address, udpsocket);
			udpSocketHandlers.append(handler);

			if (!udpsocket->bind(address, RIP_PORT)) {
				err("Interface: " + address.toString() + " UDP bind failed!");
				continue;
			}

			connect(udpsocket, SIGNAL(readyRead()), handler, SLOT(recv_slot()));
			connect(handler, SIGNAL(receive(RipPackage )),
					this, SLOT(recieveudp(RipPackage )));


			msg("Interface: " + address.toString() + " Is Listening!");
		}


	return ans;
}


void RipPackageListener::sendPackage(RipPackage &package) {
	char buffer[512];



	qint64 size = packageToBytes(package, buffer);

	QUdpSocket* pUdpsocket;
	pUdpsocket = new QUdpSocket();
	bool newbind = pUdpsocket->bind(package.interface
			, RIP_PORT);

	qDebug()<<newbind;

	if(!newbind){

		delete pUdpsocket;
		pUdpsocket = udpSocketHandlers.first()->socket;

		foreach(UdpHandler* h,udpSocketHandlers)
		{
			if(h->interface == package.interface)
				pUdpsocket = h->socket;
		}
	}


	qDebug()<<package.target.toString();



#ifdef QTSCOKETONLY
	if( pUdpsocket->writeDatagram(buffer, size, package.target, RIP_PORT) == -1)
		err("Package send to "+ package.target.toString()+" is failed at "+package.interface.toString());
	else
		msg("Package sent to " + package.target.toString()+" at "+package.interface.toString());
#else
	if( ::rawsendudp(package.interface.toIPv4Address(),buffer,size,package.target.toIPv4Address(),RIP_PORT)== -1)
		err("Package send to "+ package.target.toString()+" is failed at "+package.interface.toString());
	else
		msg("Package sent to " + package.target.toString()+" at "+package.interface.toString());
#endif
}
void RipPackageListener::recieveudp(RipPackage package) {

	msg("Msg Recieved from " + package.sender.toString());

	qDebug()<<package.interface.toString();
	qDebug()<<package.sender.toString();
	emit received(package);
}

void UdpHandler::recv_slot() {

	char buffer[512];

	QHostAddress from;
	qint64 size = socket->readDatagram(buffer, 512,&from);

	qDebug()<< from.toString();

	RipPackage package = bytesToPackage(buffer, size);
	package.sender = from;
	package.interface = interface;
	emit receive(package);
}
