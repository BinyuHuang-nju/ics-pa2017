#include "cpu/cpu.h"

void set_CF_add(uint32_t result,uint32_t src)
{
	cpu.eflags.CF=result<src;
}

void set_PF(uint32_t result)
{
	uint32_t flag=0,x=result;
	for(uint32_t i=0;i<8;i++)
	{
		if(x%2==1)
			flag++;
		x=x>>1;
	}
	if(flag%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;
}

void set_ZF(uint32_t result)
{
	cpu.eflags.ZF=(result==0);
}

void set_SF(uint32_t result)
{
	cpu.eflags.SF=sign(result);
}

void set_OF_add(uint32_t result,uint32_t src,uint32_t dest)
{
	if(sign(src)==sign(dest))
	{
		if(sign(src)!=sign(result))
			cpu.eflags.OF=1;
		else
			cpu.eflags.OF=0;
	}
	else
		cpu.eflags.OF=0;
}

uint32_t alu_add(uint32_t src, uint32_t dest) {
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	assert(0);
//	return 0;
	uint32_t res=0;
	res=dest+src;

	set_CF_add(res,src);
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	set_OF_add(res,src,dest);

	return res;
}

uint32_t alu_adc(uint32_t src, uint32_t dest)
{
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	assert(0);
//	return 0;
	uint32_t res=0;
	res=dest+src+cpu.eflags.CF;

	if(src+cpu.eflags.CF<src||dest+cpu.eflags.CF<dest||dest+src<src)
		cpu.eflags.CF=1;
	else
	{
		cpu.eflags.CF=res<src;
	}
	
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	
	if(sign(dest)==sign(src))
	{
		if(sign(dest+src)!=sign(dest))
			cpu.eflags.OF=1;
		else
		{
			if(sign(cpu.eflags.CF)!=sign(dest+src))
				cpu.eflags.OF=0;
			else
			{
				if(sign(res)!=sign(dest+src))
					cpu.eflags.OF=1;
				else
					cpu.eflags.OF=0;
			}
		}
	}
	else
	{
		if(sign(dest+src)==sign(cpu.eflags.CF))
		{
			if(sign(res)!=sign(cpu.eflags.CF))
				cpu.eflags.OF=1;
			else
				cpu.eflags.OF=0;
		}
		else
			cpu.eflags.OF=0;
	}

	return res;
}


uint32_t alu_sub(uint32_t src, uint32_t dest)
{
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	assert(0);
//	return 0;
	uint32_t res=0;
	res=dest-src;
	
	cpu.eflags.CF=dest<src;

	set_PF(res);
	set_ZF(res);
	set_SF(res);

	if(sign(dest)!=sign(src))
	{
		if(sign(res)==sign(src))
			cpu.eflags.OF=1;
		else
			cpu.eflags.OF=0;
	}
	else
		cpu.eflags.OF=0;

	return res;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest)
{
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	assert(0);
//	return 0;
	uint32_t res=0;
	res=dest-src-cpu.eflags.CF;

	if(dest<src)
		cpu.eflags.CF=1;
	else
	{
		if(dest-src<cpu.eflags.CF)
			cpu.eflags.CF=1;
		else
			cpu.eflags.CF=0;
	}	

	set_PF(res);
	set_ZF(res);
	set_SF(res);

	if(sign(dest)!=sign(src+cpu.eflags.CF))
	{
		if(sign(res)==sign(src+cpu.eflags.CF))
			cpu.eflags.OF=1;
		else
			cpu.eflags.OF=0;

	}
	else
		cpu.eflags.OF=0;

	return res;

}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
	uint64_t dest_64,src_64,res,sign;
	dest_64=dest;
	src_64=src;
	res=dest_64*src_64;
	sign=((res>>data_size)<<data_size)<<(64-2*data_size);
	if(sign!=0)
		cpu.eflags.CF=cpu.eflags.OF=1;
	else
		cpu.eflags.CF=cpu.eflags.OF=0;
	return res;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
	int64_t dest_64,src_64,res;
	dest_64=dest;
	src_64=src;
	res=dest_64*src_64;
	return res;
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
	uint32_t res=dest/src;

	return res;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
	int32_t res=dest/src;

	return res;
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
	uint32_t res;
	res=dest%src;
	return res;
}

int32_t alu_imod(int64_t src, int64_t dest) 
{
	int32_t res;
	res=dest%src;
	return res;
}

uint32_t alu_and(uint32_t src, uint32_t dest)
{
	uint32_t res=0;
	res=src&dest;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	cpu.eflags.OF=0;

	return res;
}

uint32_t alu_xor(uint32_t src, uint32_t dest)
{
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	uint32_t res=0;
	res=src^dest;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	cpu.eflags.OF=0;
	return res;
}

uint32_t alu_or(uint32_t src, uint32_t dest) 
{
	uint32_t res=0;
	res=src|dest;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res);
	set_SF(res);
	cpu.eflags.OF=0;

	return res;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res=dest<<src;
	if(data_size!=32)
		cpu.eflags.CF=(res>>data_size)%2;
	else
	{
		if(src>32)
			cpu.eflags.CF=0;
		else
			cpu.eflags.CF=(dest>>(32-src))%2;
	}

	set_PF(res);

	cpu.eflags.ZF=1;
	uint32_t res0=res;
	for(uint32_t i=0;i<data_size;i++)
	{
		if(res0%2==1)
		{
			cpu.eflags.ZF=0;
			break;
		}
		res0=res0>>1;
	}

	cpu.eflags.SF=(res>>(data_size-1))%2;

