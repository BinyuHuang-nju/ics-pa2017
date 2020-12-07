#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"


void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
// Trigger an exception/interrupt with 'intr_no'
// 'intr_no' is the index to the IDT
// Push EFLAGS, CS, and EIP
// Find the IDT entry using 'intr_no'
// Clear IF if it is an interrupt
// Set EIP to the entry of the interrupt handler
/*	OPERAND ESP;
	ESP.data_size = data_size;
	ESP.type = OPR_REG;
	ESP.addr = REG_ESP;
	operand_read(&ESP);
*/
	OPERAND temp;
	temp.type = OPR_MEM;
	temp.sreg = SREG_SS;
	temp.data_size = 32;
	cpu.esp = cpu.esp-4;
	temp.addr = cpu.esp;
	temp.val = cpu.eflags.val;
	operand_write(&temp); // push eflags

	cpu.esp = cpu.esp-4;
	temp.addr = cpu.esp;
	temp.val = cpu.cs.base;
	operand_write(&temp);//push CS.base

	cpu.esp = cpu.esp-4;
	temp.addr = cpu.esp;
//	temp.data_size = 16;
	temp.val = (uint32_t)cpu.cs.val;
	operand_write(&temp);//push CS.val
	
	cpu.esp = cpu.esp-4;
	temp.addr = cpu.esp;
	temp.val = cpu.eip;
	operand_write(&temp);//push eip
	
	GateDesc gatedesc;
	uint32_t pos = cpu.idtr.base+intr_no*8;
	gatedesc.val[0]=vaddr_read(pos,SREG_CS,4);
	gatedesc.val[1]=vaddr_read(pos+4,SREG_CS,4);

	cpu.eip = (gatedesc.offset_31_16<<16)+gatedesc.offset_15_0
		+cpu.segReg[gatedesc.selector].base;
	if(gatedesc.type == 0xe)
		cpu.eflags.IF = 0;
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
