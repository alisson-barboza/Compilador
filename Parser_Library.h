#include "Scanner_Library.h"

void parser_programa(FILE * arq);

void parser_bloco(FILE * arq, Ttolken * tolken);

int parser_decl_var(FILE * arq, Ttolken * tolken);  

int parser_comando(FILE * arq, Ttolken *tolken); 

void parser_cmd_basico(FILE * arq, Ttolken * tolken);

void parser_atribuicao(FILE * arq, Ttolken *tolken);

void parser_exp_arit(FILE *arq, Ttolken * tolken);

void parser_termo(FILE * arq, Ttolken * tolken);

void parser_fator(FILE * arq,Ttolken * tolken);

void parser_exp_rela(FILE * arq, Ttolken * tolken);

void parser_cmd_iteracao(FILE * arq, Ttolken * tolken);

void parser_cmd_if(FILE * arq, Ttolken * tolken);