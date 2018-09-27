#include "Parser_Library.h"

enum Erros_do_parser{ESPECIAL, COMANDO, OP_RELACIONAL};

void parser_erro(Ttolken * tolken, int linha, int coluna, int classificacao_do_erro){
    printf("\nErro na linha %d, coluna %d", linha, coluna);
    switch(classificacao_do_erro){
        case PARENTESIS_FECHADO:        
            printf("\nEsperado ')'");
            break;
        case PARENTESIS_ABERTO:
            printf("\nEsperado '('");
            break;
        case MAIN:
            printf("\nEsperado MAIN");
            break;
        case INT:
            printf("\nEsperado INT");
            break;
        case CHAVE_ABERTA:
            printf("\nEsperado '{'");
            break;
        case CHAVE_FECHADA:
            printf("\nEsperado '}'");
            break;
        case IDENTIFICADOR:
            printf("\nEsperado um Idenficador");
            break;
        case PONTO_E_VIRGULA:
            printf("\nEsperado ';'");
            break;
        case IGUAL:
            printf("\nEsperado um '='");
            break;
        case ESPECIAL://esse caso é quando eu não sei o que falta, id, float, int, char...
            printf("\nEsperado um '(', identificador ou palavra reservada relativa a declaracao");
            break;
        case WHILE:
            printf("\nEsperado um WHILE");
            break;
        case COMANDO:
            printf("\nEsperado um comando");
            break;
        case OP_RELACIONAL:
            printf("\nEsperado um operador relacional");
            break;
        default:
            break;    
    }
    printf("\nUltimo tolken lido: %s", tolken->lexema);
    exit(-1);
}

void parser_programa(FILE * arq){
    Ttolken tolken;

    tolken = scan(arq);
    if(tolken.classificacao == INT){
        tolken = scan(arq);
        if(tolken.classificacao == MAIN){
            tolken = scan(arq);
            if(tolken.classificacao == PARENTESIS_ABERTO){
                tolken = scan(arq);
                if(tolken.classificacao == PARENTESIS_FECHADO){     
                    //Lógica do lookahead, passo para o bloco decidir se está correto a abertura do bloco
                    tolken = scan(arq);               
                    
                    parser_bloco(arq, &tolken);                    
                }else{
                    parser_erro(&tolken, linha,coluna,PARENTESIS_FECHADO);
                    //ERRO ERA PRA SER LIDO UM ")"
                }
            }else{
                parser_erro(&tolken, linha,coluna, PARENTESIS_ABERTO);
                //ERRO ERA PRA SER LIDO UM "("
            }
        }else{
            parser_erro(&tolken, linha,coluna, MAIN);
            //ERRO ERA PRA SER LIDO UM "MAIN"
        }
    }else{
        parser_erro(&tolken, linha,coluna, INT);
        //ERRO ERA PRA SER LIDO UM "INT"
    }
}

void parser_bloco(FILE * arq, Ttolken * tolken){    
    printf("Bloco =>");
    if(tolken->classificacao == CHAVE_ABERTA){
        *tolken = scan(arq);
        if(tolken->classificacao == INT || tolken->classificacao == FLOAT || tolken->classificacao == CHAR){
            while(parser_decl_var(arq, tolken)){                           
            }
        }
        while(parser_comando(arq, tolken)){
        }
        
        if(tolken->classificacao == CHAVE_FECHADA){
            //De novo a lógica do lookahead pois pode ser que alguem tenha chamado essa função além do começo do programa
            //então movo a cabeça de leitura para facilitar
            printf("FIM");
            *tolken = scan(arq);
            
        }else{
            parser_erro(tolken, linha,coluna, CHAVE_FECHADA);
            //ERRO, FALTOU FECHAR O BLOCO
        }          
    }else{
        parser_erro(tolken, linha,coluna, CHAVE_ABERTA);
        //ERRO ERA PRA SER UMA ABERTURA DE BLOCO
    }

}

