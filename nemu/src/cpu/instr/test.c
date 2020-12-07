#include "cpu/instr.h"
#include "cpu/alu.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	uint32_t temp = alu_and(sign_ext(opr_src.val,opr_src.data_size),opr_dest.val);
	if(opr_src.data_size == 8 || opr_dest.data_size == 8)
		cpu.eflags.SF = ( temp & 0x80) ? 1:0;
}

make_instr_impl_2op(test,r,rm,v)//85 c0: test %eax,%eax
make_instr_impl_2op(test,r,rm,b)//84 c0:test %al,%al
make_instr_impl_2op(test,i,rm,v)//f7 /0
make_instr_impl_2op(test,i,a,v)//a9
make_instr_impl_2op(test,i,a,b)//a8
make_instr_impl_2op(test,i,rm,b)//f6 /0
