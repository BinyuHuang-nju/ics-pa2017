
#include "cpu/instr.h"

static void instr_execute_1op()
{
	operand_read(&opr_src);
/*	
	opr_src.val=opr_src.val-1;
	set_PF(opr_src.val);
	set_ZF(opr_src.val);
	set_SF(opr_src.val);
	if(sign(opr_src.val+1)!=0)
	{
		if(sign(opr_src.val)==0)
			cpu.eflags.OF=1;
		else
			cpu.eflags.OF=0;
	}
	else
		cpu.eflags.OF=0;
*/
	uint32_t temp=cpu.eflags.CF;
	opr_src.val=alu_sub(1,opr_src.val);
	cpu.eflags.CF=temp;

	operand_write(&opr_src);
}

make_instr_impl_1op(dec,r,v)//48 - 4f
make_instr_impl_1op(dec,rm,v)//ff
//make_instr_impl_1op(dec,rm,b)//fe
