#include "../include/simulator.h"
#include <vector>
#include <iostream>
#include <string.h>

/* Project 2
 * CSE 589
 * hamzaabu and muhanned
 * "We have read and understood the course academic integrity policy"
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

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

/* called from layer 5, passed the data to be sent to other side */

//Data Structure declaration
int next_seq_num;
int base;
int exp_seq_num = 0;
struct messages{
	struct msg message;
	struct message_queue * next_message;
}* message_queue;

//Function declaration
int checksum_init(struct pkt packet);
int corrupt_packet(pkt packet);
void send_data();

int corrupt_packet(struct pkt packet){

	//Checking for the corruption through packet checksum
	if(packet.checksum != checksum_init(packet) return true;
	else return false;

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

	for(int n = 0 ; n < data ; n++) checksum += (unsigned char) packet.payload[i];

	return checksum;

}	
void A_output(message)
  struct msg message;
{
	//Add the message to the message queue
	message_queue.push_back(message);
	
	//Send the data 
	send_data();
	
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
	
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
	
	//Assign the next sequence from the beginning
	next_seq_num = base;
	
	//Send the data again
	send_data();
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
	base = 0;
	next_seq_num = 0;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{

}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
	exp_seq_num = 0;
}
