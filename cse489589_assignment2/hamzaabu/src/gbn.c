#include "../include/simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project 2 - Reliable Transport Protocols
 * CSE 589
 * hamzaabu and muhanned
 * "We have read and undrstood the academic integrity policy"
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

struct msg buffer[1000];
struct pkt retransmit[1000]; //Buffer of struct pkt instead of msg because we need seqnum and acknum
int begin;
int end;
int base;
int window_size;
int seq_num;
int expected_pkt;
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
	if (seq_num < base + getwinsize()) {
		struct pkt packet;
		memset(&packet, 0, sizeof(packet));
		int packet_checksum = 0;
		packet.seqnum = seq_num;
		packet_checksum = checksum_init(packet);
		packet.checksum = packet_checksum;
		strcpy(packet.payload, message.data);
		tolayer3(0, packet);
		//Note that if we are sending the first packet in the window, then we must start a countdown
		//as demonstrated by the behavior in the GBN applet found in https://www2.tkn.tu-berlin.de/teaching/rn/animations/gbn_sr/
		if (base == seq_num) {
			starttimer(0, 50);
		}
		retransmit[seq_num] = packet;
		seq_num++;
		return;
	}
	buffer[end] = message;
	end++;
	return;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
	int expected_checksum = 0;
	expected_checksum = checksum_init(packet);
	if (expected_checksum != packet.checksum) {
		return;
	}
	
	base = packet.acknum + 1;
	
	while (seq_num < base + getwinsize() && begin < end) {
		
		struct pkt buffered_packet;
		memset(&buffered_packet, 0, sizeof(buffered_packet));
		int packet_checksum = 0;
		buffered_packet.seqnum = seq_num;
		packet_checksum = checksum_init(buffered_packet);
		buffered_packet.checksum = packet_checksum;
		strcpy(buffered_packet.payload, buffer[begin].data);
		tolayer3(0, buffered_packet);
		if (base == seq_num) {
			starttimer(0, 50);
		}
		retransmit[seq_num] = buffered_packet;
		begin++;
		seq_num++;
	}
	if (base == seq_num) {
		stoptimer(0);
	}
	else {
		starttimer(0, 50);
	}
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
	starttimer(0, 50);
	for (int i = base; i < seq_num; i++) {
		tolayer3(0, retransmit[i]);
	}
	return;
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
	memset(&buffer, 0, sizeof(buffer));
	memset(&retransmit, 0, sizeof(retransmit));
	begin = 0;
	end = 0;
	base = 0;
	seq_num = 0;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{
	int expected_checksum = 0;
	expected_checksum = checksum_init(packet);
	if (expected_checksum != packet.checksum) {
		return;
	}

	if (packet.seqnum == expected_pkt) {
		struct pkt ack;
		memset(&ack, 0, sizeof(ack));
		ack.acknum = seq_num;
		ack.checksum = checksum_init(ack);
		tolayer3(1, ack);
		tolayer5(1, packet.payload);
		expected_pkt++;
	}
	else if (expected_pkt != 0) {
		struct pkt ack;
		memset(&ack, 0, sizeof(ack));
		ack.acknum = seq_num - 1;
		ack.checksum = checksum_init(ack);
		tolayer3(1, ack);
	}
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
	expected_pkt = 0;
	return;
}
