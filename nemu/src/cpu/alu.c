#include "cpu/cpu.h"
void all_SF(uint32_t res,size_t data_size){
	res = sign_ext(res & (0xFFFFFFFF >>( 32 -data_size)),data_size);
	cpu.eflags.SF = sign(res);
}
void all_ZF(uint32_t res,size_t data_size){
	res = res & (0xFFFFFFFF >> (32 -data_size));
	cpu.eflags.ZF =( res == 0);
}

void ad_OF(uint32_t res,uint32_t dest,uint32_t src,size_t data_size){
	switch(data_size)
	{
		case 8:
			res = sign_ext(res,data_size);
			dest = sign_ext(dest,data_size);
			src = sign_ext(src,data_size);
			break;
		case 16:
			res = sign_ext(res,data_size);
			dest = sign_ext(dest,data_size);
			src = sign_ext(src,data_size);
			break;
		case 32:
			res = sign_ext(res,data_size);
			dest = sign_ext(dest,data_size);
			src = sign_ext(src,data_size);
			break;
	}
	if(sign(src) == sign(dest)){
		if(sign(res) != sign(dest)){
	 		cpu.eflags.OF = 1;
		}
		else{
			cpu.eflags.OF = 0;	
		}
	}
	else{
		cpu.eflags.OF = 0;
	}
}

void add_CF(uint32_t res,uint32_t src,size_t data_size){
	res = res & (0xFFFFFFFF >> (32 - data_size));
	src = src & (0xFFFFFFFF >> (32 - data_size));
	res = sign_ext(res,data_size);
	src = sign_ext(src ,data_size);
	cpu.eflags.CF = res < src;
}
void all_PF(uint32_t res){
	int even = 0;
	switch(res & 0xf)
	{
	case 0x0:
	case 0x3:
	case 0x5:
	case 0x6:
	case 0x9:
	case 0xa:
	case 0xc:
	case 0xf:
 		even = 1;//偶数
		break;
	default:
		even = 0;//奇数
		break;
	}
	switch((res >> 4) &0xf)
	{
	case 0x0:
	case 0x3:
	case 0x5:
	case 0x6:
	case 0x9:
	case 0xa:
	case 0xc:
	case 0xf:
		if(even == 1){
			cpu.eflags.PF = 1;
		}
		else{
			cpu.eflags.PF = 0;
		}
		break;
	default:
		if(even == 1){
			cpu.eflags.PF = 0;
		}
		else{
			cpu.eflags.PF = 1;
		}
		break;
	}
}
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res = src +dest;
        all_SF(res,data_size);
	ad_OF(res,dest,src,data_size);
	all_ZF(res,data_size);	
	all_PF(res);
	add_CF(res,src,data_size);
//	rintf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	return res & ( 0xFFFFFFFF >> (32 - data_size));
#endif
}
void adc_CF(uint32_t res,uint32_t src,size_t data_size){
	res = res & (0xFFFFFFFF >> (32 - data_size));
	src = src & (0xFFFFFFFF >> (32 - data_size));
	res = sign_ext(res,data_size);
	src = sign_ext(src ,data_size);
	if(cpu.eflags.CF == 1){//存在进位
		cpu.eflags.CF = res <= src;
	}
	else{//不存在进位加入运算
		cpu.eflags.CF = res < src;
	}
}
uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
// 	fflush(stdout);
//	assert(0);
	uint32_t res = src + dest + cpu.eflags.CF;	
       	all_SF(res,data_size);
	ad_OF(res,dest,src,data_size);
	all_ZF(res,data_size);	
	all_PF(res);
	adc_CF(res,src,data_size);
//	
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

void sb_OF(uint32_t res,uint32_t dest,uint32_t src,size_t data_size){
	switch(data_size)
	{
		case 8:
			res = sign_ext(res,data_size);
			dest = sign_ext(dest,data_size);
			src = sign_ext(src,data_size);
			break;
		case 16:
			res = sign_ext(res,data_size);
			dest = sign_ext(dest,data_size);
			src = sign_ext(src,data_size);
			break;
		case 32:
			res = sign_ext(res,data_size);
			dest = sign_ext(dest,data_size);
			src = sign_ext(src,data_size);
			break;
	}
	if(sign(src) != sign(dest)){
		if(sign(res) != sign(dest)){
	 		cpu.eflags.OF = 1;
		}
		else{
			cpu.eflags.OF = 0;	
		}
	}
	else{
		cpu.eflags.OF = 0;
	}
}
void sub_CF(uint32_t dest,uint32_t src,size_t data_size){
	dest = sign_ext(dest,data_size);
	src = sign_ext(src,data_size);
	cpu.eflags.CF = dest < src;
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res = dest - src;
	all_SF(res,data_size);
	all_ZF(res,data_size);	
	all_PF(res);
	sub_CF(dest,src,data_size);
	sb_OF( res,dest,src,data_size);
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}
void sbb_CF(uint32_t dest,uint32_t src,size_t data_size){
	dest = sign_ext(dest,data_size);
	src = sign_ext(src,data_size);
	if(cpu.eflags.CF  == 1){
	cpu.eflags.CF = dest <= src;
	}
	else{
	cpu.eflags.CF = dest < src;
	}
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res = dest - src -cpu.eflags.CF;
	all_SF(res,data_size);
	all_ZF(res,data_size);	
	all_PF(res);
	sbb_CF(dest,src,data_size);
	sb_OF( res,dest,src,data_size);

//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	return res & (0xFFFFFFFF >> (32 -data_size));
#endif
}

