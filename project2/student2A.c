#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "project2.h"
#include "student2_common.h"

#define AENTITY 0

bool alternating_bit;
struct pkt unacked_packet;
bool valid_unacked_packet;

/*
 * A_input(packet), where packet is a structure of type pkt. This routine
 * will be called whenever a packet sent from the B-side (i.e., as a result
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side.
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
  if (packet.seqnum == -1) { // NAK, retransmit
    tolayer3(AENTITY, unacked_packet);
    printf(BLU "retransmitting %i\n" RESET, packet.acknum);
  }
  else if (packet.seqnum == 1) {
    //nah we good
    printf(GRN "got %i just fine\n" RESET, packet.acknum);
  }
}

/*
 * A_output(message) Where message is a structure of type msg, containing data to be sent
 * to the B-side. This routine will be called whenever the upper layer
 * at the sending side (A) has a message to send.
 * It is the job of your protocol to insure that the data
 * in such a message is delivered in-order, and correctly,
 * to the receiving side upper layer.
 */
void A_output(struct msg message) {
  struct pkt unacked_packet;
  memcpy(unacked_packet.payload, message.data, MESSAGE_LENGTH);
  unacked_packet.seqnum = alternating_bit;
  unacked_packet.checksum = 0;
  unacked_packet.checksum = compute_checksum(unacked_packet);

  valid_unacked_packet = true;

  // send the data to the network
  tolayer3(AENTITY, unacked_packet);

  // flip the bit
  alternating_bit = !alternating_bit;
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires
 * (thus generating a timer interrupt). You'll probably want to use this
 * routine to control the retransmission of packets. See starttimer()
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
  //resend the packet
}

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
  valid_unacked_packet = false;
  alternating_bit = true;
}
