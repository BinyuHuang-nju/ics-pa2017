#include "cpu/instr.h"

make_instr_func(lgdt_imm)
{
	int len = 1;
	OPERAND flimit,fbase;
	flimit.data_size = 16;
	fbase.data_size = 32;
	len += modrm_rm(eip+1,&flimit);
	operand_read(&flimit);
	fbase.addr = flimit.addr+2;
	fbase.type = flimit.type;
	operand_read(&fbase);

	cpu.gdtr.limit = flimit.val;
	if(data_size == 16)
		cpu.gdtr.base = fbase.val & 0xffffff;
	else
		cpu.gdtr.base = fbase.val;
	return len;
/*
	OPERAND find;
	find.type = OPR_MEM;
	find.sreg = SREG_DS;
	find.data_size = data_size;
	find.addr = eip+1;
	int len = modrm_rm(eip+1,&find);
	operand_read(&find);
	uint32_t limit = 0,base = 0;
	limit = laddr_read(find.addr,2) & 0xffff;
	base = laddr_read(find.addr+2,4);
	cpu.gdtr.limit = limit;
	cpu.gdtr.base = base;
	return len+1;
*/
/*
int len = 1;
	OPERAND find;
	find.type = OPR_IMM;
	find.sreg = SREG_CS;
	find.data_size = data_size;
	
	len += modrm_rm(eip+1,&find);
	find.data_size = data_size;

	find.addr=eip+2;
	operand_read(&find);
	
	uint32_t flimit = 0,fbase = 0;
	flimit = laddr_read(find.val,2);
	fbase = laddr_read(find.val+2,4);
	cpu.gdtr.limit = flimit & 0xffff;
	cpu.gdtr.base = fbase;
*/
/*	OPERAND flimit,fbase;
	flimit.type = fbase.type = OPR_MEM;
	flimit.sreg = fbase.sreg = SREG_DS;
	flimit.data_size = 16;
	fbase.data_size = 32;
	flimit.addr = find.val;
	fbase.addr = find.val + 2;
	
	operand_read(&flimit);
	operand_read(&fbase);

	cpu.gdtr.limit = flimit.val;
	cpu.gdtr.base = fbase.val;
*/
//	printf("%x %x\n",cpu.gdtr.limit,cpu.gdtr.base);

	return len;
}
