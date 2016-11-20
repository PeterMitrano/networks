#include <stdbool.h>

#include "student2_common.h"
#include "crc32.h"

int compute_checksum(struct pkt packet) {
  char *raw_packet = (char *) &packet;
  uint32_t crc = crc32(0, raw_packet, sizeof(struct pkt));
  return crc;
}

bool verify_checksum(struct pkt packet, uint32_t checksum) {
  char *raw_packet = (char *) &packet;
  uint32_t crc = crc32(0, raw_packet, sizeof(struct pkt));
  return (crc == checksum);
}
