#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project2.h"
#include "student2_common.h"

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

  if (corrupt_count > NumMsgsCorrupt) {
    printf("FUCK %i %i", corrupt_count, NumMsgsCorrupt);
  }

  // deliver the data to app layer
  // but only if it's not fucked
  if (!verify_checksum(packet)) {
    corrupt_count++;

    // send a NAK
    printf("(%i, %i) ", corrupt_count, NumMsgsCorrupt);
    printf(RED "CORRUPT PKT. ");
    print_packet(packet);
    struct pkt nak;
    memset(nak.payload, 0, MESSAGE_LENGTH);
    nak.acknum = packet.seqnum;
    nak.seqnum = -1;
    set_checksum(&nak);
    printf("(%i, %i) ", corrupt_count, NumMsgsCorrupt);
    printf(CYN "NAK . ");
    print_packet(nak);
    tolayer3(BEntity, nak);
  }
  else if (packet.seqnum != B.alternating_bit) {
    // send ack for the (wrong) packet
    printf("(%i, %i) ", corrupt_count, NumMsgsCorrupt);
    printf(RED "Wrong PKT : ");
    print_packet(packet);
    struct pkt nak;
    memset(nak.payload, 0, MESSAGE_LENGTH);
    nak.acknum = packet.seqnum;
    nak.seqnum = 1;
    set_checksum(&nak);
    printf("(%i, %i) ", corrupt_count, NumMsgsCorrupt);
    printf(CYN "ACK . ");
    print_packet(nak);
    tolayer3(BEntity, nak);
  }
  else {
    struct msg message;
    printf("(%i, %i) ", corrupt_count, NumMsgsCorrupt);
    printf(GRN "Recieve : ");
    print_packet(packet);
    memcpy(message.data, packet.payload, MESSAGE_LENGTH);
    tolayer5(BEntity, message);

    // send an ACK in response
    struct pkt ack;
    ack.acknum = packet.seqnum;
    ack.seqnum = 1;
    memset(ack.payload, 0, MESSAGE_LENGTH);
    set_checksum(&ack);
    printf("(%i, %i) ", corrupt_count, NumMsgsCorrupt);
    printf(CYN "ACK . ");
    print_packet(ack);
    tolayer3(BEntity, ack);

    // flip
    B.alternating_bit = 1 - B.alternating_bit;
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
