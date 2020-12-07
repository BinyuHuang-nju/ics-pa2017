#include "cpu/instr.h"

make_instr_func(ret_near)
{
	OPERAND rett;
//	rett.type=OPR_IMM;
	rett.type=OPR_MEM;
	rett.sreg=SREG_SS;
	rett.data_size=data_size;
	rett.addr=cpu.esp;

	operand_read(&rett);

	cpu.esp=cpu.esp+ data_size/8;
	cpu.eip=sign_ext(rett.val,data_size);


	return 0;
}
make_instr_func(ret_near_i)
{
	OPERAND rm;
	rm.addr = cpu.esp;
	rm.data_size = data_size;
	rm.type = OPR_MEM;
	rm.sreg = SREG_SS;
	operand_read(&rm);

	cpu.eip = rm.val;
	if(data_size == 16)
	{	
		cpu.esp +=2;
		cpu.eip = cpu.eip & 0x0000ffff;
	}
	else
		cpu.esp +=4;

	OPERAND imm;
	imm.addr = eip+1;
	imm.type = OPR_IMM;
	imm.sreg = SREG_CS;
	imm.data_size = 16;
	operand_read(&imm);

	cpu.esp += imm.val;

	return 0;
/*	OPERAND rett,rel;
//	rett.type=OPR_IMM;
	rett.type=OPR_MEM;
	rett.sreg=SREG_SS;
	rett.data_size=data_size;
	rett.addr=cpu.esp;

	operand_read(&rett);
	cpu.esp=cpu.esp+4;
	cpu.eip=sign_ext(rett.val,data_size);
	
	rel.type=OPR_IMM;
	rel.sreg=SREG_CS;
	rel.data_size=16;
	rel.addr=eip+1;
	operand_read(&rel);
	int offset=sign_ext(rel.val,16);
	cpu.esp=cpu.esp+offset;

	return 0;
*/
}
//c2 00 04:ret $0x4

make_instr_func(iret)
{
	cpu.eip = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp +=4;
	cpu.cs.val = (uint16_t)vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp +=4;
    cpu.cs.base = vaddr_read(cpu.esp,SREG_SS,4);
    cpu.esp+=4;
	cpu.eflags.val = vaddr_read(cpu.esp,SREG_SS,4);
	cpu.esp +=4;
	return 0;
}
//cf
