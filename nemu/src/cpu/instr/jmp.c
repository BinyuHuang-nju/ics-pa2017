#include "cpu/instr.h"

make_instr_func(jmp_near) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	print_asm_1("jmp", "", 2, &rel);
//	printf("Old eip in jmp/e9 :%x\n",cpu.eip);
	cpu.eip += offset;
//	printf("jmp to in jmp/e9 %x\n",cpu.eip+1+data_size/8);
        return 1 + data_size / 8;
}

make_instr_func(jmp_short)
{
	OPERAND rel;
	rel.type=OPR_IMM;
	rel.sreg=SREG_CS;
	rel.data_size=8;
	rel.addr= eip + 1;

	operand_read(&rel);

	int offset=sign_ext(rel.val,8);
//	print_asm_1("jmp","", 2,&rel);

	cpu.eip=cpu.eip+offset;

	return 2;
}
//eb 51:jmp 3008d

make_instr_func(jmp_rm_v)
{
	OPERAND rm;
	rm.type=OPR_IMM;
//	rm.addr=eip+1;
	rm.data_size=data_size;
	modrm_rm(eip+1,&rm);

	operand_read(&rm);
	if(data_size==16)
		cpu.eip=rm.val&0xffff;
	else
		cpu.eip=rm.val;
	return 0;
}
//ff/4 24 95 54 6a 03 00
//jmp *0x36a54(,%edx,4)
