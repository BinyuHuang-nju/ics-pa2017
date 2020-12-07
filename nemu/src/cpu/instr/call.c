#include "cpu/instr.h"
/*
static void instr_execute_1op()
{
	OPERAND rel;
	rel.type=OPR_MEM;
	cpu.esp=cpu.esp-(data_size/8);
	rel.addr=cpu.esp;
	rel.data_size=data_size;
	rel.val=cpu.eip+1+(data_size/8);
	opr_src.type=OPR_MEM;

	operand_write(&rel);
	
	opr_src.addr=cpu.eip+1;
	operand_read(&opr_src);
	cpu.eip+=opr_src.val;
}
*/
/*
static void instr_execute_1op()
{
	OPERAND rel;
	cpu.esp=cpu.esp-data_size/8;
	rel.type=OPR_MEM;
//	rel.type=OPR_IMM;
	rel.addr=cpu.esp;
	rel.data_size=data_size;
	rel.val=cpu.eip+1+(data_size/8);
	operand_write(&rel);

	opr_src.addr=cpu.eip+1;
	opr_src.data_size=data_size;
	operand_read(&opr_src);
//	printf("Old eip in call/e8:%x\n",cpu.eip);
	cpu.eip=cpu.eip+sign_ext(opr_src.val,opr_src.data_size);
//	printf("jmp to in call/e8:%x\n",cpu.eip+1+data_size/8);
}

make_instr_impl_1op(call,i,v)//e8
*/
make_instr_func(call_i_v)//e8
{
	OPERAND m,r;
//	assert(data_size==32);
	cpu.esp = cpu.esp-data_size/8;
	r.type = OPR_MEM;
	r.addr = cpu.esp;
	r.sreg = SREG_SS;
	r.data_size = data_size;
	r.val = cpu.eip+1+(data_size/8);
	operand_write(&r);
	
	m.type = OPR_IMM;
	m.data_size = data_size;
	m.addr = eip+1;
	operand_read(&m);

	cpu.eip = cpu.eip+sign_ext(m.val,m.data_size);

	return 1+data_size/8;
}
make_instr_func(call_rm_v)
{
	OPERAND rm,opr_eip;
	int len = 1;
	cpu.esp = cpu.esp-data_size/8;

	rm.data_size = data_size;
	len += modrm_opcode_rm(eip+len, &opcode, &rm);
	operand_read(&rm);

	opr_eip.data_size = data_size;
	opr_eip.type = OPR_MEM;
	opr_eip.addr = cpu.esp;
	opr_eip.val = eip+len;
	opr_eip.sreg = SREG_SS;
	operand_write(&opr_eip);

	cpu.eip = (data_size==16)?(rm.val & 0xffff) : rm.val;
	return 0;
}
//ff /2
/*
make_instr_func(call_rm_v)
{
	OPERAND ps;
	assert(data_size==32);
	cpu.esp=cpu.esp-data_size/8;
//	ps.type=OPR_MEM;
//	ps.addr=cpu.esp;
	ps.type = OPR_REG;
	ps.addr = REG_EAX;
	ps.data_size=data_size;
//	ps.val=cpu.eip;
//	operand_write(&ps);
	operand_read(&ps);

	OPERAND rm;
	rm.type=OPR_MEM;
	rm.data_size=data_size;
//	modrm_rm(eip+1,&rm);
	rm.addr = cpu.esp;
	rm.val = cpu.eip+1+(data_size/8);

//	operand_read(&rm);
//	cpu.eip=(data_size==16)?(rm.val&0xffff):rm.val;
	cpu.eip = ps.val;
	operand_write(&rm);
	return 0;
}//ff /2
*/
