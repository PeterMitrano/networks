#include "project2.h"
#include "student2_common.h"

int main() {

  struct pkt pkt3;
  pkt3.seqnum = 3;

  struct pkt pkt2;
  pkt2.seqnum = 2;

  struct pkt pkt1;
  pkt1.seqnum = 1;

  struct pkt pkt_;

  struct queue q;
  queue_new(&q);
  enqueue(&q, pkt1);
  enqueue(&q, pkt2);
  dequeue(&q, &pkt_);
  enqueue(&q, pkt3);
  enqueue(&q, pkt1);
  enqueue(&q, pkt2);
  dequeue(&q, &pkt_);
  enqueue(&q, pkt3);
  enqueue(&q, pkt1);
  enqueue(&q, pkt2);
  dequeue(&q, &pkt_);
  dequeue(&q, &pkt_);
  dequeue(&q, &pkt_);
  dequeue(&q, &pkt_);
}
