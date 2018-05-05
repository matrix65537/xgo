#include <stdlib.h>
#include <string.h>
#include "ring.h"

Ring* create_ring(uint32_t size)
{
    Ring* ring;
    uint8_t* p;
    uint32_t total_size;

    total_size = sizeof(Ring) + size;

    p = malloc(total_size);
    memset(p, 0x00, total_size);
    ring = (Ring*)p;
    ring->size = size;
    return ring;
}

uint32_t get_ring_size(Ring* ring)
{
    return ring->size;
}

uint32_t get_data_size(Ring* ring)
{
	return ring->data_size;
}

uint32_t get_remain_size(Ring* ring)
{
    uint32_t remain_size;
    remain_size = ring->size - get_data_size(ring);
    return remain_size;
}

int ring_is_empty(Ring* ring)
{
	return ring->data_size == 0;
}

int ring_is_full(Ring* ring)
{
	return ring->data_size == ring->size;
}

void delete_ring(Ring* ring)
{
    free(ring);
}

int ring_write_data(Ring* ring, uint8_t* data, uint32_t len)
{
	uint32_t i = 0;
	if(len > get_remain_size(ring))
	{
		return -1;
	}
	while(i < len)
	{
		ring->data[ring->woff++] = data[i];
		if(ring->woff == ring->size)
		{
			ring->woff = 0;
		}
		i++;
	}
	ring->data_size += len;
	return len;
}

int ring_read_data(Ring* ring, uint8_t* data, uint32_t len)
{
	uint32_t i = 0;
	if(len > get_data_size(ring))
	{
		return -1;
	}
	while(i < len)
	{
		data[i] = ring->data[ring->roff++];
		if(ring->roff == ring->size)
		{
			ring->roff = 0;
		}
		i++;
	}
	ring->data_size -= len;
	return len;
}
