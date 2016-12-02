#include <algorithm>
#include <stdio.h>

#include "node_common.h"

void init(int MyNodeNumber, struct Node *node) {

    printf("At time t=%1.3f, init %i called\n", clocktime, MyNodeNumber);

  // initialize the diagonal of our cost matrix to 0
  // and initialize our first row/column
  // everything else is INFINITY
  for (int i = 0; i < MAX_NODES; i++) {
    for (int j = 0; j < MAX_NODES; j++) {
      node->distance_table[i][j] = INFINITY;
    }
  }

  // neighors is only so we know who we're directly connected too
  node->neighbors = getNeighborCosts(MyNodeNumber);
  for (int i = 0; i < MAX_NODES; i++) {
    node->distance_table[MyNodeNumber][i] = node->neighbors->NodeCosts[i];
    node->distance_table[i][i] = 0;
  }

  if (TraceLevel == -1) {
    my_printdt(MyNodeNumber, node->distance_table);
  }
  else {
    printdt(MyNodeNumber, node->neighbors, node->distance_table);
  }

  update_neighbors(MyNodeNumber, *node);
}

void update(int MyNodeNumber, struct Node *node, struct RoutePacket *rcvdpkt) {
  //save the new routing vector for rcvdpkt->sourceid
  for (int j = 0; j < MAX_NODES; j++) {
    node->distance_table[rcvdpkt->sourceid][j] = rcvdpkt->mincost[j];
  }

  bool update = false;
  for (int y = 0; y < MAX_NODES; y++) {
    // compute min{c(x,v) + d(v,y)} for all v in N
    for (int v = 0; v < MAX_NODES; v++) {
      int d = node->distance_table[MyNodeNumber][v] + node->distance_table[v][y]; // c(0,v) + D(v,y)

      if (d < node->distance_table[MyNodeNumber][y]) {
        update = true;
        node->distance_table[MyNodeNumber][y] = d;
      }
    }
  }

  if (TraceLevel == -1) {
    my_printdt(MyNodeNumber, node->distance_table);
  }
  else {
    printdt(MyNodeNumber, node->neighbors, node->distance_table);
  }

  if (update) {
    // send update to all neighbors...
    update_neighbors(MyNodeNumber, *node);
  }
}

void update_neighbors(int MyNodeNumber, struct Node node) {
  //send update packets to all my neighbors
  for (int i = 0; i < MAX_NODES; i++) {
    if (i == MyNodeNumber || node.neighbors->NodeCosts[i] == INFINITY) {
      continue;
    }

    struct RoutePacket pkt;
    pkt.sourceid = MyNodeNumber;
    pkt.destid = i;

    printf("At time t=%1.3f, node %i sends packet to node %i with: { ",
        clocktime, MyNodeNumber, i);

    for (int j = 0; j < MAX_NODES; j++) {
      pkt.mincost[j] = node.distance_table[MyNodeNumber][j];
      printf("%i ", pkt.mincost[j]);
    }

    printf("}\n");
    toLayer2(pkt);
  }
}

void my_printdt(int MyNodeNumber, int distance_table[MAX_NODES][MAX_NODES]) {
  printf(BLU "D%d\n" RESET, MyNodeNumber);
  printf("---------------------------\n" GRN);
  for (int i=0;i<MAX_NODES;i++) {
    for (int j=0;j<MAX_NODES;j++) {
      printf("%4d ", distance_table[i][j]);
    }
    printf("\n");
  }
  printf("\n" RESET);
}

/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  int cost[MAX_NODES][MAX_NODES]: This is the running record of the
//                 current costs as seen by this node.  It is
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt(int MyNodeNumber, struct NeighborCosts *neighbor,
    int costs[MAX_NODES][MAX_NODES]) {
  int i, j;
  int TotalNodes = neighbor->NodesInNetwork;
  int NumberOfNeighbors = 0;
  int Neighbors[MAX_NODES] = {0};

  // Determine our neighbors
  for (i = 0; i < TotalNodes; i++) {
    if ((neighbor->NodeCosts[i] != INFINITY) && i != MyNodeNumber) {
      Neighbors[NumberOfNeighbors] = i;
      NumberOfNeighbors++;
    }
  }

  // Print the header
  printf("                via     \n");
  printf("   D%d |", MyNodeNumber);
  for (i = 0; i < NumberOfNeighbors; i++)
    printf("     %d", Neighbors[i]);
  printf("\n");
  printf("  ----|-------------------------------\n");

  // For each node, print the cost by travelling thru each of our neighbors
  for (i = 0; i < TotalNodes; i++)   {
    if (i != MyNodeNumber)  {
      printf("dest %d|", i);
      for (j = 0; j < NumberOfNeighbors; j++)  {
        int d = costs[MyNodeNumber][i];
        printf("  %4d", std::min(INFINITY, d));
      }
      printf("\n");
    }
  }
  printf("\n");
}    // End of printdt0

