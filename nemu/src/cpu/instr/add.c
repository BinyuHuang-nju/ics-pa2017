#include "cpu/instr.h"
#include "cpu/alu.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val=alu_add(sign_ext(opr_src.val,opr_src.data_size),opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(add,i,rm,v)//81 c2 f3 1f 00 00:add $0x1ff3,%edx
make_instr_impl_2op(add,rm,r,b)//02 7e 06:add 0x6(%esi),%bl
make_instr_impl_2op(add,i,a,v)//05 f0 1f 00 00:add $0x1ff0,%eax
make_instr_impl_2op(add,r,rm,v)//01 d0: add $edx,%eax
make_instr_impl_2op(add,i,rm,bv)//83 c4 08: add $0x8,%esp
