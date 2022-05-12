#include "../include/simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project 2 - Reliable Transport Protocols
 * CSE 589
 * hamzaabu and muhanned
 * "We have read and understood the academic integrity policy"
 */ 

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer 
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#define A 0
#define B 1

struct msg buffer[1500];
struct pkt retransmit[1500]; //Buffer of struct pkt instead of msg because we need seqnum and acknum
int i,j;
int min;
int timeout;
int seqNum;
int packEx;
/*Function Definition*/
int checksum_init(struct pkt packet);

/*Function Declaration*/
int checksum_init(struct pkt packet){
	
	int checksum = 0;
	int data = 20;
	
	//Check if the payload is Null
	if(packet.payload == NULL) return 0;
	
	int seq_num = packet.seqnum;
	int ack_num = packet.acknum;
				
	checksum += seq_num;
	checksum += ack_num;
						
	for(int n = 0 ; n < data ; n++) checksum += (unsigned char) packet.payload[n];

	return checksum;
}

/* called from layer 5, passed the data to be sent to other side */
void A_output(message)
  struct msg message;
{
	if (seqNum < min + getwinsize()) {
		struct pkt temp;
		memset(&temp, 0, sizeof(temp));
		
		temp.seqnum = seqNum;
		strncpy(temp.payload, message.data, 20);
		int temp_checksum = checksum_init(temp);
		temp.checksum = temp_checksum;
		tolayer3(A, temp);
		//Note that if we are sending the first packet in the window, then we must start a countdown
		//as demonstrated by the behavior in the GBN applet found in https://www2.tkn.tu-berlin.de/teaching/rn/animations/gbn_sr/
		if (seqNum == min) starttimer(A, timeout);
		
		retransmit[seqNum] = temp;
		seqNum++;
		return;
	}
	buffer[j++] = message;
	
	return;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
	
	int checkSum = checksum_init(packet);
	if (checkSum != packet.checksum) return;
	
	min = packet.acknum + 1;
	
	while (seqNum < min + getwinsize() && i < j) {
		
		struct pkt temp;
		memset(&temp, 0, sizeof(temp));
		
		temp.seqnum = seqNum;
		strncpy(temp.payload, buffer[i].data, 20);
		
		temp.checksum = checksum_init(temp);
		tolayer3(A, temp);
		if (min == seqNum) starttimer(A, timeout);
		
		retransmit[seqNum] = temp;
		i++;
		seqNum++;
	}
	if (min == seqNum) stoptimer(A);
	else starttimer(A, timeout);
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
	starttimer(A, timeout);
	for (int i = min; i < seqNum; i++) tolayer3(A, retransmit[i]);
	return;
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
	memset(&buffer, 0, sizeof(buffer));
	memset(&retransmit, 0, sizeof(retransmit));
	i = 0;
	j = 0;
	min = 0;
	seqNum = 0;
	timeout=20;
	return;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{
	
	int checkSum = checksum_init(packet);
	if (checkSum != packet.checksum) return;

	if (packet.seqnum == packEx) {
	
		struct pkt temp;
		memset(&temp, 0, sizeof(temp));
	
		temp.acknum = seqNum;
		temp.checksum = checksum_init(temp);
	
		tolayer3(B, temp);
		tolayer5(B, packet.payload);
	
		packEx++;
	}
	else if (packEx != 0) {
		
		struct pkt temp;
		memset(&temp, 0, sizeof(temp));
		
		temp.acknum = seqNum - 1;
		temp.checksum = checksum_init(temp);
		tolayer3(B, temp);
	}
	return;
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
	packEx = 0;
	timeout=20;
	return;
}
