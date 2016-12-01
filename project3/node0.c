#include "project3.h"
#include "node_common.h"

struct Node0 {
  int distance_table[MAX_NODES][MAX_NODES];
  struct NeighborCosts *neighbor;
} N0;

void rtinit0() {
  N0.neighbor = getNeighborCosts(0);

  //initialize the diagonal of our cost matrix to 0
  //and initialize our first row/column
  for (int i = 0; i < MAX_NODES; i++) {
    N0.distance_table[i][i] = 0;
    N0.distance_table[0][i] = N0.neighbor->NodeCosts[i];
  }

  //send update packets to all my neighbors
  for (int i = 0; i < N0.neighbor->NodesInNetwork; i++) {
    struct RoutePacket pkt;
    pkt.destid = 0;
    pkt.sourceid = i;
    for (int j = 0; j < MAX_NODES; j++) {
      pkt.mincost[j] = N0.distance_table[0][j];
    }

    toLayer2(pkt);
  }
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {

}
