#include "../include/simulator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Project 2 - Reliable Transport Protocols
 * CSE 589
 *  hamzaabu and muhanned
 *  "We have read and understood the academic integrity policy"
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

/********* STUDENTS WRITE THE NEXT SIX ROUTINES *********/

/*Variable(s) and data structure(s) declaration*/
#define A 0
#define B 1

int i,j;
struct msg buffer[1000];
struct pkt sample;
int timeout;
int seqNum;
int ackNum;
int stateA;
int stateB;
 
/*Function Drfinitions*/
int checksum_init(struct pkt packet);
int isValidPacket(struct pkt packet);

/*Function Declarations*/
int isValidPacket(struct pkt packet){
	if(packet.checksum == checksum_init(packet)) return 1; 	
	else return 0;
	}

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
	if(stateA){
	        buffer[j] = message;
		j++;            
		return;
	}

	struct pkt temp;
	memset(&temp,0,sizeof(temp));
	temp.acknum = seqNum;
	
	//
	temp.checksum = seqNum + temp.acknum;
	temp.seqnum = seqNum;
	for(int i = 0 ; i < 20 ; i++) temp.checksum += (int) message.data[i];
	//
	stateA = 1;
	strncpy(temp.payload,message.data,20);
	starttimer(A,timeout);

	tolayer3(A,temp);
	sample = temp;
	return;	
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
	if(!isValidPacket(packet) || seqNum != packet.acknum) return;
	
	stoptimer(A);
	seqNum = !seqNum;
	
	if(i<j){
		struct msg messageTemp = buffer[i];
		struct pkt temp;
		memset(&temp,0,sizeof(temp));

		temp.checksum = checksum_init(temp);
		stateA = 1;
		strncpy(temp.payload,messageTemp.data,20);
		starttimer(A,timeout);
		tolayer3(A,temp);
		sample = temp;
		i++;
	}
        else stateA = 0;	

	return;
	
	
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
	tolayer3(A,sample);
	starttimer(A,timeout);
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
	timeout = 15;
	stateA = 0;	
	seqNum = 0;
	i = 0;
	j = 0;
	memset(&sample,0,sizeof(sample));
	memset(&buffer,0,sizeof(buffer));	
	return;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{
	//Check for the checksum and packet corruption
	if(isValidPacket(packet)){
		struct pkt temp;
		memset(&temp,0,sizeof(temp));
		temp.acknum = packet.seqnum;
		//temp.checksum = checksum_init(temp);
		temp.checksum = temp.seqnum + temp.acknum;
		for(int n = 0 ; n < 20 ; n++) temp.checksum += (int) temp.payload[n];
		tolayer3(B,temp);
		
		if(packet.seqnum == stateB){
			tolayer5(B,packet.payload);
			stateB = !stateB;
		} 
	} 	
	return;
	 
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
	stateB = 0;
	return;
}