//	cpu.eflags.OF=0;
	if(data_size==32)
		return res;
	uint32_t pow=1;
	for(uint32_t i=0;i<data_size;i++)//floating point exception:intersting
	//	!When the data_size
	//is equal to 32,then(unsigned)2^32 is equal to 0!
	{
		pow=pow*2;
	}
	uint32_t reshigh=dest-dest%pow,reslow=(dest%pow)<<src;
	switch(data_size)
	{
		case 8:reslow=reslow&0xFF;break;
		case 16:reslow=reslow&0xFFFF;break;
		default:break;
	}
	return reshigh+reslow; //pass
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
	if(src==0)
		cpu.eflags.CF=0;
	else
	{
		if(data_size>src)
			cpu.eflags.CF=(dest>>(src-1))%2;
		else
			cpu.eflags.CF=0;
	}

	uint32_t res=0,reshigh=0,reslow=0;
	if(data_size==32)
		reslow=res=dest>>src;
	else
	{
		uint32_t pow=1;
		for(uint32_t i=0;i<data_size;i++)
			pow=pow*2;
	        reshigh=dest-dest%pow,reslow=(dest%pow)>>src;
		res=reshigh+reslow;
	}

	set_PF(reslow);

	set_ZF(reslow);

	cpu.eflags.SF=(reslow>>(data_size-1))%2;
//	cpu.eflags.SF=0;

	return res;
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res=0,reslow=0;
	uint32_t temp=1;
	for(uint32_t i=0;i<data_size;i++)
	{
		uint32_t x=dest&temp;
		reslow=reslow+x;
		temp=temp*2;
	}
	temp=temp/2;
	int flag=1;
	for(int i=0;i<src-1;i++)
		flag*=2;

	if((dest&flag)==0)
		cpu.eflags.CF=0;
	else
		cpu.eflags.CF=1;

	uint32_t reshigh=dest;
	reshigh>>=data_size;
	reshigh<<=data_size;
	
	int sign_data_size=reslow&temp;
	reslow=reslow>>src;
	for(int i=0;i<src;i++)
	{
		reslow=reslow+sign_data_size;
		sign_data_size/=2;
	}
	res=reshigh+reslow;

	set_PF(reslow);

	set_ZF(reslow);

	cpu.eflags.SF=((reslow&temp)>>(data_size-1));

	if(data_size==32)
	{
		int dest_int=dest;
		set_PF(dest_int>>src);
		cpu.eflags.SF=sign(dest_int>>src);

		return dest_int>>src;
	}
	return res;

/*	if(src==0)
		cpu.eflags.CF=0;
	else
	{
		if(data_size>src)
			cpu.eflags.CF=(dest>>(src-1))%2;
		else
			cpu.eflags.CF=0;
	}

	uint32_t res=0,reshigh=0,reslow=0;
	if(data_size==32)
	{
		uint32_t flag_data_size=(reslow>>31)%2;
		uint32_t beginbit=1;
		if(src<32)
			beginbit=data_size-src+1;
		uint32_t ready=0,power=flag_data_size;
		for(uint32_t i=beginbit;i<=data_size;i++)
		{
			ready+=power;
			power=power*2;
		}
		res=reslow=(res>>src)|ready;
	}
	else
	{
		uint32_t pow=1;
		for(uint32_t i=0;i<data_size;i++)
			pow=pow*2;
		reshigh=dest-dest%pow;
		reslow=dest%pow;
		uint32_t flag_data_size=(reslow>>(data_size-1))%2;
		uint32_t beginbit=1;
		if(data_size>src)
		{
			beginbit=data_size-src+1;
		}
		uint32_t ready=0,power=flag_data_size;
		for(uint32_t i=beginbit;i<=data_size;i++)
		{
			ready+=power;
			power*=2;
		}
		reslow=(reslow>>src)|ready;
		res=reshigh+reslow;
	}

	set_PF(reslow);

	set_ZF(reslow);

	cpu.eflags.SF=(reslow>>(data_size-1))%2;

	return res;
*/
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)//same with shl
{
	uint32_t res=dest<<src;
	if(data_size!=32)
		cpu.eflags.CF=(res>>data_size)%2;
	else
	{
		if(src>32)
			cpu.eflags.CF=0;
		else
			cpu.eflags.CF=(dest>>(32-src))%2;
	}

	set_PF(res);

	cpu.eflags.ZF=1;
	uint32_t res0=res;
	for(uint32_t i=0;i<data_size;i++)
	{
		if(res0%2==1)
		{
			cpu.eflags.ZF=0;
			break;
		}
		res0=res0>>1;
	}

	cpu.eflags.SF=(res>>(data_size-1))%2;

	if(data_size==32)
		return res;
	uint32_t pow=1;
	for(uint32_t i=0;i<data_size;i++)
		pow=pow*2;
	uint32_t reshigh=dest-dest%pow,reslow=(dest%pow)<<src;
	switch(data_size)
	{
		case 8:reslow=reslow&0xFF;break;
		case 16:reslow=reslow&0xFFFF;break;
		default:break;
	}
	return reshigh+reslow;
}
