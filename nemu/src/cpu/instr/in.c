#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in_dx2al) {
	uint16_t value=cpu.gpr[2]._16;
	cpu.gpr[0]._8[0]=pio_read(value,1);
	return 1;
}

make_instr_func(in_dx2eax_v) {
    uint16_t value=cpu.gpr[2]._16;
	if(data_size==16)
		cpu.gpr[0]._16=pio_read(value,2);
	else
		cpu.gpr[0]._32=pio_read(value,4);
	//printf("%x\n",cpu.eip+1);
	return 1;
}
