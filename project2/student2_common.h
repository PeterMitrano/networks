#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "project2.h"

// 20 + 3 * sizeof(int) = 32
#define TOTAL_PACKET_SIZE 32

#define QUEUE_SIZE 128 // must be power of 2

// index wrapping macro
#define QUEUE_WRAP(i) ((i) & (QUEUE_SIZE - 1))

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET  "\x1B[0m"

extern int TraceLevel;
extern int NumMsgsCorrupt;
extern int corrupt_count;
extern double AveTimeBetweenMsgs;

void debug_print(char *prefix, char *color, struct pkt packet);
void print_packet(struct pkt packet);

void set_checksum(struct pkt *packet);
bool verify_checksum(struct pkt packet);

struct queue {
  int head;
  int tail;
  int size;
  struct pkt buffer[QUEUE_SIZE];
};

void queue_new(struct queue *q);
bool queue_empty(struct queue q);
struct pkt queue_at(struct queue q, int i);
bool enqueue(struct queue *q, struct pkt p);
bool dequeue(struct queue *q, struct pkt *p);
