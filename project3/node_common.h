#pragma once

#include <stdio.h>

#include "project3.h"

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET  "\x1B[0m"


extern int TraceLevel;
extern float clocktime;

struct Node {
  int distance_table[MAX_NODES][MAX_NODES];
  struct NeighborCosts *neighbor;
};

void my_printdt(int MyNodeNumber,
    int distance_table[MAX_NODES][MAX_NODES]);

void printdt(int MyNodeNumber, struct NeighborCosts *neighbor,
    int cost[MAX_NODES][MAX_NODES]);

void init(int MyNodeNumber, struct Node *node);

void update_neighbors(int MyNodeNumber, struct Node node);
