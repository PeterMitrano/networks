#include "project3.h"
#include "node_common.h"

Node N2;

void rtinit2() {
  init(2, &N2);
  printf(MAG "Init 2: " RESET);
  my_printdt(2, N2.distance_table);
}


void rtupdate2(struct RoutePacket *rcvdpkt) {

}
