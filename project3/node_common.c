#include <stdio.h>

#include "node_common.h"

void init(int MyNodeNumber, struct Node *node){
  node->neighbor = getNeighborCosts(MyNodeNumber);

  // initialize the diagonal of our cost matrix to 0
  // and initialize our first row/column
  // everything else is INFINITY
  for (int i = 0; i < MAX_NODES; i++) {
    for (int j = 0; j < MAX_NODES; j++) {
      node->distance_table[i][j] = INFINITY;
    }
  }
  for (int i = 0; i < MAX_NODES; i++) {
    node->distance_table[i][i] = 0;
    node->distance_table[MyNodeNumber][i] = node->neighbor->NodeCosts[i];
    node->distance_table[i][MyNodeNumber] = node->neighbor->NodeCosts[i];
  }
}

void update_neighbors(int MyNodeNumber, struct Node node) {
  //send update packets to all my neighbors
  for (int i = 0; i < node.neighbor->NodesInNetwork; i++) {
    struct RoutePacket pkt;
    pkt.sourceid = MyNodeNumber;
    pkt.destid = i;
    for (int j = 0; j < MAX_NODES; j++) {
      pkt.mincost[j] = node.distance_table[0][j];
    }

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
                    printf("  %4d", costs[i][Neighbors[j]]);
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt0
