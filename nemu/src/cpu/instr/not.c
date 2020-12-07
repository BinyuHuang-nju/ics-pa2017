#include "cpu/instr.h"

static void instr_execute_1op()
{
	operand_read(&opr_src);
	if(opr_src.data_size==8)
		opr_src.val=opr_src.val^0xff;
	else if(opr_src.data_size==16)
		opr_src.val=opr_src.val^0xffff;
	else
		opr_src.val=opr_src.val^0xffffffff;

	operand_write(&opr_src);
}

make_instr_impl_1op(not,rm,v)//f7