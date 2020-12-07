#ifndef __INSTR_RET_H__
#define __INSTR_RET_H__

make_instr_func(ret_near);//c3
make_instr_func(ret_near_i);//c2 00 04:ret $0x4
make_instr_func(iret);//cf

#endif
