#include "../include/simulator.h"
#include <vector>
#include <iostream.h>
#include <string.h>

/* Project 2
 * CSE 589
 * hamzaabu and muhanned
 *  "We have read and understood the course academix integrity policy"
 * */

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

/* called from layer 5, passed the data to be sent to other side */

/*Define A and B + Data Structures needed*/
#define A 0
#define B 1
#define waitingAck 0
#define available 1

int stateA;

struct bufferQueue {
	struct msg message;
	struct bufferQueue * nextMessage; 
} messages;

/*Declare the functions*/
int checksum_init(struct pkt packet);
void send_msg();

/*Declare the functions*/
int checksum_init(struct pkt packet){
	/*Function to create a checksum*/
	int checksum = 0;
		
	//Check if the payload is Null, it is then an acknowledgement
	if(packet.payload == NULL) return 0;

	int seq_num = packet.seqnum;
	int ack_num = packet.acknum;
	
	checksum += seq_num;
	checksum += ack_num;
	
	for(int n = 0 ; n < data ; n++) checksum += (unsigned char) packet.payload[i];

	return checksum;

}

bool isCorrupt(pkt packet){
	/*Function to check if the given packet is corrupt*/
	if(packet.checksum != checksum_init(packet) return true;
	else return false;
}

int send_msg(){

	if(messages){
		/*The next packet*/
		struct packet temp;
		temp.seqnum = seqNum_A;
		temp.acknum = seqNum_A;
	
		/*Allot the soace*/
		memcpy(temp.payload,(messages -> message).data,sizeof((messages.message).data));

		/*Create the checksum*/
		temp.checksum = checksum_init(temp);
		
		/*Send the packet to Layer 3*/		
		tolayer3(A,temp);

		/*Change the state of A to waiting*/
		stateA = waitingAck;
		
		/*Start the timer for A*/
		starttimer(A,TIMEOUT);
	}	

}

void A_output(message)
  struct msg message;
{

}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{

}

/* called when A's timer goes off */
void A_timerinterrupt()
{
	/*We will only resend the last message in case of the timer interrupt*/
	send_msg();
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
	
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{

}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{

}
