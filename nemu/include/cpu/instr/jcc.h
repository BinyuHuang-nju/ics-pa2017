#ifndef __INSTR_JCC_H__
#define __INSTR_JCC_H__

make_instr_func(je_short_);//74 06
make_instr_func(jg_short_);//7f 06
make_instr_func(jle_short_);//7e
make_instr_func(jne_short_);//75
make_instr_func(jbe_short_);//3009e:76 a7 jbe 30047
make_instr_func(jge_short_);//7d
make_instr_func(jl_short_);//7c
make_instr_func(jae_short_);//73
make_instr_func(ja_short_);//77
make_instr_func(js_short_);//78
make_instr_func(jb_short_);//72
make_instr_func(jns_short_);//79

make_instr_func(jbe_near);//opcode2byte 0f 86
make_instr_func(jl_near);//opcode2byte 0f 8c
make_instr_func(jle_near);//2byte 0f 8e
make_instr_func(je_near);//0f 84
make_instr_func(ja_near);//0f 87
make_instr_func(js_near);//0f 88
make_instr_func(jne_near);//0f 85
make_instr_func(jg_near);//0f 8f
make_instr_func(jae_near);//0f 83
make_instr_func(jns_near);//0f 89
make_instr_func(jb_near);//0f 82

#endif
