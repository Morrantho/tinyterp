#include<stdio.h>
#include<stdlib.h>
static unsigned short ip=0;
static char regs[16]={0};
static char hex_map[255]=
{
	['0']=1,
	['1']=2,
	['2']=3,
	['3']=4,
	['4']=5,
	['5']=6,
	['6']=7,
	['7']=8,
	['8']=9,
	['9']=10,
	['A']=11,
	['B']=12,
	['C']=13,
	['D']=14,
	['E']=15,
	['F']=16,
	0
};

#define LBL_MOV\
	mov:\
		regs[(hex_map[args[ip+1][0]])-1]=reg_val(args[ip+2]);\
		ip+=2;\
		goto end;

#define LBL_ADD\
	add:\
		regs[(hex_map[args[ip+1][0]])-1]+=reg_val(args[ip+2]);\
		ip+=2;\
		goto end;

#define LBL_SUB\
	sub:\
		regs[(hex_map[args[ip+1][0]])-1]-=reg_val(args[ip+2]);\
		ip+=2;\
		goto end;

#define LBL_MUL\
	mul:\
		regs[(hex_map[args[ip+1][0]])-1]*=reg_val(args[ip+2]);\
		ip+=2;\
		goto end;

#define LBL_DIV\
	_div:\
		regs[(hex_map[args[ip+1][0]])-1]/=reg_val(args[ip+2]);\
		ip+=2;\
		goto end;

#define LBL_MOD\
	mod:\
		regs[(hex_map[args[ip+1][0]])-1]%=reg_val(args[ip+2]);\
		ip+=2;\
		goto end;

#define LBL_END\
	end:\
		ip++;

#define LBLS\
	X(mov,LBL_MOV,'=')\
	X(add,LBL_ADD,'+')\
	X(sub,LBL_SUB,'-')\
	X(mul,LBL_MUL,'*')\
	X(_div,LBL_DIV,'/')\
	X(mod,LBL_MOD,'*')\
	X(end,LBL_END,'.')

char reg_val(char* s)
{
	if(s[0]=='@') return regs[(hex_map[s[1]])-1];
	return atoi(s);
}

void show_regs()
{
	char i;
	for(i=0;i<16;i++)
	{
		printf("i:%d %d\n",i,regs[i]);
	}
}

/* Tested Source: */

/*
= A 50
= B 25
+ C @A
+ C @B
+ C 25
*/

int main(int nargs,char** args)
{
	ip++;
	#define X(LBL,LBL_SRC,LBL_IDX) [LBL_IDX]=&&LBL,
	static void* lbls[255]={LBLS};
	while(ip<nargs)
	{
		goto *lbls[args[ip][0]];
		#undef X
		#define X(LBL,LBL_SRC,LBL_IDX) LBL_SRC
		LBLS
	}
	show_regs();
	return 0;
}