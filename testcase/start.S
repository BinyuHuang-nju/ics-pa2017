.globl start
start:
# Set up a stack for C code.

	#movl $0, %ebp
	#movl $(128 << 20), %esp
	#sub $16, %esp
	jmp main				# never return



#	BREAK_POINT              #make sense when going into kernel
#	movl $0,%ebp
#	movl $(128<<20),%esp
#	jmp init
