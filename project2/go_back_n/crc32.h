#pragma once
#include <stdint.h>
#include <sys/param.h>

uint32_t crc32(uint32_t crc, const void *buf, size_t size);
