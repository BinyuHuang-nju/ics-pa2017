#include "cpu/instr.h"
#include "cpu/alu.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);

	opr_dest.val=alu_or(sign_ext(opr_src.val,opr_src.data_size),opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(or,r,rm,v)//09
make_instr_impl_2op(or,rm,r,b)//0a
make_instr_impl_2op(or,i,rm,bv)//83
