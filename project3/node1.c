#include "project3.h"
#include "node_common.h"

Node N1;

void rtinit1() {
  init(1, &N1);
  printf(MAG "Init 1: " RESET);
  my_printdt(1, N1.distance_table);
}


void rtupdate1(struct RoutePacket *rcvdpkt) {
  bool update = false;
  for (int y = 0; y < MAX_NODES; y++) {
    // compute cost from 0 to y
    // rcvdpkt->sourceid is v
    int c = N1.neighbor->NodeCosts[rcvdpkt->sourceid]; // c(0,rcvdpkt->sourceid)
    int d = rcvdpkt->mincost[y]; // D(rcvdpkt->sourceid, y);

    if ((c + d) < N1.distance_table[1][y]) {
      update = true;
      N1.distance_table[1][y] = c + d;
    }
  }

  my_printdt(1, N1.distance_table);

  if (update) {
    // send update to all neighbors...
    update_neighbors(1, N1);
  }
}
