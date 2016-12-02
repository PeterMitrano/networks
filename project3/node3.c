#include "project3.h"
#include "node_common.h"

Node N3;

void rtinit3() {
  init(3, &N3);
}


void rtupdate3(struct RoutePacket *rcvdpkt) {
  update(3, &N3, rcvdpkt);
}

