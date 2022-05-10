#include "../include/simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project 2 - Reliable Transport Protocols
 *  CSE 589
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

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/*Variable(s) and data structure(s) declaration*/
#define A 0
#define B 1

int timeout;
int seqNum;
int i,j;
int min;
int winSize;
int packEx;
float timer[4000];
int ackSend[4000];
float isValidBuffer[4000];

struct pkt bufferSend[4000];
struct pkt bufferReach[4000];
struct msg buffer[4000];

/*Function Definition*/
int checksum_init(struct pkt packet);


/*Function Declataion*/
int checksum_init(struct pkt packet){
	
	int checksum = 0;
	int data = 20;
	
	//Check if the payload is Null
	if(packet.payload == NULL) return 0;

	int seq_num = packet.seqnum;
	int ack_num = packet.acknum;
				
	checksum += seq_num;
	checksum += ack_num;
					
	for(int n = 0 ; n < data ; n++) checksum += (int) packet.payload[n];

	return checksum;

	}


/* called from layer 5, passed the data to be sent to other side */
void A_output(message)
  struct msg message;
{
	int temp1 = winSize + min;
	if(seqNum >= temp1){
		buffer[j] = message;
		j++;
		return;
	}
	
	struct pkt temp;
	memset(&temp,0,sizeof(temp));
	temp.seqnum = seqNum;
	temp.checksum = seqNum + temp.acknum;
	
	for(int n = 0 ; n < 20 ; n++) temp.checksum += (int) message.data[n];
	
	strcpy(temp.payload,message.data);
	tolayer3(A,temp);
	
	if(seqNum == min) starttimer(A,timeout);
	
	bufferSend[seqNum] = temp;
	timer[seqNum] = timeout + get_sim_time(); 
	
	seqNum++;
	return;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
	int ackNumPacket = packet.acknum;
	int tempChecksum = checksum_init(packet);
	if(tempChecksum != packet.checksum || ackNumPacket < min || ackNumPacket >= winSize) return; 
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
	float time = timeout + get_sim_time();
	for(int n = 0 ; n < seqNum ; n++){
		if(timer[n] < time && ackSend[n] == 0) min = timer[n];
	} 		
	float simTime = get_sim_time(); 
	for(int n = min ; n < seqNum ; n++){
		if(ackSend[n] == 0 && simTime == timer[n]){
			tolayer3(A,bufferSend[n]);
			timer[i] = simTime + timeout;
			break;
		}
	}
	if(time > 0) starttimer(A,time - simTime); 
	return;
} 

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
 	winSize = getwinsize();
	timeout = 40;
	i = 0;
	j = 0;
	min = 0;
	seqNum = 0;
	
	memset(&buffer,0,sizeof(buffer));	
	memset(&timer,0,sizeof(timer));
	memset(&ackSend,0,sizeof(ackSend));
	memset(&bufferSend ,0,sizeof(bufferSend));
	return;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{
	int seqNumPacket = packet.seqnum;
	int checkSum = checksum_init(packet);
	
	//Chcck for possible corruption, overflow
	if(checkSum != packet.checksum || seqNumPacket >= packEx + winSize || seqNumPacket < packEx - winSize) return;
	 	
	struct pkt temp;
	memset(&temp,0,sizeof(temp));
	
	temp.seqnum = packet.seqnum;
	temp.checksum = checksum_init(temp);
	tolayer3(B,temp);

	if(packet.seqnum != packEx){
		bufferReach[packet.seqnum] = packet;
		isValidBuffer[packet.seqnum] = 1;
	}
	else{
		int temp = packEx + winSize;
		tolayer5(B,packet.payload);
		packEx++;
	
		for(int n = packEx ; n < temp ; n++){
		
			if(isValidBuffer[n] == 0) break;
			tolayer5(B,bufferReach[n].payload);
			packEx++;
		}
	}
	return;

}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
	winSize = getwinsize();
	packEx  = 0;
	timeout = 40;
	
	memset(&bufferReach,0,sizeof(bufferReach));
	memset(&isValidBuffer,0,sizeof(isValidBuffer));
	return;	
}