int parser_decl_var(FILE * arq, Ttolken * tolken){
    printf("decl_var");
    if(tolken->classificacao == INT){
        *tolken = scan(arq);
        if(tolken->classificacao == IDENTIFICADOR){
            *tolken = scan(arq);
            while(tolken->classificacao == VIRGULA){
                *tolken = scan(arq);
                if(tolken->classificacao == IDENTIFICADOR){
                    
                    *tolken = scan(arq);
                }else if(tolken->classificacao == PONTO_E_VIRGULA){
                    break;
                }else{
                    parser_erro(tolken, linha,coluna, IDENTIFICADOR);
                    //ERRO ESPERAVA UM IDENTIFICADOR                    
                }
            }
            //preciso da checagem abaixo pq ele pode ler somente um identificador
            if(tolken->classificacao == PONTO_E_VIRGULA){
                *tolken = scan(arq);
                return 1;
            }else{
                parser_erro(tolken, linha,coluna, PONTO_E_VIRGULA);
                //ERRO ESPERAVA UM ";"
            }
        }        
    }else if(tolken->classificacao == FLOAT){
        *tolken = scan(arq);
        if(tolken->classificacao == IDENTIFICADOR){
            *tolken = scan(arq);
            while(tolken->classificacao == VIRGULA){
                *tolken = scan(arq);
                if(tolken->classificacao == IDENTIFICADOR){
                    *tolken = scan(arq);
                }else if(tolken->classificacao == PONTO_E_VIRGULA){
                    break;
                }else{
                    parser_erro(tolken, linha,coluna, IDENTIFICADOR);
                    //ERRO ESPERAVA UM IDENTIFICADOR                    
                }
            }
        }
        //preciso da checagem abaixo pq ele pode ler somente um identificador
        if(tolken->classificacao == PONTO_E_VIRGULA){
            *tolken = scan(arq);
            return 1;
        }else{
            parser_erro(tolken, linha,coluna, PONTO_E_VIRGULA);
            //ERRO ESPERAVA UM ";"
        }        
    }else if(tolken->classificacao == CHAR){
        *tolken = scan(arq);
        if(tolken->classificacao == IDENTIFICADOR){
            *tolken = scan(arq);
            while(tolken->classificacao == VIRGULA){
                *tolken = scan(arq);
                if(tolken->classificacao == IDENTIFICADOR){
                    *tolken = scan(arq);
                }else if(tolken->classificacao == PONTO_E_VIRGULA){
                    break;
                }else{
                    parser_erro(tolken, linha,coluna, IDENTIFICADOR);
                    //ERRO ESPERAVA UM IDENTIFICADOR                    
                }
            }
        }
        //preciso da checagem abaixo pq ele pode ler somente um identificador
        if(tolken->classificacao == PONTO_E_VIRGULA){
            *tolken = scan(arq);
            return 1;
        }else{
            parser_erro(tolken, linha,coluna, PONTO_E_VIRGULA);
            //ERRO ESPERAVA UM ";"
        }
    }else{
        return 0;
    }
}

int parser_comando(FILE * arq, Ttolken *tolken){
    printf("comando =>");
    if(tolken->classificacao == IDENTIFICADOR ||tolken->classificacao == CHAVE_ABERTA){        
        parser_cmd_basico(arq, tolken);
    }
    else if(tolken->classificacao == DO ||tolken->classificacao == WHILE){
        parser_cmd_iteracao(arq, tolken);
    }else if(tolken->classificacao == IF){
        parser_cmd_if(arq, tolken);
    }else{
        return 0;
    }
    return 1;
}

void parser_cmd_basico(FILE * arq, Ttolken * tolken){
    printf("comando_basico =>");
    if(tolken->classificacao == IDENTIFICADOR){
        parser_atribuicao(arq, tolken);
    }
    else if(tolken->classificacao == CHAVE_ABERTA){
        parser_bloco(arq, tolken);
    }
}

void parser_atribuicao(FILE * arq, Ttolken *tolken){
    printf("atribuicao =>");
    if(tolken->classificacao == IDENTIFICADOR){
        *tolken = scan(arq);

        if(tolken->classificacao == ATRIBUICAO){
            *tolken = scan(arq);
            
            parser_exp_arit(arq, tolken);

            if(tolken->classificacao == PONTO_E_VIRGULA){
                *tolken = scan(arq);
            }
            else{
                parser_erro(tolken, linha,coluna, PONTO_E_VIRGULA);
                //ERRO ESPERAVA ";"
            }
        }
        else{
            parser_erro(tolken, linha,coluna, IGUAL);
            //ERRO ESPERAVA "="
        }
    }else{
        parser_erro(tolken, linha,coluna, IDENTIFICADOR);
        //ERRO ESPERAVA UM IDENTIFICADOR
    }

}

void parser_exp_arit(FILE *arq, Ttolken * tolken){
    printf("expressao aritmetica =>");
    if(tolken->classificacao == PARENTESIS_ABERTO || tolken->classificacao == IDENTIFICADOR || tolken->classificacao == TIPO_FLOAT ||
        tolken->classificacao == TIPO_INT || tolken->classificacao == TIPO_CHAR){         
    
            parser_termo(arq, tolken);

            if(tolken->classificacao == SOMA || tolken->classificacao == SUBTRACAO){
                *tolken = scan(arq);
                parser_exp_arit(arq, tolken);
            }

    }else{
        parser_erro(tolken, linha, coluna, ESPECIAL);
        //ERRO EU ESPERAVA ALGUMA COISA VINDA DO USUARIO DENTRE AS LISTADAS
    }    
}

void parser_termo(FILE * arq, Ttolken * tolken){
    printf("termo =>");
    if(tolken->classificacao == PARENTESIS_ABERTO || tolken->classificacao == IDENTIFICADOR || tolken->classificacao == TIPO_FLOAT ||
        tolken->classificacao == TIPO_INT || tolken->classificacao == TIPO_CHAR){
            

            parser_fator(arq, tolken);

            if(tolken->classificacao == DIVISAO || tolken->classificacao == MULTIPLICACAO){
                *tolken = scan(arq);
                parser_termo(arq, tolken);
            }
        }
}

