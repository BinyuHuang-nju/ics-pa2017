#include "cpu/instr.h"

make_instr_func(leave)
{
	OPERAND popp;
	popp.type=OPR_MEM;
	popp.sreg=SREG_SS;
	popp.data_size=data_size;
	
	cpu.esp=cpu.ebp;

	popp.addr=cpu.esp;
	operand_read(&popp);

//	cpu.esp=cpu.ebp;
	assert(data_size == 32);
	cpu.esp=cpu.esp+4;
	cpu.ebp=sign_ext(popp.val,data_size);
//	cpu.ebp = vaddr_read(cpu.esp,2,data_size/8);	
//	operand_write(&popp);

	return 1;
}
