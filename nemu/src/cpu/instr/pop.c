#include "cpu/instr.h"
/*
static void instr_execute_1op()
{
	operand_read(&opr_src);
	OPERAND r;
	r.type=OPR_MEM;
	r.data_size=data_size;
	r.sreg=SREG_SS;
	r.addr=cpu.esp;

	operand_read(&r);
	cpu.esp=cpu.esp+(data_size/8);
//    cpu.esp=cpu.esp+4;

	opr_src.val=sign_ext(r.val,data_size);
	operand_write(&opr_src);
}

make_instr_impl_1op(pop,r,v)//58 - 5f
*/
make_instr_func(pop_r_v)
{
	OPERAND m,r;
	m.data_size = data_size;
	m.type = OPR_MEM;
	m.sreg = SREG_SS;
	m.addr = cpu.esp;
	operand_read(&m);

	r.type = OPR_REG;
	r.data_size = data_size;
	r.addr = opcode & 0x7;
	r.val = m.val;
	operand_write(&r);

	cpu.esp = cpu.esp+(data_size/8);
	return 1;
}//58 - 5f. 58:010 11 000 . 000:eax

make_instr_func(popa)
{
	cpu.edi = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp = cpu.esp+4;
	cpu.esi = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp = cpu.esp+4;
	cpu.ebp = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp = cpu.esp+4;
	if(cpu.esp+20 != vaddr_read(cpu.esp,SREG_SS,4))
		printf("Not Equal in popa.\n");
	cpu.esp = cpu.esp+4;
	cpu.ebx = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp = cpu.esp+4;
	cpu.edx = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp = cpu.esp+4;
	cpu.ecx = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp = cpu.esp+4;
	cpu.eax = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp = cpu.esp+4;
	return 1;
}//61