void parser_fator(FILE * arq, Ttolken * tolken){
    printf("fator =>");
    if(tolken->classificacao == PARENTESIS_ABERTO){        
        *tolken = scan(arq);
        parser_exp_arit(arq, tolken);
        if(tolken->classificacao == PARENTESIS_FECHADO){
            *tolken = scan(arq);
        }else{
            parser_erro(tolken, linha,coluna, PARENTESIS_FECHADO);
            //ERRO ESPERAVA UM PARENTESIS FECHADO
        }
    }
    else if(tolken->classificacao == IDENTIFICADOR|| tolken->classificacao == TIPO_FLOAT ||
        tolken->classificacao == TIPO_INT || tolken->classificacao == TIPO_CHAR){
        printf(" %s ", tolken->lexema);
        *tolken = scan(arq);
        
    }else{
        parser_erro(tolken, linha,coluna, ESPECIAL);
        //ERRO ESPERAVA UM ID, INTEIRO, FLOAT OU CHAR, NÃO LI NADA DISSO
    }

}

void parser_cmd_iteracao(FILE * arq, Ttolken * tolken){
    printf("iteracao =>");
    if(tolken->classificacao == WHILE){
        *tolken = scan(arq);

        if(tolken->classificacao == PARENTESIS_ABERTO){
            *tolken = scan(arq);
            parser_exp_rela(arq, tolken);

            if(tolken->classificacao == PARENTESIS_FECHADO){
                *tolken = scan(arq);

                if(!parser_comando(arq, tolken)){/*teste pra saber se ele tem pelo menos um comando*/
                    parser_erro(tolken, linha,coluna, COMANDO);
                    //ERRO PRECISA DE UM COMANDO
                }
            }else{
                parser_erro(tolken, linha,coluna, PARENTESIS_FECHADO);
                //ERRO ESPERAVA PARENTESIS FECHADO
            }
        }else{
            parser_erro(tolken, linha,coluna, PARENTESIS_ABERTO);
            //ERRO ESPERAVA PARENTESIS ABERTO
        }
    }
    else if(tolken->classificacao == DO){
        *tolken = scan(arq);
        if(parser_comando(arq, tolken)){

            if(tolken->classificacao == WHILE){
                *tolken = scan(arq);

                if(tolken->classificacao == PARENTESIS_ABERTO){
                    *tolken = scan(arq);
                    parser_exp_rela(arq, tolken);    

                    if(tolken->classificacao == PARENTESIS_FECHADO){
                        *tolken = scan(arq);

                        if(tolken->classificacao == PONTO_E_VIRGULA){
                            *tolken = scan(arq);
                        }else{
                            parser_erro(tolken, linha,coluna, PONTO_E_VIRGULA);
                            //ERRO PRECISA DE UM PONTO E VIRGULA
                        }
                    }
                    else{
                        parser_erro(tolken, linha,coluna, PARENTESIS_FECHADO);
                        //ERRO PRECISA DE ")"
                    }
                }
                else{
                    parser_erro(tolken, linha,coluna, PARENTESIS_ABERTO);
                    //ERRO FALTOU "("
                }            
            }else{
                parser_erro(tolken, linha,coluna, WHILE);
                //ERRO FALTOU WHILE
            }
        }
        else{
            parser_erro(tolken, linha,coluna, COMANDO);
            //ERRO FALTOU UM COMANDO
        }
    }
}

void parser_exp_rela(FILE * arq, Ttolken * tolken){
    printf("expr_relacional =>");
    parser_exp_arit(arq, tolken);
    if(tolken->classificacao == IGUAL || tolken->classificacao == MAIOR_OU_IGUAL || tolken->classificacao == MAIOR_QUE || 
    tolken->classificacao == MENOR_OU_IGUAL || tolken->classificacao == MENOR_QUE || tolken->classificacao == DIFERENTE){
        printf(" %s ",tolken->lexema);
       *tolken = scan(arq);
        parser_exp_arit(arq, tolken);
    }else{
        parser_erro(tolken, linha,coluna, OP_RELACIONAL);
        //ERRO ESPERAVA UM OP RELACIONAL
    }
}

void parser_cmd_if(FILE * arq, Ttolken * tolken){
    if(tolken->classificacao == IF){
        *tolken = scan(arq);
        if(tolken->classificacao == PARENTESIS_ABERTO){
            *tolken = scan(arq);
            parser_exp_rela(arq, tolken);
            if(tolken->classificacao == PARENTESIS_FECHADO){
                *tolken = scan(arq);
                parser_comando(arq, tolken);
                if(tolken->classificacao == ELSE){
                    *tolken = scan(arq);
                    parser_comando(arq, tolken);
                }
            }else{
                parser_erro(tolken, linha, coluna, PARENTESIS_FECHADO);
                //ERRO ESPERAVA ')'
            }
        }else{
            parser_erro(tolken, linha,coluna, PARENTESIS_ABERTO);
            //ERRO ESPERAVA '('
        }
    }
}