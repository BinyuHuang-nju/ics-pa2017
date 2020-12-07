#include "cpu/instr.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);

	opr_dest.val=alu_and(sign_ext(opr_src.val,opr_src.data_size),opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(and,i,rm,bv)//83 e4 f8  and $0xfffffff8,%esp
make_instr_impl_2op(and,rm,r,b)//22 45 fb and -0x5(%ebp),%al
make_instr_impl_2op(and,r,rm,v)//21
make_instr_impl_2op(and,i,rm,v)//81 /4
make_instr_impl_2op(and,i,rm,b)//80 /4
make_instr_impl_2op(and,i,a,v)//25
