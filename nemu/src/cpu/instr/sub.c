#include "cpu/instr.h"
#include "cpu/alu.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val=alu_sub(sign_ext(opr_src.val,opr_src.data_size),opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(sub,i,rm,bv)//83 ec 10:sub $0x10,%esp
make_instr_impl_2op(sub,rm,r,v)//2b
make_instr_impl_2op(sub,r,rm,v)//29
make_instr_impl_2op(sub,i,rm,v)//81
make_instr_impl_2op(sub,i,a,v)//2d
