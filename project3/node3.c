#include "project3.h"
#include "node_common.h"

struct Node3 {
  int costs[MAX_NODES][MAX_NODES];
  struct NeighborCosts *neighbor;
} N3;

void rtinit3() {
  N3.neighbor = getNeighborCosts(3);
}


void rtupdate3(struct RoutePacket *rcvdpkt) {

}

