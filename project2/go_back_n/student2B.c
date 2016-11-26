#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project2.h"
#include "student2_common.h"

struct B_data {
  int packet_id;
  int expected_seqnum;
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
  if (!verify_checksum(packet)) {
    corrupt_count++;
    debug_print("Corrupt pkt", RED, packet);

    struct pkt nak;
    memset(nak.payload, 0, MESSAGE_LENGTH);
    nak.acknum = B.expected_seqnum - 1;
    nak.seqnum = -B.packet_id++;
    set_checksum(&nak);

    // send ack for the last recieved packet
    debug_print("Sending NAK", CYN, nak);
    tolayer3(BEntity, nak);
  }
  else if (packet.seqnum != B.expected_seqnum) {
    debug_print("Wrong seqnum", RED, packet);

    struct pkt nak;
    memset(nak.payload, 0, MESSAGE_LENGTH);
    nak.acknum = B.expected_seqnum - 1;
    nak.seqnum = -B.packet_id++;
    set_checksum(&nak);

    // send nak for the last recieved pnaket
    debug_print("Sending Old Ack", CYN, nak);
    tolayer3(BEntity, nak);
  }
  else {
    debug_print("Received", GRN, packet);

    //deliver to app layer
    struct msg message;
    memcpy(message.data, packet.payload, MESSAGE_LENGTH);
    tolayer5(BEntity, message);

    // send a cummulative ACK in response
    struct pkt ack;
    ack.acknum = B.expected_seqnum;
    ack.seqnum = B.packet_id++;
    memset(ack.payload, 0, MESSAGE_LENGTH);
    set_checksum(&ack);

    debug_print("Sending Ack", CYN, ack);
    tolayer3(BEntity, ack);

    B.expected_seqnum++;
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
  B.packet_id = 9000;
  B.expected_seqnum = 0;
}


// UNUSED IN THIS PROJECT
void B_output(struct msg message) {}
