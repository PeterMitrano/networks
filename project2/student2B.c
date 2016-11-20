#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project2.h"
#include "student2_common.h"

#define BENTITY 1

struct B_data {
  int alternating_bit;
} B;

/*
 * B_input(packet),where packet is a structure of type pkt. This routine
 * will be called whenever a packet sent from the A-side (i.e., as a result
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side.
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {

  // deliver the data to app layer
  // but only if it's not fucked
  if (verify_checksum(packet) && packet.seqnum == B.alternating_bit) {
    struct msg message;
    printf(GRN "Delivering correct data: %i\n" RESET, packet.seqnum);
    memcpy(message.data, packet.payload, MESSAGE_LENGTH);
    tolayer5(BENTITY, message);

    // send an ACK in response
    struct pkt ack;
    ack.acknum = packet.seqnum;
    ack.seqnum = 1;
    ack.checksum = 0;
    ack.checksum = compute_checksum(ack);
    tolayer3(BENTITY, ack);

    B.alternating_bit = 1 - B.alternating_bit;
  }
  else {
    // send a NAK
    // use -1 seq num to indicate nak
    struct pkt nak;
    nak.acknum = packet.seqnum;
    nak.seqnum = -1;
    tolayer3(BENTITY, nak);
  }
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires
 * (thus generating a timer interrupt). You'll probably want to use this
 * routine to control the retransmission of packets. See starttimer()
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
}

/*
 * The following routine will be called once (only) before any other
 * entity B routines are called. You can use it to do any initialization
 */
void B_init() {
  B.alternating_bit = 0;
}


// UNUSED IN THIS PROJECT
void B_output(struct msg message) {}
