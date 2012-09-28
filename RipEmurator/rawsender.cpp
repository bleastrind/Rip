
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>  //IP_HDRINCL is here
#include <conio.h>
#include <Winbase.h>
#include <windows.h>
//#pragma comment(lib,"ws2_32.lib")  //winsock 2.2 library

typedef struct ip_hdr
{
    unsigned char  ip_header_len:4;  // 4-bit header length (in 32-bit words) normally=5 (Means 20 Bytes may be 24 also)
    unsigned char  ip_version   :4;  // 4-bit IPv4 version
    unsigned char  ip_tos;           // IP type of service
    unsigned short ip_total_length;  // Total length
    unsigned short ip_id;            // Unique identifier

	unsigned char  ip_frag_offset   :5;        // Fragment offset field

	unsigned char  ip_more_fragment :1;
	unsigned char  ip_dont_fragment :1;
	unsigned char  ip_reserved_zero :1;

    unsigned char  ip_frag_offset1;    //fragment offset

	unsigned char  ip_ttl;           // Time to live
    unsigned char  ip_protocol;      // Protocol(TCP,UDP etc)
    unsigned short ip_checksum;      // IP checksum
    unsigned int   ip_srcaddr;       // Source address
    unsigned int   ip_destaddr;      // Source address
}IPHeader;


// UDP header
typedef struct udp_header
{
	unsigned short source_port;  // source port
	unsigned short dest_port;    // destination port
	unsigned short udp_length;
	unsigned short udp_checksum;
}UDPHeader;

typedef struct pheader
{
	unsigned int src_addr;
	unsigned int dst_addr;
	unsigned char zero;
	unsigned char protocal;
	unsigned short length;
}PseudoUDPHeader;

unsigned short checksum(unsigned short *buffer, int size)
{
	unsigned long cksum=0;
	while (size > 1)
	{
		cksum += * buffer++;
		size -= sizeof(unsigned short);
	}
	if (size)
	{
		cksum += *(unsigned char *)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (unsigned short)(~cksum);
}

SOCKET InitSocket()
{

	SOCKET s;
	//Initialise Winsock
	WSADATA wsock;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsock) != 0)
	{
		fprintf(stderr,"WSAStartup() failed");
        exit(EXIT_FAILURE);
	}
	printf("Initialised successfully.");
	////////////////////////////////////////////////
	int er;
	long flag ;

			//Create Raw TCP Packet
	printf("\nCreating Raw UDP Socket...");
	if((s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("Creation of raw socket failed.");
		return 0;
	}

	er = setsockopt( s, IPPROTO_IP, IP_HDRINCL, (const char *) &flag, sizeof(long));
		if(er == SOCKET_ERROR)
		{
			er = WSAGetLastError();
			printf("Send Error:%d\n",er);
		}
		bool broadcast = true;
		er = setsockopt(s,SOL_SOCKET,SO_BROADCAST,(const char*)&broadcast,sizeof(bool));
		if(er == SOCKET_ERROR)
		{
			er = WSAGetLastError();
			printf("Send Error:%d\n",er);
		}

	printf("Raw UDP Socket Created successfully.");
	////////////////////////////////////////////////

	return s;
}

