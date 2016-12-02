#include "project3.h"
#include "node_common.h"

Node N0;

void rtinit0() {
  init(0, &N0);

  printf(MAG "Init 0: " RESET);
  my_printdt(0, N0.distance_table);

  update_neighbors(0, N0);
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
  update(0, &N0, rcvdpkt);
}
