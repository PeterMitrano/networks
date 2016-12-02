#include "project3.h"
#include "node_common.h"

Node N3;

void rtinit3() {
  init(3, &N3);
  printf(MAG "Init 3: " RESET);
  my_printdt(3, N3.distance_table);
}


void rtupdate3(struct RoutePacket *rcvdpkt) {

}

