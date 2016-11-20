#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "project2.h"

// 20 + 3 * sizeof(int) = 32
#define TOTAL_PACKET_SIZE 32

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define RESET  "\x1B[0m"

int compute_checksum(struct pkt packet);
bool verify_checksum(struct pkt packet, uint32_t checksum);