unsigned short calcUdpCheckSum(char * data,int size,IPHeader * iphdr,UDPHeader *udphdr)
{
	static char temp[1000];
		PseudoUDPHeader * phdr = (PseudoUDPHeader *)temp;
		UDPHeader * uhdr = (UDPHeader *)(temp + sizeof(PseudoUDPHeader));
		phdr->src_addr = iphdr->ip_srcaddr;
		phdr->dst_addr = iphdr->ip_destaddr;
		phdr->zero = 0;
		phdr->protocal = IPPROTO_UDP;
		phdr->length = udphdr->udp_length;
		uhdr->source_port = udphdr->source_port;
		uhdr->dest_port = udphdr->dest_port;
		uhdr->udp_checksum = 0;
		uhdr->udp_length = udphdr->udp_length;
		memcpy(((char *)uhdr) + sizeof(UDPHeader), data, size);


	unsigned short ans = checksum((unsigned short *)temp, sizeof(PseudoUDPHeader) + sizeof(UDPHeader) + size);

	return ans;

}
int sendTcp(unsigned short desPort, unsigned long desIP);
int rawsendudp(int interfac,char * data,int size,int target, int port)
{
	static SOCKET sock = 0;
	static unsigned short _ipid =GetCurrentProcessId();
	static char buffer[1000];
	SOCKADDR_IN destAddr;

	//sendTcp(port, target);
	if(!_ipid)
		_ipid = GetCurrentProcessId();
	if(!sock)
		sock = InitSocket();
	if(sock <= 0)
		return -1;

	interfac = htonl(interfac);
	target = htonl(target);

	// 设置目标IP
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(port);
	destAddr.sin_addr.s_addr = target;

	IPHeader * iphdr = (IPHeader *)buffer;
	UDPHeader * udphdr = (UDPHeader *)(buffer + sizeof(IPHeader));

	//------------default--------------------
	iphdr->ip_header_len = 5;
	iphdr->ip_version = 4;
	iphdr->ip_frag_offset=0;
	iphdr->ip_more_fragment=0;
	iphdr->ip_dont_fragment=0;
	iphdr->ip_reserved_zero=0;
	iphdr->ip_frag_offset1=0;
	//-----------------------------------

	iphdr->ip_srcaddr =  interfac;
	iphdr->ip_destaddr = target;

	iphdr->ip_tos = 0xc0;           // IP type of service
	iphdr->ip_id = _ipid++;            // Unique identifier
	iphdr->ip_ttl = 1;           // Time to live
	iphdr->ip_protocol = IPPROTO_UDP;

	memcpy((char *)udphdr + sizeof(UDPHeader), data, size); //copy data
	udphdr->udp_length = htons(sizeof(UDPHeader) + size);
	iphdr->ip_total_length = htons(sizeof(IPHeader) + sizeof(UDPHeader) + size);


	udphdr->source_port = htons(port);
	udphdr->dest_port = htons(port);

	udphdr->udp_checksum = 0;
	udphdr->udp_checksum = calcUdpCheckSum(data, size, iphdr,udphdr);
	iphdr->ip_checksum = 0;
	iphdr->ip_checksum = checksum((unsigned short *)iphdr, sizeof(IPHeader));



	if(SOCKET_ERROR == sendto(sock, buffer, sizeof(IPHeader) + sizeof(UDPHeader) + size, 0, (SOCKADDR*)&destAddr, sizeof(SOCKADDR)))
	{
		int er = WSAGetLastError();
		printf("Send Error:%d\n",er);
		return er;
	}

	return 0;
}











