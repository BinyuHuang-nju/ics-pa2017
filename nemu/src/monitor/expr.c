#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#include "cpu/cpu.h"
#include "elf.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

void load_elf_tables(char * exec_file);
uint32_t look_up_symtab(char *sym, bool *success);

enum {
	NOTYPE = 256,                                          
	HEXNUM, DECNUM,                                  //num 257-258
	ARG,                                             //argument 259
	REG_1, REG_2, REG_3, REG_4,		                 //regs 260-263
	OP_NOT, OP_ADD, OP_SUB, OP_DIV, OP_MUL, OP_MOD,        
	OP_LEFTBRC, OP_RIGHTBRC, OP_AND, OP_OR,          //operator 264-273
	JD_EQ, JD_GE, JD_LE, JD_G, JD_L, JD_NEQ,         //judge 274-279
	SP_DEREF, SP_NEG                                 //special'-''*' 280 281

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"0[xX][0-9a-fA-F]+", HEXNUM},	//num
	{"[0-9]+", DECNUM},
	{"[0-9a-z_A-Z]+", ARG},         //arguement 
	{"\\$e[a-d]+x", REG_1},         //regs
	{"\\$e[sd]+i", REG_2},
	{"\\$e[sb]+p", REG_3},
	{"\\$eip", REG_4},
	{"!", OP_NOT},                  //operator sign
	{"\\+", OP_ADD},
	{"\\-", OP_SUB},
	{"/", OP_DIV},
	{"\\*", OP_MUL},
	{"%", OP_MOD},
	{"\\(", OP_LEFTBRC},
	{"\\)", OP_RIGHTBRC},
	{"&&", OP_AND},
	{"\\|\\|", OP_OR},
	{"==", JD_EQ},                  //judge sign
	{">=", JD_GE},
	{"<=", JD_LE},
	{">", JD_G},
	{"<", JD_L}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	int ndx;
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		bool ifBlank=false;
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

