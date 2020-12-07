#include "cpu/instr.h"
#include "cpu/alu.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(sign_ext(opr_src.val,opr_src.data_size),sign_ext(opr_dest.val,opr_dest.data_size));
}


make_instr_impl_2op(cmp,i,rm,bv)  //83 fb 01:cmp 0x1,%eax
make_instr_impl_2op(cmp,r,rm,v)//39 c1 :cmp %eax,%ecx
make_instr_impl_2op(cmp,i,rm,v)//81 7d 08 f4 01 00 00 cmpl $0x1f4,0x8(%ebp)
make_instr_impl_2op(cmp,rm,r,v)//3b
make_instr_impl_2op(cmp,i,a,b)//3c
make_instr_impl_2op(cmp,i,rm,b)//80
make_instr_impl_2op(cmp,rm,r,b)//3a
make_instr_impl_2op(cmp,i,a,v)//3d
