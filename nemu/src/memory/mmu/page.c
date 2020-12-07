#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
//	printf("\nPlease implement page_translate()\n");
//	assert(0);
	PDE pde;
	PTE pte;
	uint16_t pdir = ((unsigned int)laddr)>>22;
	uint16_t ptab = (laddr>>12)%1024;
	uint16_t poffset = laddr & 0xfff;
//	printf("dir page offset:%x %x %x\n",pdir,ptab,poffset);
	uint32_t VirAddrFir = ((cpu.cr3.PDBR)<<12) + pdir*4;
	pde.val = paddr_read(VirAddrFir,4);
	assert(pde.present==1);
	uint32_t VirAddrSec = (pde.page_frame<<12) + ptab*4;
	pte.val = paddr_read(VirAddrSec,4);
	assert(pte.present==1);
	uint32_t PhyAddr = (pte.page_frame<<12) + poffset;
	return PhyAddr;
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
