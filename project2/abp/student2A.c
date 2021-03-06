#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "project2.h"
#include "student2_common.h"

struct A_data {
  int successes;
  int alternating_bit;
  int expected_ack;
  struct pkt unacked_packet;
  bool ready_to_send;
  struct queue packet_queue;
  double timer_length;
} A;

/*
 * A_input(packet), where packet is a structure of type pkt. This routine
 * will be called whenever a packet sent from the B-side (i.e., as a result
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side.
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
  if (!verify_checksum(packet)) {
    corrupt_count++;

    debug_print("Corrupt Ack/Nak", RED, packet);
    debug_print("Resending", GRN, A.unacked_packet);
    tolayer3(AEntity, A.unacked_packet);
  }
  else if (packet.acknum != A.expected_ack) {
    debug_print("Wrong Ack.", RED, packet);
    debug_print("Resending", GRN, A.unacked_packet);
    print_packet(A.unacked_packet);
    tolayer3(AEntity, A.unacked_packet);
  }
  else if (packet.seqnum == -1) { // NAK, retransmit
    debug_print("Got OK Nak", RED, packet);
    debug_print("Resending", GRN, A.unacked_packet);
    tolayer3(AEntity, A.unacked_packet);
  }
  else if (packet.seqnum == 1) {
    // nah we good
    A.successes++;
    debug_print("Got Ok Ack", GRN, packet);

    // stop the timer
    stopTimer(AEntity);

    // send the next packet waiting in the queue
    if (!queue_empty(A.packet_queue)) {
      dequeue(&A.packet_queue, &A.unacked_packet);
      A.expected_ack = A.unacked_packet.seqnum;
      debug_print("Dequeue", GRN, A.unacked_packet);
      tolayer3(AEntity, A.unacked_packet);

      //start timer for new one
      startTimer(AEntity, A.timer_length);
    }
    else {
      A.ready_to_send = true;
    }
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
  struct pkt new_packet;
  memcpy(new_packet.payload, message.data, MESSAGE_LENGTH);
  new_packet.seqnum = A.alternating_bit;
  new_packet.acknum = 0;
  set_checksum(&new_packet);

  if (A.ready_to_send) {
    // send the data to the network
    A.ready_to_send = false;
    A.unacked_packet = new_packet;
    A.expected_ack = new_packet.seqnum;

    debug_print("Sending", GRN, new_packet);
    tolayer3(AEntity, A.unacked_packet);

    //start the timer
    startTimer(AEntity, A.timer_length);
  }
  else {
    debug_print("Queueing", MAG, new_packet);
    enqueue(&A.packet_queue, new_packet);
  }

  // flip
  A.alternating_bit = 1 - A.alternating_bit;
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires
 * (thus generating a timer interrupt). You'll probably want to use this
 * routine to control the retransmission of packets. See starttimer()
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
  // resend the packet
  A.timer_length *= 2;
  if (TraceLevel == -1 || TraceLevel > 1) {
    printf(YEL "Timeout\n" RESET);
  }
  debug_print("Resending", GRN, A.unacked_packet);
  tolayer3(AEntity, A.unacked_packet);

  // restart the timer
  startTimer(AEntity, A.timer_length);
}

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
  A.timer_length = 1000;
  A.successes = 0;
  A.expected_ack = 0;
  A.alternating_bit = 0;
  A.ready_to_send = true;
  queue_new(&A.packet_queue);
}
