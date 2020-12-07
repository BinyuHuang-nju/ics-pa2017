#include "cpu/cpu.h"
#include "memory/memory.h"
#include "memory/mmu/segment.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
//	printf("%x\n",sreg);
//	assert(sreg <= 5 && sreg >= 0);
	uint32_t res = cpu.segReg[sreg].base + offset;
	return res;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
//	assert(segDesc.present == 1);
//	assert(segDesc.granularity == 1);
//	assert(segDesc.base_15_0 == 0 && segDesc.base_23_16 == 0 && segDesc.base_31_24 ==0);
//	assert(segDesc.limit_15_0 == 0xffff && segDesc.limit_19_16 ==0xf);
	
//	printf("%d\n",sreg);
	assert(sreg>=0 && sreg <=5);
	assert(cpu.segReg[sreg].ti == 0); //gdt
	uint32_t addrSeg = cpu.gdtr.base + cpu.segReg[sreg].index*8;//8 : the length of segDesc
	uint32_t val[2] = {0,0};
//	val[0] = vaddr_read(addrSeg,SREG_DS,4);
//	val[1] = vaddr_read(addrSeg+4,SREG_DS,4);
	val[0] = paddr_read(addrSeg,4);
	val[1] = paddr_read(addrSeg+4,4);
//	printf("%x %x %x %x\n",cpu.segReg[0].val,cpu.segReg[1].val,cpu.segReg[2].val,
//		cpu.segReg[3].val);
//	printf("%x %x %x %x %x\n",cpu.gdtr.base,cpu.segReg[sreg].val,val[0],val[1],addrSeg);
	cpu.segReg[sreg].limit = (val[0]&0xffff) + (val[1]&0xf0000);
	cpu.segReg[sreg].base = (val[0]>>16) + ((val[1]&0xff)<<16) + (val[1]&0xff000000);
	cpu.segReg[sreg].type = (val[1]>>8) & 0xf;
	cpu.segReg[sreg].privilege_level = (val[1]>>13) & 0x3;
	cpu.segReg[sreg].soft_use = (val[1]>>20) & 0x1;
	uint8_t present0 = (val[1]>>15) & 0x1;
	uint8_t granularity0 = (val[1]>>23) & 0x1;
//	printf("%x %x %x %x %x\n",cpu.segReg[sreg].limit,cpu.segReg[sreg].base,cpu.segReg[sreg].type,
//		cpu.segReg[sreg].privilege_level,cpu.segReg[sreg].soft_use);
	assert(present0 == 1);
	assert(granularity0 ==1);
	assert(cpu.segReg[sreg].limit == 0xfffff);
	assert(cpu.segReg[sreg].base == 0);
}
