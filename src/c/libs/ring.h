#ifndef __RING_H__
#define __RING_H__

#include <stdint.h>

#define RING_TEMP_BUF_SIZE 0x20

typedef struct Ring{
    uint32_t size;
	uint32_t data_size;
    uint32_t roff;
    uint32_t woff;
	uint8_t temp_buf[RING_TEMP_BUF_SIZE];
    uint8_t data[];
}Ring;


Ring* create_ring(uint32_t size);

uint32_t get_ring_size(Ring* ring);

uint32_t get_data_size(Ring* ring);

uint32_t get_remain_size(Ring* ring);

void delete_ring(Ring* ring);

int ring_write_data(Ring* ring, uint8_t* data, uint32_t len);
int ring_read_data(Ring* ring, uint8_t* data, uint32_t len);

int ring_is_empty(Ring* ring);
int ring_is_full(Ring* ring);

#endif

