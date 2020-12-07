#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"
#include "memory.h"

typedef struct
{
	uint8_t valid_bit;
	uint32_t mem_tag; // Number OF group
	uint8_t mem_data[64];
}CacheLine;

extern CacheLine cacheline[];
void init_cache();
uint32_t cache_read(paddr_t paddr,size_t len,CacheLine *cache);
void cache_write(paddr_t paddr,size_t len,uint32_t data,CacheLine *cache);

#endif
