#include "cpu/instr.h"

make_instr_func(loopne_b)
{
	uint32_t countReg = 0;
	if(data_size == 16)
		countReg = cpu.gpr[1]._16;
	else
		countReg = cpu.ecx;
	countReg = countReg-1;
	if(countReg!=0 && cpu.eflags.ZF==0)
	{
//		printf("in\n");
		OPERAND rel;
		rel.type = OPR_IMM;
		rel.sreg = SREG_CS;
		rel.data_size = data_size;
		rel.addr = eip + 1;
		operand_read(&rel);
		int offset = sign_ext(rel.val,data_size);
		cpu.eip += offset;
	}
	return 1+(data_size/8);
}
//e0
