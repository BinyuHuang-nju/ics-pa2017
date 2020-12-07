#include "cpu/instr.h"
make_instr_func(cli)
{
	cpu.eflags.IF = 0;
	return 1;
}
//fa cli

make_instr_func(sti)
{
	cpu.eflags.IF = 1;
	return 1;
}
//fb sti
