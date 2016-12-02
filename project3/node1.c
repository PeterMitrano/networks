#include "project3.h"
#include "node_common.h"

Node N1;

void rtinit1() {
  init(1, &N1);
}


void rtupdate1(struct RoutePacket *rcvdpkt) {
  update(1, &N1, rcvdpkt);
}
