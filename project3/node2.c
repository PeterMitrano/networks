#include "project3.h"
#include "node_common.h"

Node N2;

void rtinit2() {
  init(2, &N2);
}


void rtupdate2(struct RoutePacket *rcvdpkt) {
  update(2, &N2, rcvdpkt);
}

void fini2() {
  fini(2, N2);
}
