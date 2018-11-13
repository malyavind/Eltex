#include "mylib.h"

void my_callback(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet);

int main() 
{ 
	//int i;
	char *dev; 
	char errbuf[PCAP_ERRBUF_SIZE]; 
	pcap_t* descr; 
	const char *filter = "ip or icmp or tcp or udp"; 
	const u_char *packet; 
	struct pcap_pkthdr hdr;
	struct ether_header *eptr; /* net/ethernet.h */ 
	struct bpf_program fp;     /*выражение фильтрации в составленном виде */ 
	bpf_u_int32 maskp;         /*маска подсети */ 
	bpf_u_int32 netp;          /* ip */ 

	/* Получение имени устройства */
	dev = pcap_lookupdev(errbuf); 
	
	if(dev == NULL) {
		fprintf(stderr, "%s\n", errbuf);
		exit(1);
	} 
	/* Получение сетевого адреса и маски сети для устройства */ 
	pcap_lookupnet(dev, &netp, &maskp, errbuf); 

	/* открытие устройства в  promiscuous-режиме */ 
	descr = pcap_open_live(dev, BUFSIZ, 1,-1, errbuf); 
	if(descr == NULL) {
		printf("pcap_open_live(): %s\n", errbuf);
		exit(1);
	} 

	/* теперь составляется выражение фильтрации*/ 
	if(pcap_compile(descr, &fp, filter, 0, netp) == -1) {
		fprintf(stderr, "Error calling pcap_compile\n");
		exit(1);
	} 

	/* применение фильтра*/ 
	if(pcap_setfilter(descr, &fp) == -1) {
		fprintf(stderr, "Error setting filter\n");
		exit(1);
	} 

	/* функция обратного вызова используется в цикле */ 
	pcap_loop(descr, -1, my_callback, NULL); 
	return 0; 
}

void my_callback(u_char *args, const struct pcap_pkthdr* pkthdr, const u_char* packet) 
{ 
	const struct sniff_ethernet *ethernet;
	static int count = 0; 

	printf("Packet Count: %d\n", ++count);             
	
	/* define ethernet header */
	ethernet = (struct sniff_ethernet*)(packet);
	
	printf("Packet Ethernet Header:\n");
	printf("Dest: %02X:%02X:%02X:%02X:%02X:%02X\n",
			ethernet->ether_dhost[0],
			ethernet->ether_dhost[1],
			ethernet->ether_dhost[2],
			ethernet->ether_dhost[3],
			ethernet->ether_dhost[4],
			ethernet->ether_dhost[5]);
	printf("Source: %02X:%02X:%02X:%02X:%02X:%02X\n",
			ethernet->ether_shost[0],
			ethernet->ether_shost[1],
			ethernet->ether_shost[2],
			ethernet->ether_shost[3],
			ethernet->ether_shost[4],
			ethernet->ether_shost[5]);
	
	switch(ethernet->ether_type){	
		case 0x0008:
			printf("Type: IPv4\n");
			break;
		case 0x0608:
			printf("Type: Address Resolution Protocol (ARP)\n");
			break;
		case 0x3580:
			printf("Type: Reverse Address Resolution Protocol (RARP)\n");
			break;
		case 0x9b80:
			printf("Type: AppleTalk (Ethertalk)\n");
			break;
		case 0xf380:
			printf("Type: Appletalk Address Resolution Protocol (AARP)\n");
			break;
		case 0x3781:
			printf("Type: Novell IPX (alt)\n");
			break;
		case 0x3881:
			printf("Type: Novell\n");
			break;
		case 0xdd86:
			printf("Type: IPv6\n");
			break;																	
	}
	printf("\n");				
		   
}

