#pragma once

#include "project3.h"

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET  "\x1B[0m"

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

extern int TraceLevel;
extern float clocktime;

void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor,
    struct distance_table *dtptr);
