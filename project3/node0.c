#include "project3.h"
#include "node_common.h"

Node N0;

void rtinit0() {
  init(0, &N0);
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
  update(0, &N0, rcvdpkt);
}

void fini0() {
  fini(0, N0);
}
