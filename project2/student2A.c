#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "project2.h"
#include "student2_common.h"

struct A_data {
  int window_size;
  int base;
  int seqnum;
  double timer_length;
  struct queue packet_queue;
  struct queue send_window;
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
    debug_print("Correupt Ack/Nak", RED, packet);
    printf(YEL "Ignoring...\n" RESET);
  }
  else if (packet.seqnum == -1) { //TODO: does GBN have NAKs?
    debug_print("Got OK Nak", RED, packet);
    printf(YEL "Ignoring...\n" RESET);
  }
  else if (packet.seqnum == 1) {
    // nah we good
    debug_print("Got Ok Ack", GRN, packet);

    // stop the timer & move window forward
    A.base = packet.acknum + 1;
    dequeue(&A.send_window, NULL);
    if (A.base == A.seqnum) {
      stopTimer(AEntity);
    }
    else {
      //start timer for new one
      startTimer(AEntity, A.timer_length);
    }

    // send the next packet waiting in the queue
    if ((A.seqnum < A.base + A.window_size)
        && !queue_empty(A.packet_queue)) {
      struct pkt p;
      dequeue(&A.packet_queue, &p);
      //resassign seqnum because these were queue'd in the past
      p.seqnum = A.seqnum;
      debug_print("Dequeue", GRN, p);

      tolayer3(AEntity, p);
      enqueue(&A.send_window, p);

      A.seqnum++;
    }
  }
}

/*
 * A_output(message) Where message is a structure of type msg, containing data to be sent
 * to the B-side. This routine will be called whenever the upper layer
 *
 * at the sending side (A) has a message to send.
 * It is the job of your protocol to insure that the data
 * in such a message is delivered in-order, and correctly,
 * to the receiving side upper layer.
 */
void A_output(struct msg message) {
  struct pkt new_packet;
  memcpy(new_packet.payload, message.data, MESSAGE_LENGTH);
  new_packet.seqnum = A.seqnum;
  new_packet.acknum = 0;
  set_checksum(&new_packet);

  // send if it's within our window
  if (A.seqnum < A.base + A.window_size) {
    // send the data to the network
    debug_print("Sending", GRN, new_packet);
    tolayer3(AEntity, new_packet);
    enqueue(&A.send_window, new_packet);

    //start the timer & increment seqnum
    if (A.base == A.seqnum) {
      startTimer(AEntity, A.timer_length);
    }
    A.seqnum++;
  }
  else {
    debug_print("Queueing", MAG, new_packet);
    enqueue(&A.packet_queue, new_packet);
  }
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires
 * (thus generating a timer interrupt). You'll probably want to use this
 * routine to control the retransmission of packets. See starttimer()
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
  // resend the packet
  printf(YEL "Timeout\n" RESET);

  if (A.send_window.size <= 0) {
    exit(0);
  }

  int i;
  for (i = 0; i < A.send_window.size; i++) {
    struct pkt p = queue_at(A.send_window, i);
    debug_print("Resending", GRN, p);
    tolayer3(AEntity, p);
  }

  // restart the timer
  startTimer(AEntity, A.timer_length);
}

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
  // A small hack. Take the user-specified time between messages
  // and double it, just to be safe. This gives a decent initial value
  A.timer_length = 5 * AveTimeBetweenMsgs + 10;
  A.seqnum = 0;
  A.base = 0;
  A.window_size = 4; // treated as constant, but could be anything
  queue_new(&A.packet_queue);
  queue_new(&A.send_window);
}
