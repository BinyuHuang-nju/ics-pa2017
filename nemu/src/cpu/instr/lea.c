#include "cpu/instr.h"

make_instr_func(lea_m2r_v)
{
	int len=1;
	OPERAND r,m;
	m.type=OPR_MEM;
	r.type=OPR_REG;

	m.sreg=SREG_DS;

	m.data_size=data_size;
	r.data_size=data_size;

	len+=modrm_r_rm(eip+1,&r,&m);

//	operand_read(&m);
	r.val=m.addr;
	operand_write(&r);

	return len;
}
//8d 50 01:lea 0x1(%eax),%edx