void mul_eflags(uint64_t res,size_t data_size){
	res = res >> data_size;
	cpu.eflags.OF = res != 0;
	cpu.eflags.CF = res != 0;

}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	uint64_t tsrc =(uint64_t)src;
	uint64_t res = tsrc * dest;
//	switch(data_size)
//	{
//	case 8:
//		res =(uint16_t)res;
//		break;
//	case 16:
//		res = (uint32_t)res;
//		break;
//	default:
//		res = (uint64_t)res;
//		break;
//	}
	mul_eflags(res,data_size);
	return res;
#endif
}
void imul_eflags(int64_t res,size_t data_size){
	uint64_t mask;
	mask = 1;
	mask << data_size;
	mask<< (data_size-1);
	mask = ~mask;
	uint64_t temp = (uint64_t)res;
	temp = temp & mask;
	temp = temp >> data_size;
	cpu.eflags.CF = temp!=0;
	cpu.eflags.OF = temp!=0;
}
int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	int64_t res =((int64_t)src) * dest;
	imul_eflags(res,data_size);
	return res;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	if(src == 0){
		printf("遇到0了");
		return 0;
	}
	uint32_t res = dest /src;
	return res;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	uint32_t res = dest % src;
	return res;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	uint32_t res = dest & src;
	all_PF(res);
	all_ZF(res,data_size);
	all_SF(res,data_size);
	return res & (0xFFFFFFFF >>(32 - data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	uint32_t res = dest ^ src;
	all_PF(res);
	all_ZF(res,data_size);
	all_SF(res,data_size);
	return res & (0xFFFFFFFF >>(32 - data_size));

	
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res = dest | src;
	all_PF(res);
	all_ZF(res,data_size);
	all_SF(res,data_size);
	return res & (0xFFFFFFFF >>(32 - data_size));

//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	
#endif
}

void shl_OF(uint32_t res,size_t data_size){
	res = res & (0xFFFFFFFF << data_size);
	cpu.eflags.OF = res != 0;
}
//找到位移前的一位，判断是0还是1
void shl_CF(uint32_t dest,uint32_t src,size_t data_size){
	if(src == 0){
		cpu.eflags.CF = 0;
	}
	if(src >= data_size) {
		cpu.eflags.CF = 0;
	}
	dest = dest <<( src -1);
	dest = sign_ext(dest,data_size);
	cpu.eflags.CF = sign(dest);	
}
uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	uint32_t res =  dest << src;
	all_ZF(res,data_size);
	all_SF(res,data_size);
	all_PF(res);
	shl_OF(res,data_size);
	shl_CF(dest,src,data_size);

	return res & (0xFFFFFFFF >> (32 -data_size));
#endif
}
void shr_CF(uint32_t dest,uint32_t src,size_t data_size){
	src = src -1;
	dest = dest>>src;
	dest = dest & 0x1;
	cpu.eflags.CF = dest;
}
void shr_OF(uint32_t dest,uint32_t src ,size_t data_size){
	if(src > 1)return;
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	dest = sign_ext(dest,data_size);
	cpu.eflags.OF = sign(dest);

}
uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	uint32_t res = dest >> src;
	all_ZF(res,data_size);
	all_SF(res,data_size);
	all_PF(res);
	shr_OF(dest,src,data_size);
	shr_CF(dest,src,data_size);
	return res;
#endif
}
void sar_CF(int dest,uint32_t src,size_t data_size){
	if(src < 1) return;
	dest = dest >> (src - 1);
	cpu.eflags.CF = dest & 0x1;
}
void sar_OF(){
	cpu.eflags.OF = 0;
}
uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	dest = sign_ext(dest,data_size);
//	int sg = sign(dest,data_size);
	int32_t res = dest;
	res =  res >> src;
	all_ZF(res,data_size);
	all_SF(res,data_size);
	all_PF(res);
	sar_CF(dest,src,data_size);	
	sar_OF();
	res = res & (0xFFFFFFFF >> (32 - data_size));
	return res;
#endif
}

void sal_OCF(uint32_t dest,uint32_t src,size_t data_size){
	dest =	dest <<( src - 1);
	if(src == 0 || src >data_size){
		cpu.eflags.CF = 0;
	}
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	dest = sign_ext(dest,data_size);
	cpu.eflags.CF = sign(dest);//最高数值位移进CF
	if(src <= 1){
		cpu.eflags.OF = sign(dest);
	}	
}
uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
//	fflush(stdout);
//	assert(0);
	uint32_t res = dest << src;
	res = res & (0xFFFFFFFF >>(32 - data_size));	
	all_ZF(res,data_size);
	all_SF(res,data_size);
	all_PF(res);
	sal_OCF(dest,src,data_size);
//	shl_OF(res,data_size);
//	shl_CF(dest,src,data_size);

	return res;
#endif
}
