#include "project3.h"
#include "node_common.h"

struct Node1 {
  int distance_table[MAX_NODES][MAX_NODES];
  struct NeighborCosts *neighbor;
} N1;

void rtinit1() {
  N1.neighbor = getNeighborCosts(1);
}


void rtupdate1(struct RoutePacket *rcvdpkt) {

}
