#include "cpu/instr.h"
//sr : special register, there,including control  & segment
make_instr_func(mov_cr2rm_v)
{
	int len=1;
	OPERAND rm;
	rm.data_size=data_size;
	uint8_t opc=0;
	len +=modrm_opcode_rm(eip+1,&opc,&rm);
	if(opc==0)
		rm.val = cpu.cr0.val;
	else if(opc == 3)
		rm.val = cpu.cr3.val;
	operand_write(&rm);
	return len;
}
//0f 20 c0:mov %cr0,%eax

make_instr_func(mov_rm2cr_v)
{
	int len=1;
	OPERAND rm;
	rm.data_size=data_size;
	uint8_t opc = 0;
	len +=modrm_opcode_rm(eip+1,&opc,&rm);
	operand_read(&rm);

	if(opc == 0)
		cpu.cr0.val=rm.val;
	else if(opc == 3)
		cpu.cr3.val=rm.val;
//	printf("%d\n",cpu.cr0.pe);
	return len;
}
//0f 22 c0:mov %eax,%cr0

make_instr_func(mov_rm2sr_v)
{
	int len=1;
	OPERAND rm,sr;
	rm.data_size=16;
	sr.data_size=16;
	len += modrm_r_rm(eip+1,&sr,&rm);
	sr.type=OPR_SREG;
	operand_read(&rm);
	sr.val=rm.val;
//	load_sreg(sr.addr);
//	printf("%x %x %x\n",sr.val,sr.addr,sr.sreg);
	operand_write(&sr);
	load_sreg(sr.addr);
	return len;
}
//8e d8:mov %eax,%ds d8: 11 011 000 , 011:ds , 00 000 : eax
// to load DS,SS,ES

make_instr_func(ljmp)
{
	OPERAND imm,sreg_cs;
	imm.type=OPR_IMM;
	imm.sreg=SREG_CS;
	imm.addr=eip+1;
	imm.data_size=data_size;
	operand_read(&imm);

	sreg_cs.type=OPR_IMM;
	sreg_cs.sreg=SREG_CS;
	sreg_cs.addr=eip+5;
	sreg_cs.data_size=16;
	operand_read(&sreg_cs);
	cpu.cs.val=sreg_cs.val;

	load_sreg(SREG_CS);
	cpu.eip=imm.val;
	return 0;
}
//ea 17 00 03 00 08 00 ljmp $0x8,$0x30017  : to load CS
