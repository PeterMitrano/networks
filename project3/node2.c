#include "project3.h"
#include "node_common.h"

struct Node2 {
  distance_table dt;
  struct NeighborCosts *neighbor;
} N2;

void rtinit2() {
  N2.neighbor = getNeighborCosts(2);
}


void rtupdate2(struct RoutePacket *rcvdpkt) {

}