//				printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
//				printf("  %d\n",rules[i].token_type);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				if(substr_len>=32)
				{
					printf("expression too long!\n");
					return false;
				}
				
				Token new_token;
				switch(rules[i].token_type) {
					case NOTYPE: ifBlank=true;
						     break;
					case OP_LEFTBRC:
					case OP_RIGHTBRC:
					case OP_ADD: 
					case OP_SUB:
					case OP_AND:
					case OP_OR:
					case OP_MOD:
					case OP_DIV:
					case OP_MUL: new_token.type=rules[i].token_type;
						     break;
					case JD_EQ:
					case JD_GE:
					case JD_G:
					case JD_LE:
					case JD_L:  new_token.type=rules[i].token_type;
		       				    break;	
					case OP_NOT:	
						if(e[position]=='=')
				    		{
							new_token.type=JD_NEQ;
							position++;
						}		
						else
						       	new_token.type=rules[i].token_type;
						break;
					case DECNUM:
						new_token.type=rules[i].token_type;
						for(ndx=0;ndx<substr_len;ndx++)
							new_token.str[ndx]=substr_start[ndx];
						new_token.str[substr_len]='\0';
						break;
					case HEXNUM:
						new_token.type=rules[i].token_type;
						for(ndx=0;ndx<substr_len;ndx++)
							new_token.str[ndx]=substr_start[ndx];
						new_token.str[substr_len]='\0';
						break;
					case REG_1:  //eax ecx edx ebx
					case REG_2:  //esi edi
					case REG_3:  //esp ebp
					case REG_4:  //eip
						new_token.type=rules[i].token_type;
						for(ndx=0;ndx<substr_len;ndx++)
							new_token.str[ndx]=substr_start[ndx];
						new_token.str[substr_len]='\0';
						break;
					case ARG:
						new_token.type=rules[i].token_type;
						for(ndx=0;ndx<substr_len;ndx++)
							new_token.str[ndx]=substr_start[ndx];
						new_token.str[substr_len]='\0';
						break;
					default: tokens[nr_token].type = rules[i].token_type;
							 nr_token ++;
				}
				if(ifBlank)
					break;
				tokens[nr_token]=new_token;
				nr_token++;			
				if(nr_token>=32)
				{
					printf("\nTokens too many!\n");
					return false;
				}
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}
bool isBad=false;
bool check_parentheses(int p,int q)
{
	if(q-p==1 && tokens[p].type==OP_LEFTBRC && tokens[q].type==OP_RIGHTBRC)
	{
		printf("\nError:Nothing in the bracket!\n");
		isBad=true;
		return false;
	}
	if(tokens[p].type!=OP_LEFTBRC || tokens[q].type!=OP_RIGHTBRC)
		return false;

	int numl=0,numr=0;
	for(int i=p;i<=q;i++)
	{
		if(numl>0 && numl==numr)
		{
			isBad=true;
			return false;
		}
		if(numl<numr)
		{
			isBad=true;
			return false;
		}
		if(tokens[i].type==OP_LEFTBRC)
			numl++;
		if(tokens[i].type==OP_RIGHTBRC)
			numr++;
	}
	if(numl!=numr)
	{
		isBad=true;
		return false;
	}
	return true;
}
int dominant_operator(int p,int q)
{
	int x;
	int brc_match=0;
	for(x=q;x>=p;x--)// || and && priority 10
	{
		if(tokens[x].type==OP_RIGHTBRC) brc_match++;
		if(tokens[x].type==OP_LEFTBRC) brc_match--;
		if(brc_match==0 && (tokens[x].type==OP_AND || tokens[x].type==OP_OR))
			return x;
	}
	if(x==p-1)
	{
		for(x=q;x>=p;x--) // == and != priority 9
		{
			if(tokens[x].type==OP_RIGHTBRC) brc_match++;
			if(tokens[x].type==OP_LEFTBRC) brc_match--;
			if(brc_match==0 && (tokens[x].type==JD_EQ || tokens[x].type==JD_NEQ))
				return x;
		}
	}
	if(x==p-1)
	{
		for(x=q;x>=p;x--) // > , >= , < and <= priority 8
		{
			if(tokens[x].type==OP_RIGHTBRC) brc_match++;
			if(tokens[x].type==OP_LEFTBRC) brc_match--; 
			if(brc_match==0 &&(tokens[x].type==JD_G||tokens[x].type==JD_GE||
					tokens[x].type==JD_L||tokens[x].type==JD_LE))
				return x;
		}
	}
	if(x==p-1)
	{
		for(x=q;x>=p;x--)// + and - priority 7
		{
			if(tokens[x].type==OP_RIGHTBRC) brc_match++;
			if(tokens[x].type==OP_LEFTBRC) brc_match--;
			if(brc_match==0 && (tokens[x].type==OP_ADD||tokens[x].type==OP_SUB))
				return x;
		}
	}
	if(x==p-1)
	{
		for(x=q;x>=p;x--) // * , / and % priority 6
		{
			if(tokens[x].type==OP_RIGHTBRC) brc_match++;
			if(tokens[x].type==OP_LEFTBRC) brc_match--;
			if(brc_match==0 && (tokens[x].type==OP_MUL||
				tokens[x].type==OP_DIV||tokens[x].type==OP_MOD))
				return x;
		}
	}
	if(x==p-1&&(tokens[p].type==SP_DEREF||tokens[p].type==SP_NEG
				||tokens[p].type==OP_NOT))
		return p;
	isBad=true;
	return 0;
}
int eval(int p,int q)
{
//	printf("%d  %d  ,",p,q);
//	printf("%s\n",(check_parentheses(p,q)==true)?"true":"false");
	if(isBad)
	       	return 0;
	if(p>q)
	{
		printf("\nError:Problems in function eval!\n");
		isBad=true;
		return 0;
	}
	else if(p==q || q==p+1)
	{
		if(tokens[q].type==ARG)
		{
			bool successful=true;
			uint32_t addr=look_up_symtab(tokens[q].str,&successful);
			if(!successful)
			{
				isBad=true;
				printf("Invalid variable:%s!\n",tokens[q].str);
				return 0;
			}
			if(p==q)
				return addr;
			else if(q==p+1 && tokens[p].type==OP_NOT)
				return !addr;
			else if(q==p+1 && tokens[p].type==SP_NEG)
				return -addr;
			else if(q==p+1 && tokens[p].type==SP_DEREF)
			{
				return vaddr_read(addr,SREG_DS,4);
			}
		}
		else if(tokens[q].type==DECNUM)
		{
			int len_num=strlen(tokens[q].str);
			int decnum=0,pow=1;
			for(int i=len_num-1;i>=0;i--)
			{
				decnum+=pow*(tokens[q].str[i]-'0');
				pow = pow*10;
			}
			if(q==p+1 && tokens[p].type==SP_DEREF)
			{
				int addr;
				sscanf(tokens[q].str,"%x",&addr);
				return vaddr_read(addr,SREG_DS,4);
			}
			else if(q==p+1 && tokens[p].type==SP_NEG)
				return -decnum;
			else if(q==p+1 && tokens[p].type==OP_NOT)
				return !decnum;
			else if(p==q)
				return decnum;
			else
			{
				isBad=true;
				printf("\nError:Invalid decimal number!\n");
				return 0;
			}
		}
		else if(tokens[q].type==HEXNUM)
		{
			int len_num=strlen(tokens[q].str);
			int hexnum=0,pow=1;
			for(int i=len_num-1;i>=2;i--)
			{
				if(tokens[q].str[i]>='0' && tokens[q].str[i]<='9')
					hexnum+=pow*(tokens[q].str[i]-'0');
				else if(tokens[q].str[i]>='a'&&tokens[q].str[i]<='f')
					hexnum+=pow*(tokens[q].str[i]-'a'+10);
				else if(tokens[q].str[i]>='A'&&tokens[q].str[i]<='F')
					hexnum+=pow*(tokens[q].str[i]-'A'+10);
				pow = pow*16;
			}
			if(q==p+1 && tokens[p].type==SP_DEREF)
			{
				int addr;
				sscanf(tokens[q].str,"%x",&addr);
				return vaddr_read(addr,SREG_DS,4);
			}
			else if(q==p+1 && tokens[p].type==SP_NEG)
				return -hexnum;
			else if(q==p)
				return hexnum;
			else 
			{
				isBad=true;
				printf("\nError:Invalid hexadecimal number!\n");
				return 0;
			}
		}
		else if(tokens[q].type==REG_1 ||tokens[q].type==REG_2 
				||tokens[q].type==REG_3||tokens[q].type==REG_4)
		{
			uint32_t ret=0;
			if(tokens[q].type==REG_1)
			{
				if(tokens[q].str[2]=='a')
					ret = cpu.eax;
				else if(tokens[q].str[2]=='b')
					ret = cpu.ebx;
				else if(tokens[q].str[2]=='c')
					ret = cpu.ecx;
				else if(tokens[q].str[2]=='d')
					ret = cpu.edx;
			}
			else if(tokens[q].type==REG_2)
			{
				if(tokens[q].str[2]=='s')
					ret = cpu.esi;
				if(tokens[q].str[2]=='d')
					ret = cpu.edi;
			}
			else if(tokens[q].type==REG_3)
			{
				if(tokens[q].str[2]=='s')
					ret = cpu.esp;
				else if(tokens[q].str[2]=='b')
					ret = cpu.ebp;
			}
			else if(tokens[q].type==REG_4)
				ret = cpu.eip;
			return ret;
		}
		else
		{
			isBad=true;
			return 0;
		}
	}
	else if(check_parentheses(p,q)==true)
		return eval(p+1,q-1);//throw away the parentheses.
	else
	{
		int op=dominant_operator(p,q);
		if(op>=p+1)
		{
			int val1=eval(p,op-1);
			int val2=eval(op+1,q);
			switch(tokens[op].type)
			{
			   case OP_ADD:return val1+val2; break;
		           case OP_SUB:return val1-val2; break;
		           case OP_MUL:return val1*val2; break;
			   case OP_DIV:
			       if(strlen(tokens[op+1].str)==1&&tokens[op+1].str[0]=='0')
			       {
				       printf("\nError:Divide by 0!\n");
				       isBad=true;
				       return 0;
			       }
			       else
			       	   return val1/val2;
			       break;
			   case OP_MOD:
			       if(strlen(tokens[op+1].str)==1&&tokens[op+1].str[0]=='0')
			       {
				       printf("\nError:Mod by 0!\n");
				       isBad=true;
				       return 0;
			       }
			       else
				       return val1%val2;
			       break;
			   case OP_AND:return val1 && val2; break;
			   case OP_OR: return val1 || val2; break;
			   case JD_EQ: return val1 == val2; break;
		   	   case JD_NEQ:return val1 != val2; break;
			   case JD_GE: return val1 >= val2; break;
		  	   case JD_G:  return val1 > val2;  break;
			   case JD_LE: return val1 <= val2; break;
			   case JD_L:  return val1 < val2;  break;
			   default:assert(0);
			}
		}
		else
		{
			if(tokens[op].type==SP_DEREF)
			{
				int addr=eval(op+1,q);
				sscanf(tokens[q].str,"%x",&addr);
				return vaddr_read(addr,SREG_DS,4);
			}
			else if(tokens[op].type==SP_NEG)
			{
				int ret= 0 - eval(op+1,q);
				return ret;
			}
			else if(tokens[op].type==OP_NOT)
			{
				int ret = !eval(op+1,q);
				return ret;
			}
		}
	}
	return 0;
}

