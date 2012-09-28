/*
 * winpcapsender.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Bleastrind
 */
#include <pcap.h>

int winpcapsend(int interfac,char * buffe,int siz,int targe, int por)
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int i=0;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* 获取本地机器设备列表 */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs_ex: %s\n", errbuf);
		return -1;
	}

	/* 打印列表 */
	for(d= alldevs; d != NULL; d= d->next)
	{
		int address;
		printf("%d. %s", ++i, d->name);
		//address =  *(d->addresses->addr);
		if (address == interfac)
		{
			pcap_t *fp;
		    /* 打开输出设备 */
		    if ( (fp= pcap_open(d->name,            // 设备名
		                        100,                // 要捕获的部分 (只捕获前100个字节)
		                        1,  // 混杂模式
		                        1000,               // 读超时时间
		                        NULL,               // 远程机器验证
		                        errbuf              // 错误缓冲
		                        ) ) == NULL)
		    {
		        fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		        return -1;
		    }

		    /* 发送数据包 need  add ip & UDP head!!*/
		    //if (pcap_sendpacket(fp, packet, 100 /* size */) != 0)
		    //{
		    //    fprintf(stderr,"\nError sending the packet: \n", pcap_geterr(fp));
		    //    return;
		    //}
		}
	}


	/* 不再需要设备列表了，释放它 */
	pcap_freealldevs(alldevs);
}
