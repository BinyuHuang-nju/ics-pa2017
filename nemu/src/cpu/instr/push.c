#include "cpu/instr.h"

static void instr_execute_1op()
{
	assert(data_size == 32);
	operand_read(&opr_src);
	OPERAND r;
	r.type=OPR_MEM;
	r.sreg=SREG_SS;
	cpu.esp=cpu.esp-(data_size/8);
	r.addr=cpu.esp;
	r.data_size=data_size;
	r.val=sign_ext(opr_src.val,data_size);

	operand_write(&r);
}

//make_instr_impl_1op(push,r,v)   //50~57
make_instr_impl_1op(push,rm,v)   //ff 71 fc : pushl -0x4(%ecx)
make_instr_impl_1op(push,i,b)//6a 63:push $0x63
make_instr_impl_1op(push,i,v)//68

make_instr_func(push_r_v)
{
	assert(data_size == 32);
	uint32_t tempESP = cpu.esp;
	cpu.esp = cpu.esp-4;
	if(opcode == 0x54)
		vaddr_write(cpu.esp,2,4,tempESP);
	else
	{
		uint32_t value = cpu.gpr[opcode-0x50].val;
		vaddr_write(cpu.esp,2,4,value);
	}
	return 1;
}

make_instr_func(pusha)
{
	uint32_t temp = cpu.esp;
	cpu.esp = cpu.esp-4;
	vaddr_write(cpu.esp,SREG_SS,4,cpu.eax);
	cpu.esp = cpu.esp-4;
	vaddr_write(cpu.esp,SREG_SS,4,cpu.ecx);
	cpu.esp = cpu.esp-4;
	vaddr_write(cpu.esp,SREG_SS,4,cpu.edx);
	cpu.esp = cpu.esp-4;
	vaddr_write(cpu.esp,SREG_SS,4,cpu.ebx);
	cpu.esp = cpu.esp-4;
	vaddr_write(cpu.esp,SREG_SS,4,temp);
	cpu.esp = cpu.esp-4;
	vaddr_write(cpu.esp,SREG_SS,4,cpu.ebp);
	cpu.esp = cpu.esp-4;
	vaddr_write(cpu.esp,SREG_SS,4,cpu.esi);
	cpu.esp = cpu.esp-4;
	vaddr_write(cpu.esp,SREG_SS,4,cpu.edi);
	return 1;
}//60
