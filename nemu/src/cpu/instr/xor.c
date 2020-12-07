#include "cpu/instr.h"
#include "cpu/alu.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);

	opr_dest.val=alu_xor(sign_ext(opr_src.val,opr_src.data_size),opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(xor,r,rm,v)//31 c7 xor %eax,%edi