uint32_t expr(char *e, bool *success) {
	//init
	nr_token=0;
	for(int i=0;i<32;i++)
	{
		tokens[i].type=NOTYPE;
		for(int j=0;j<32;j++)
			tokens[i].str[j]='\0';
	}
	isBad=false;
	//step 1
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	// step 1 plus ----judging * and -
	for(int i=0;i<nr_token;i++) 
	{
		if(tokens[i].type==OP_MUL&&(i==0||(tokens[i-1].type!=DECNUM && 
		tokens[i-1].type!=HEXNUM && tokens[i-1].type!=OP_RIGHTBRC)))
			tokens[i].type = SP_DEREF;
	}
	for(int i=0;i<nr_token;i++)
	{
		if(tokens[i].type==OP_SUB&&(i==0||(tokens[i-1].type!=DECNUM && 
		tokens[i-1].type!=HEXNUM && tokens[i-1].type!=OP_RIGHTBRC)))
			tokens[i].type = SP_NEG;
	}

//	for(int i=0;i<nr_token;i++)
//	{
//		printf("%d %s\n",tokens[i].type,tokens[i].str);
//	}
	//step 2
	int res = eval(0,nr_token-1);
	if(isBad)
	{
		*success=false;
		return 0;
	}
//	printf("\nPlease implement expr at expr.c\n");
//	assert(0);

	return res;
}
