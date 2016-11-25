#include <stdbool.h>
#include <string.h>

#include "student2_common.h"
#include "crc32.h"

int corrupt_count = 0;

void debug_print(char *prefix, char *color, struct pkt packet) {
  if (TraceLevel == -1 || TraceLevel > 1) {
    printf("(%-3i, %-3i) %s%-17s ", corrupt_count, NumMsgsCorrupt, color, prefix);
    print_packet(packet);
  }
}

void print_packet(struct pkt packet) {
  printf("%-10i  %-10i  ", packet.seqnum, packet.acknum);
  int i;
  for (i = 0; i < MESSAGE_LENGTH; i++) {
    printf("%c", packet.payload[i]);
  }
  printf(RESET "\n");
}

void set_checksum(struct pkt *packet) {
  packet->checksum = 0;
  char *raw_packet = (char *) packet;
  uint32_t crc = crc32(0, raw_packet, sizeof(struct pkt));
  packet->checksum = crc;
  return;
}

bool verify_checksum(struct pkt packet) {
  uint32_t checksum = (uint32_t) packet.checksum;
  packet.checksum = 0;
  char *raw_packet = (char *) &packet;
  uint32_t crc = crc32(0, raw_packet, sizeof(struct pkt));
  if (crc == checksum) {
    return true;
  }
  else {
    return false;
  }
}

void queue_new(struct queue *q) {
  q->head = 0;
  q->tail = 0;
  q->size = 0;
  memset(q->buffer, 0, QUEUE_SIZE * sizeof(struct pkt));
}

bool queue_empty(struct queue q) {
  if (q.head == q.tail) {
    return true;
  }
  else {
    return false;
  }
}

struct pkt queue_at(struct queue q, int i) {
  return q.buffer[QUEUE_WRAP(q.tail + i)];
}

bool enqueue(struct queue *q, struct pkt p) {
  int next = QUEUE_WRAP(q->head + 1);

  // buffer is full, overwrite tail
  if (next == q->tail) {
    q->tail = QUEUE_WRAP(q->tail + 1);
  }

  q->buffer[q->head] = p;
  q->head = next;
  q->size++;
  return true;
}

bool dequeue(struct queue *q, struct pkt *p) {
  // if the head isn't ahead of the tail, it's empty
  if (q->head == q->tail)
      return false;  // quit with an error

  // give the dequeue'd pkt to the sender if they want it
  if (p != NULL) {
    *p = q->buffer[q->tail];
  }

  // clear the data (optional)
  q->buffer[q->tail].checksum = -1;
  q->buffer[q->tail].acknum = -1;
  memset(q->buffer[q->tail].payload, 0, MESSAGE_LENGTH);

  q->tail = QUEUE_WRAP(q->tail + 1);

  q->size--;
  return true;
}

