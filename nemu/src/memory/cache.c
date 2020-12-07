#include "memory/cache.h"
#include "nemu.h"
#include "cpu/cpu.h"
#include <time.h>
#include <stdlib.h>

CacheLine cacheline[1024];
//CacheLine cacheline[128][8];
//pow(2,16)B=pow(2,7) groups * pow(2,3) lines/group * pow(2,6) B/line
//Number of cache group = (Number 0f memory block) mod 128
// m + b = 32 , m = 24 ,b = 6,        
// m = 24 = 17 + 7 ,7:Number of group.     
//                 --------------------------------------
//  memory address:| 19b-tag  |  7b-numgroup | 6b-block |
//                 --------------------------------------
void init_cache()
{
/*	for(int i=0;i<128;i++)
		for(int j=0;j<8;j++)
			cacheline[i][j].valid_bit=0;
*/
	for(int i=0;i<1024;i++)
		cacheline[i].valid_bit=0;
}

uint32_t cache_read(paddr_t paddr,size_t len,CacheLine *cache)
{
	if(paddr%64 + len > 64) // data not in just one block
	{
		uint32_t ret = 0;
		memcpy(&ret, hw_mem + paddr, len);
		return ret;
	}
	bool cache_hit=false;
	uint32_t NumGroup=(paddr>>6)%128;
	uint32_t i;
	for(i=NumGroup*8;i<NumGroup*8+8;i++)
	{
		/*if(cache[i].valid_bit==0)
			cache_hit=false;*/
		if(cache[i].valid_bit==1 && cache[i].mem_tag==(paddr>>13))
		{
			cache_hit=true;
			break;	
		}
	}
	if(cache_hit==true) //read
	{
		uint32_t ret = 0;
		memcpy(&ret, cache[i].mem_data+(paddr%64), len);
		return ret;
	}
	else //cache_hit == false
	{
		bool isFull=true;
		uint32_t j;
		for(j=NumGroup*8;j<NumGroup*8+8;j++)
		{
			if(cache[j].valid_bit==0)
			{
				isFull=false;
				break;
			}
		}
		uint32_t NumLine;
		if(isFull==false)
		{
			NumLine=j;
			cache[NumLine].valid_bit=1;
			cache[NumLine].mem_tag=paddr>>13;
			memcpy(cache[NumLine].mem_data,hw_mem+(paddr & 0xffffffc0),64);
		}
		else //isFull == true
		{
			srand((int)time(NULL));
			uint32_t NumLine_ingroup=rand()%8;
			NumLine=NumGroup*8+NumLine_ingroup;
			cache[NumLine].mem_tag=paddr>>13;
			memcpy(cache[NumLine].mem_data,hw_mem+(paddr & 0xffffffc0),64);
		}
		uint32_t ret = 0;
		memcpy(&ret, cache[NumLine].mem_data+(paddr%64), len);
		return ret;
	}
}
void cache_write(paddr_t paddr,size_t len,uint32_t data,CacheLine *cache)
{	
	uint32_t NumGroup=(paddr>>6)%128;
	int i;
	bool cache_hit=false;
	for(i=NumGroup*8;i<NumGroup*8+8;i++)
	{
		if(cache[i].valid_bit==1 && cache[i].mem_tag==(paddr>>13))
		{
			cache_hit=true;
			break;
		}
	}
	if(cache_hit==true)//write through
	{
		if(paddr%64 + len > 64) // data not in just one block
		{	
			memcpy(hw_mem + paddr, &data, len);
			cache[i].valid_bit=0;
                paddr_t paddr1=paddr+len;
				uint32_t tag1=paddr1>>13;
				uint32_t group1=(paddr1>>6)%128;
				for(int j=group1*8;j<NumGroup*8+8;j++)
				{
					if(cache[j].mem_tag==tag1&&cache[j].valid_bit==1)
						cache[j].valid_bit=0;
				}
			return;
		}
		memcpy(hw_mem + paddr, &data, len);
		memcpy(cache[i].mem_data,(hw_mem+(paddr&0xffffffc0)),64);
	}
	else   //cache_hit == false
	{
		memcpy(hw_mem + paddr, &data, len);
	}
}

