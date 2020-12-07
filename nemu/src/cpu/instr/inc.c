#include "cpu/instr.h"

static void instr_execute_1op()
{
	operand_read(&opr_src);
	
	uint32_t temp=cpu.eflags.CF;
	opr_src.val=alu_add(1,opr_src.val);
	cpu.eflags.CF=temp;

	operand_write(&opr_src);
}

make_instr_impl_1op(inc,rm,v)//ff 45 f4:incl -0xc(%ebp)
make_instr_impl_1op(inc,r,v)//40 - 47
