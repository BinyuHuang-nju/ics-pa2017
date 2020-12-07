#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(int_imm)
{
	OPERAND imm;
//	assert(data_size==8);
	imm.data_size = 8;
	imm.type = OPR_IMM;
	imm.sreg = SREG_CS;
	imm.addr = eip+1;
	operand_read(&imm);
	raise_sw_intr(imm.val);
	return 0;// in raise_sw_intr,"cpu.eip+=2" has been done.
}
//cd 80 : int $0x80
