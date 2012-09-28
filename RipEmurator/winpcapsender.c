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

	/* ��ȡ���ػ����豸�б� */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs_ex: %s\n", errbuf);
		return -1;
	}

	/* ��ӡ�б� */
	for(d= alldevs; d != NULL; d= d->next)
	{
		int address;
		printf("%d. %s", ++i, d->name);
		//address =  *(d->addresses->addr);
		if (address == interfac)
		{
			pcap_t *fp;
		    /* ������豸 */
		    if ( (fp= pcap_open(d->name,            // �豸��
		                        100,                // Ҫ����Ĳ��� (ֻ����ǰ100���ֽ�)
		                        1,  // ����ģʽ
		                        1000,               // ����ʱʱ��
		                        NULL,               // Զ�̻�����֤
		                        errbuf              // ���󻺳�
		                        ) ) == NULL)
		    {
		        fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		        return -1;
		    }

		    /* �������ݰ� need  add ip & UDP head!!*/
		    //if (pcap_sendpacket(fp, packet, 100 /* size */) != 0)
		    //{
		    //    fprintf(stderr,"\nError sending the packet: \n", pcap_geterr(fp));
		    //    return;
		    //}
		}
	}


	/* ������Ҫ�豸�б��ˣ��ͷ��� */
	pcap_freealldevs(alldevs);
}
