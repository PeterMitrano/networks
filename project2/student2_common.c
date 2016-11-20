#include <stdbool.h>
#include <string.h>

#include "student2_common.h"
#include "crc32.h"

int compute_checksum(struct pkt packet) {
  char *raw_packet = (char *) &packet;
  uint32_t crc = crc32(0, raw_packet, sizeof(struct pkt));
  return crc;
}

bool verify_checksum(struct pkt packet) {
  uint32_t checksum = (uint32_t) packet.checksum;
  packet.checksum = 0;
  char *raw_packet = (char *) &packet;
  uint32_t crc = crc32(0, raw_packet, sizeof(struct pkt));
  return (crc == checksum);
}

void queue_new(struct queue *q) {
  q->head = 0;
  q->tail = 0;
  memset(q->buffer, 0, QUEUE_SIZE * sizeof(struct pkt));
}

bool queue_empty(struct queue q) {
  return (q.head == q.tail);
}

bool enqueue(struct queue *q, struct pkt p) {
  int next = QUEUE_WRAP(q->head + 1);

  // buffer is full, overwrite tail
  if (next == q->tail) {
    q->tail = QUEUE_WRAP(q->tail + 1);
  }

  q->buffer[q->head] = p;
  q->head = next;
  return true;
}

bool dequeue(struct queue *q, struct pkt *p) {
  // if the head isn't ahead of the tail, it's empty
  if (q->head == q->tail)
      return false;  // quit with an error

  *p = q->buffer[q->tail];
  // clear the data (optional)
  q->buffer[q->tail].checksum = -1;
  q->buffer[q->tail].acknum = -1;
  memset(q->buffer[q->tail].payload, 0, MESSAGE_LENGTH);

  q->tail = QUEUE_WRAP(q->tail + 1);

  return true;
}

