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
  int packet_id;
  int nextseqnum;
  int incoming_seqnum;
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
    if (TraceLevel == -1 || TraceLevel > 1) {
      printf(YEL "Ignoring.\n" RESET);
    }
  }
  else {
    // if we got an old nak, just wait for timeout
    // this means the reciever missed something
    if (packet.acknum < A.base) {
      debug_print("Got Old Ack", RED, packet);
      return;
    }

    debug_print("Got Ok NAK/ACK", GRN, packet);

    // move window forward and add queued packets to send window
    do {
      A.base++;
      dequeue(&A.send_window, NULL);

      if (!queue_empty(A.packet_queue)) {
        // take one off the queue, add send it
        struct pkt p;
        dequeue(&A.packet_queue, &p);
        p.acknum = A.packet_id++;
        set_checksum(&p);
        enqueue(&A.send_window, p);
        debug_print("Sending", GRN, p);
        tolayer3(AEntity, p);

        A.nextseqnum++;
      }
    }
    while (A.base < packet.acknum + 1);

    // get rid of any previously acked packets.
    // this is a result of cummulative acking

    // restart the timer
    stopTimer(AEntity);
    startTimer(AEntity, A.timer_length);
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
  new_packet.seqnum = A.incoming_seqnum++;
  new_packet.acknum = A.packet_id++;
  set_checksum(&new_packet);

  // send if it's within our window
  if (A.nextseqnum < A.base + A.window_size) {
    // send the data to the network
    debug_print("Original Send", GRN, new_packet);
    tolayer3(AEntity, new_packet);
    enqueue(&A.send_window, new_packet);

    //start the timer & increment seqnum
    if (A.base == A.nextseqnum) {
      startTimer(AEntity, A.timer_length);
    }
    A.nextseqnum++;
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
  A.timer_length *= 2;
  if (TraceLevel == -1 || TraceLevel > 1) {
    printf(YEL "Timeout!\n" RESET);
  }

  int i;
  // resend the packets in the window
  for (i = 0; i < A.send_window.size; i++) {
    struct pkt p = queue_at(A.send_window, i);
    p.acknum = A.packet_id++;
    debug_print("Resending", GRN, p);
    set_checksum(&p);
    tolayer3(AEntity, p);
  }

  // restart the timer
  startTimer(AEntity, A.timer_length);
}

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
  A.timer_length = 100;

  A.nextseqnum = 0;
  A.packet_id = 1000;
  A.base = 0;
  A.incoming_seqnum = 0;
  A.window_size = 8; // treated as constant, but could be anything
  queue_new(&A.packet_queue);
  queue_new(&A.send_window);
}
