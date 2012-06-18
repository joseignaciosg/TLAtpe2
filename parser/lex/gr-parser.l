%{
#include "parser/c/gr-parser.h"
char previous;
int h, k;
extern int valid_name, valid_grammar, valid_nt, valid_t, valid_d, valid_p;
extern char distinguished;
%}
%x TO_VN 
%x VN 
%x TO_VT 
%x VT 
%x TO_D 
%x DISTINGUISHED
%x TO_PR 
%x PRODUCTION 
%x PROD_TRNSTN 
%x TO_FIN 
%x FIN 
%x ERROR
space [ \t\n(\r\n)]
non_terminal [A-Z]{1}
terminal [a-z]{1}
number [0-9]
grammar_name [a-zA-Z][a-zA-Z0-9]*
lamda \\
valid_left_side {non_terminal}
valid_right_side {lamda}|([{terminal}{non_terminal}]+)
%%
<INITIAL>{space}*{grammar_name}{space}*={space}*\(	{printf("grammar recognized!\n");
							for(h=0; h<yyleng && isspace(yytext[h]); h++);
							 for(k=yyleng; k>0 && isspace(yytext[k]); k--);
								BEGIN(TO_VN);}
<TO_VN>{space}*\{				validate_name();BEGIN(VN);
<VN>{space}*{non_terminal}{space}*[,\}]			{
								printf("non-terminal recognized!\n");
								add_non_terminal(yytext);
								if(yytext[yyleng-1]==',')
									BEGIN(VN);
								else if(yytext[yyleng-1]=='}')
									BEGIN(TO_VT);
								else
									BEGIN(ERROR);}
<TO_VT>{space}*,{space}*\{			validate_non_terminals();BEGIN(VT);
<VT>{space}*{terminal}{space}*[,\}]			{
								printf("terminal recognized!\n");
								add_terminal(yytext);
								if(yytext[yyleng-1]==',')
									BEGIN(VT);
								else if(yytext[yyleng-1]=='}')
									BEGIN(TO_D);
								else
									BEGIN(ERROR);}
<TO_D>{space}*,					validate_terminals(); BEGIN(DISTINGUISHED);
<DISTINGUISHED>{space}*{non_terminal}		{
								printf("distinguished element recognized!\n");
								for(h=0; h<yyleng && isspace(yytext[h]); h++);
								set_distinguished(yytext[h]);
								yytext[0]=yytext[h];
								yytext[1]='\0';
								BEGIN(TO_PR);}
<TO_PR>{space}*,{space}*\{			{printf("to production\n");validate_distinguished();BEGIN(PRODUCTION);}
<PRODUCTION>{space}*({valid_left_side}){space}*		printf("left side\n");
<PRODUCTION>\-{space}*\>{space}*			printf("arrow\n");
<PRODUCTION>({valid_right_side}){space}*[,\|\}]	{
													printf("production  recognized!\n");
													add_production(yytext);
													if(yytext[yyleng-1]==',')
														BEGIN(PRODUCTION);
													else if(yytext[yyleng-1]=='}')
														BEGIN(TO_FIN);
													else if(yytext[yyleng-1]=='|')
													{
														for(h=0; h<yyleng && isspace(yytext[h]); h++);
														previous=yytext[h];
														BEGIN(PROD_TRNSTN);
													}
													else
														BEGIN(ERROR);}
<PROD_TRNSTN>{space}*({valid_right_side}){space}*[,\|\}]	{
								printf("production recognized!\n");
								add_production2(previous, yytext);
								if(yytext[yyleng-1]==',')
									BEGIN(PRODUCTION);
								else if(yytext[yyleng-1]=='}')
									BEGIN(TO_FIN);
								else if(yytext[yyleng-1]=='|')
								{
									BEGIN(PROD_TRNSTN);
								}
								else
									BEGIN(ERROR);
								}
<TO_FIN>{space}*\)	validate_productions();validate_grammar();BEGIN(FIN);
<FIN>[.{space}]*	;
<ERROR>.{space}*	;
%%

int main()
{
	init();
	yylex();
	process();
	return 0;
}