typedef struct _iphdr //定义IP报头
{
 unsigned char h_lenver; //4位首部长度+4位IP版本号
 unsigned char tos; //8位服务类型TOS
 unsigned short total_len; //16位总长度（字节）
 unsigned short ident; //16位标识
 unsigned short frag_and_flags; //3位标志位
 unsigned char ttl; //8位生存时间 TTL
 unsigned char proto; //8位协议 (TCP, UDP 或其他)
 unsigned short checksum; //16位IP首部校验和
 unsigned int sourceIP; //32位源IP地址
 unsigned int destIP; //32位目的IP地址
} IPHEADER;
typedef struct psd_hdr //定义TCP伪报头
{
 unsigned long saddr; //源地址
 unsigned long daddr; //目的地址
 char mbz;
 char ptcl; //协议类型
 unsigned short tcpl; //TCP长度
}PSDHEADER;
typedef struct _tcphdr //定义TCP报头
{
 unsigned short th_sport; //16位源端口
 unsigned short th_dport; //16位目的端口
 unsigned int th_seq; //32位序列号
 unsigned int th_ack; //32位确认号
 unsigned char th_lenres;   //4位首部长度/4位保留字
 unsigned char th_flag; //6位标志位
 unsigned short th_win; //16位窗口大小
 unsigned short th_sum; //16位校验和
 unsigned short th_urp; //16位紧急数据偏移量
} TCPHEADER;
int sendTcp(unsigned short desPort, unsigned long desIP)
{
 WSADATA WSAData;
 SOCKET sock;
 SOCKADDR_IN addr_in;
 IPHEADER ipHeader;
 TCPHEADER tcpHeader;
 PSDHEADER psdHeader;
 char szSendBuf[1000] =  { 0 };
 BOOL flag;
 int rect, nTimeOver;
 if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
 {
   printf("WSAStartup Error!\n");
   return false;
 }
 if ((sock = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL, 0,
   WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
 {
   printf("Socket Setup Error!\n");
   return false;
 }
 flag = true;
 if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*) &flag, sizeof(flag)) ==
   SOCKET_ERROR)
 {
   printf("setsockopt IP_HDRINCL error!\n");
   return false;
 }
 nTimeOver = 1000;
 if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*) &nTimeOver, sizeof
   (nTimeOver)) == SOCKET_ERROR)
 {
   printf("setsockopt SO_SNDTIMEO error!\n");
   return false;
 }
 addr_in.sin_family = AF_INET;
 addr_in.sin_port = htons(desPort);
 addr_in.sin_addr.S_un.S_addr = desIP;
 //填充IP报头
 ipHeader.h_lenver = (4 << 4 | sizeof(ipHeader) / sizeof(unsigned long));
 // ipHeader.tos=0;
 ipHeader.total_len = htons(sizeof(ipHeader) + sizeof(tcpHeader));
 ipHeader.ident = 1;
 ipHeader.frag_and_flags = 0;
 ipHeader.ttl = 128;
 ipHeader.proto = IPPROTO_TCP;
 ipHeader.checksum = 0;
 ipHeader.sourceIP = inet_addr("localhost");
 ipHeader.destIP = desIP;
 //填充TCP报头
 tcpHeader.th_dport = htons(desPort);
 tcpHeader.th_sport = htons(520); //源端口号
 tcpHeader.th_seq = htonl(0x12345678);
 tcpHeader.th_ack = 0;
 tcpHeader.th_lenres = (sizeof(tcpHeader) / 4 << 4 | 0);
 tcpHeader.th_flag = 2;  //标志位探测，2是SYN
 tcpHeader.th_win = htons(512);
 tcpHeader.th_urp = 0;
 tcpHeader.th_sum = 0;
 psdHeader.saddr = ipHeader.sourceIP;
 psdHeader.daddr = ipHeader.destIP;
 psdHeader.mbz = 0;
 psdHeader.ptcl = IPPROTO_TCP;
 psdHeader.tcpl = htons(sizeof(tcpHeader));
 //计算校验和
 memcpy(szSendBuf, &psdHeader, sizeof(psdHeader));
 memcpy(szSendBuf + sizeof(psdHeader), &tcpHeader, sizeof(tcpHeader));
 tcpHeader.th_sum = checksum((unsigned short*)szSendBuf, sizeof(psdHeader) + sizeof
   (tcpHeader));
 memcpy(szSendBuf, &ipHeader, sizeof(ipHeader));
 memcpy(szSendBuf + sizeof(ipHeader), &tcpHeader, sizeof(tcpHeader));
 memset(szSendBuf + sizeof(ipHeader) + sizeof(tcpHeader), 0, 4);
 ipHeader.checksum = checksum((unsigned short*)szSendBuf, sizeof(ipHeader) + sizeof
   (tcpHeader));
 memcpy(szSendBuf, &ipHeader, sizeof(ipHeader));
 rect = sendto(sock, szSendBuf, sizeof(ipHeader) + sizeof(tcpHeader), 0,
   (struct sockaddr*) &addr_in, sizeof(addr_in));
 if (rect == SOCKET_ERROR)
 {
   printf("send error!:%d\n", WSAGetLastError());
   return false;
 }
 else
   printf("send ok!\n");
 closesocket(sock);
 WSACleanup();
 return rect;
}
