#include "Parser_Library.h"
#include "TolkenStack.h"
//Lembrar de fazer o fator interativo já que recursivo ele faz as operçaões ao contrário, ex.: a * b / c
//ele faz primeiro o b/c pra depois multiplicar
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
    case TERMO:
      printf("\nEsperado um termo");
      break;
        default:
            break;    
    }
    printf("\nUltimo tolken lido: %s", tolken->lexema);
    exit(-1);
}

void semantico_erro(Ttolken * tolken, int linha, int coluna, int classificacao_do_erro){
    printf("\nErro na linha %d, coluna %d", linha, coluna);
    switch(classificacao_do_erro){
        case ERRO_VARIAVEL_DECLARADA:
            printf("\nVariavel com mesmo nome ja foi declarada");
            break;
        case ERRO_VARIAVEL_NAO_DECLARADA:
            printf("\nVariavel nao foi declarada");
            break;
        case ERRO_TIPOS_INCOMPATIVEIS:
            printf("\nErro tipos incompativeis");
            break;
        default:
            break;
    }
    printf("\nUltimo tolken lido: %s", tolken->lexema);
    exit(-1);
}


void parser_programa(FILE * arq){
    Ttolken tolken;
    varTemp = 0;
    label = 0;
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
   
    if(tolken->classificacao == CHAVE_ABERTA){
        *tolken = scan(arq);
        if(tolken->classificacao == INT || tolken->classificacao == FLOAT || tolken->classificacao == CHAR){
            while(parser_decl_var(arq, tolken)){                           
            }
        }
        while(parser_comando(arq, tolken)){
        }
        
        if(tolken->classificacao == CHAVE_FECHADA){
            limparEscopo(&pilha, escopo);
            escopo--;            
            //De novo a lógica do lookahead pois pode ser que alguem tenha chamado essa função além do começo do programa
            //então movo a cabeça de leitura para facilitar
            
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

void inserirTolken(Ttolken * tolken, int tipo){
    TNewTolken newTolken;

    newTolken = criarTolken(*tolken, tipo);
    if(!buscarNoEscopo(pilha, newTolken)){                
        push(&pilha, newTolken);
        
    }else{
        //ERRO VARIAVEL JÁ FOI DECLARADA
        semantico_erro(tolken, linha, coluna, ERRO_VARIAVEL_DECLARADA);
    }
}

int parser_decl_var(FILE * arq, Ttolken * tolken){
    
    if(tolken->classificacao == INT){
        *tolken = scan(arq);
        if(tolken->classificacao == IDENTIFICADOR){
            inserirTolken(tolken, TIPO_INT);        

            *tolken = scan(arq);
            while(tolken->classificacao == VIRGULA){
                *tolken = scan(arq);
                if(tolken->classificacao == IDENTIFICADOR){
                    inserirTolken(tolken, TIPO_INT);

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
            inserirTolken(tolken, TIPO_FLOAT);

            *tolken = scan(arq);
            while(tolken->classificacao == VIRGULA){
                *tolken = scan(arq);
                if(tolken->classificacao == IDENTIFICADOR){
                    inserirTolken(tolken, TIPO_FLOAT);

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
            inserirTolken(tolken, TIPO_CHAR);

            *tolken = scan(arq);
            while(tolken->classificacao == VIRGULA){
                *tolken = scan(arq);
                if(tolken->classificacao == IDENTIFICADOR){
                    inserirTolken(tolken, TIPO_CHAR);
                    
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

TNewTolken criarTolken(Ttolken tolken, int tipo){
    TNewTolken nt;

    strcpy(nt.lexema, tolken.lexema);
    nt.tipo = tipo;
    nt.escopo = escopo;    

    return nt;
}

void parser_cmd_basico(FILE * arq, Ttolken * tolken){
    
    if(tolken->classificacao == IDENTIFICADOR){
        if(buscaGeral(pilha, criarTolken(*tolken, buscarTipo(pilha, *tolken)))){
            parser_atribuicao(arq, tolken);
        }else{
            semantico_erro(tolken, linha, coluna, ERRO_VARIAVEL_NAO_DECLARADA);
            
            // ERRO VARIAVEL NAO DECLARADA
        }
    }
    else if(tolken->classificacao == CHAVE_ABERTA){
        escopo ++;
        parser_bloco(arq, tolken);

    }
}

void parser_atribuicao(FILE * arq, Ttolken *tolken){
    TNewTolken ntk1, ntk2;
    if(tolken->classificacao == IDENTIFICADOR){
        ntk1 = criarTolken(*tolken, buscarTipo(pilha, *tolken));
       

        *tolken = scan(arq);

        if(tolken->classificacao == ATRIBUICAO){
            *tolken = scan(arq);
            
            ntk2 = parser_exp_arit(arq, tolken);
            

            
            //checagem de tipos
            if(ntk1.tipo == TIPO_INT && (ntk2.tipo == TIPO_FLOAT || ntk2.tipo == TIPO_CHAR)){
                //erro tipos incompativeis
                semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
            }
            else if(ntk1.tipo == TIPO_FLOAT && ntk2.tipo == TIPO_CHAR){
                //erro tipos incompativeis
                semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
            }else if(ntk1.tipo == TIPO_CHAR && ntk2.tipo != TIPO_CHAR){
                //erro tipos incompativeis
                semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
            }
            //=======================
            printf("\n%s = %s", ntk1.lexema, ntk2.lexema);
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

TNewTolken parser_exp_arit(FILE *arq, Ttolken * tolken){
    TNewTolken ntkFirst, ntkSecond;
    
    ntkSecond = parser_termo(arq, tolken);
    
    while(tolken->classificacao == SOMA || tolken->classificacao == SUBTRACAO){
        
            if(tolken->classificacao == SOMA){   
            *tolken = scan(arq);
            
            ntkFirst = parser_termo(arq, tolken);
	    
                if(ntkFirst.tipo == TIPO_FLOAT && ntkSecond.tipo == TIPO_INT){
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;                    
                    strcat(ntkSecond.lexema, "+");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;

                    ntkSecond.escopo = ntkFirst.escopo;
                    ntkSecond.tipo = ntkFirst.tipo;
                }else if(ntkFirst.tipo == TIPO_INT && ntkSecond.tipo == TIPO_FLOAT){
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkFirst.lexema);
                    sprintf(ntkFirst.lexema, "T%d", varTemp);
                    varTemp++;                    
                    strcat(ntkSecond.lexema, "+");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;
                }
                else if(ntkFirst.tipo == TIPO_CHAR && ntkSecond.tipo != TIPO_CHAR){
                    semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
                }
	            else if(ntkSecond.tipo == TIPO_CHAR && ntkFirst.tipo != TIPO_CHAR){
	                semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
	            }else{
                    strcat(ntkSecond.lexema, "+");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;
                }
            }
            if(tolken->classificacao == SUBTRACAO){
                *tolken = scan(arq);

                ntkFirst = parser_termo(arq, tolken);
	    
                if(ntkFirst.tipo == TIPO_FLOAT && ntkSecond.tipo == TIPO_INT){
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkSecond.lexema);
                        sprintf(ntkSecond.lexema, "T%d", varTemp);
                        varTemp++;                    
                        strcat(ntkSecond.lexema, "-");             
                        strcat(ntkSecond.lexema, ntkFirst.lexema);
                        printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                        sprintf(ntkSecond.lexema, "T%d", varTemp);
                        varTemp++;

                        ntkSecond.escopo = ntkFirst.escopo;
                        ntkSecond.tipo = ntkFirst.tipo;
                }
                else if(ntkFirst.tipo == TIPO_INT && ntkSecond.tipo == TIPO_FLOAT){
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkFirst.lexema);
                    sprintf(ntkFirst.lexema, "T%d", varTemp);
                    varTemp++;                    
                    strcat(ntkSecond.lexema, "-");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;
                }
                else if(ntkFirst.tipo == TIPO_CHAR && ntkSecond.tipo != TIPO_CHAR){
                    semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
                }
	            else if(ntkSecond.tipo == TIPO_CHAR && ntkFirst.tipo != TIPO_CHAR){
	                semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
	            }else{
                    strcat(ntkSecond.lexema, "-");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;
                }
            }

        
    }
    return ntkSecond;
}

TNewTolken parser_termo(FILE * arq, Ttolken * tolken){
    TNewTolken ntkFirst, ntkSecond;
    
    ntkSecond = parser_fator(arq, tolken);
    
    while(tolken->classificacao == DIVISAO || tolken->classificacao == MULTIPLICACAO){
               
            if(tolken->classificacao == DIVISAO){            
                *tolken = scan(arq);
                ntkFirst = parser_fator(arq, tolken);
                
                if(ntkFirst.tipo == TIPO_FLOAT && ntkSecond.tipo == TIPO_INT){
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;                    
                    strcat(ntkSecond.lexema, "/");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;

                    ntkSecond.escopo = ntkFirst.escopo;
                    ntkSecond.tipo = ntkFirst.tipo;
                }
                else if(ntkFirst.tipo == TIPO_INT && ntkSecond.tipo == TIPO_FLOAT){
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkFirst.lexema);
                    sprintf(ntkFirst.lexema, "T%d", varTemp);
                    varTemp++;                    
                    strcat(ntkSecond.lexema, "/");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;
                }
                else if(ntkFirst.tipo == TIPO_CHAR && ntkSecond.tipo != TIPO_CHAR){
                    semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
                }
	            else if(ntkSecond.tipo == TIPO_CHAR && ntkFirst.tipo != TIPO_CHAR){
    	            semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
	            }else{
                    if(ntkFirst.tipo == TIPO_INT && ntkSecond.tipo == TIPO_INT){
                        ntkSecond.tipo = TIPO_FLOAT;
                    }
                    strcat(ntkSecond.lexema, "/");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;
                }

            }
            if(tolken->classificacao == MULTIPLICACAO){            
                *tolken = scan(arq);
                ntkFirst = parser_fator(arq, tolken);
                
                if(ntkFirst.tipo == TIPO_FLOAT && ntkSecond.tipo == TIPO_INT){
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;                    
                    strcat(ntkSecond.lexema, "*");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;

                    ntkSecond.escopo = ntkFirst.escopo;
                    ntkSecond.tipo = ntkFirst.tipo;
                }
                else if(ntkFirst.tipo == TIPO_INT && ntkSecond.tipo == TIPO_FLOAT){
                    
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkFirst.lexema);
                    sprintf(ntkFirst.lexema, "T%d", varTemp);
                    varTemp++;
                    strcat(ntkSecond.lexema, "*");
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;

                    /*
                    printf("\nT%d = (FLOAT) %s", varTemp, ntkFirst.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;                    
                    strcat(ntkSecond.lexema, "*");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;
                    */
                }
                else if(ntkFirst.tipo == TIPO_CHAR && ntkSecond.tipo != TIPO_CHAR){
                    semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
                }
	            else if(ntkSecond.tipo == TIPO_CHAR && ntkFirst.tipo != TIPO_CHAR){
    	            semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
	            }else{
                    strcat(ntkSecond.lexema, "*");             
                    strcat(ntkSecond.lexema, ntkFirst.lexema);
                    printf("\nT%d = %s", varTemp, ntkSecond.lexema);
                    sprintf(ntkSecond.lexema, "T%d", varTemp);
                    varTemp++;
                }

            }
            
       
    }
    
    return ntkSecond;
}

TNewTolken parser_fator(FILE * arq, Ttolken * tolken){
    TNewTolken ntk;
    if(tolken->classificacao == PARENTESIS_ABERTO){        
        *tolken = scan(arq);
        ntk =  parser_exp_arit(arq, tolken);
        if(tolken->classificacao == PARENTESIS_FECHADO){
            *tolken = scan(arq);
	        return ntk;
        }else{
            parser_erro(tolken, linha,coluna, PARENTESIS_FECHADO);
            //ERRO ESPERAVA UM PARENTESIS FECHADO
        }
    }
    else if(tolken->classificacao == IDENTIFICADOR|| tolken->classificacao == TIPO_FLOAT ||
        tolken->classificacao == TIPO_INT || tolken->classificacao == TIPO_CHAR){
        if(tolken->classificacao != IDENTIFICADOR){
            ntk = criarTolken(*tolken, tolken->classificacao);

        }else{
            ntk = criarTolken(*tolken, buscarTipo(pilha, *tolken));

	        if(!buscaGeral(pilha, ntk)){
	            semantico_erro(tolken,linha,coluna,ERRO_VARIAVEL_NAO_DECLARADA);
	        }
        }
        
        *tolken = scan(arq);

        return ntk;
    }else{
        parser_erro(tolken, linha,coluna, ESPECIAL);
        //ERRO ESPERAVA UM ID, INTEIRO, FLOAT OU CHAR, NÃO LI NADA DISSO
    }

}

void parser_cmd_iteracao(FILE * arq, Ttolken * tolken){
    TNewTolken ntk;
    int l1 = label;

    if(tolken->classificacao == WHILE){
       printf("\nL%d:", l1);
       l1++;
       label++;
        
        *tolken = scan(arq);

        if(tolken->classificacao == PARENTESIS_ABERTO){
            *tolken = scan(arq);
            ntk = parser_exp_rela(arq, tolken);
            printf("\nif %s != 0 goto L%d", ntk.lexema, l1);
            l1--;
            
            if(tolken->classificacao == PARENTESIS_FECHADO){
                *tolken = scan(arq);

                if(!parser_comando(arq, tolken)){/*teste pra saber se ele tem pelo menos um comando*/
                    parser_erro(tolken, linha,coluna, COMANDO);
                    //ERRO PRECISA DE UM COMANDO
                }
                
            printf("\ngoto L%d", l1);  
            l1++;
            label++;
            printf("\nL%d: ", l1);
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
        printf("\nL%d: ", l1);
        
        label++;
        if(parser_comando(arq, tolken)){

            if(tolken->classificacao == WHILE){
                *tolken = scan(arq);

                if(tolken->classificacao == PARENTESIS_ABERTO){
                    *tolken = scan(arq);
                    ntk = parser_exp_rela(arq, tolken);
                      
                    printf("\nif %s != 0 goto L%d", ntk.lexema, l1);
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

TNewTolken parser_exp_rela(FILE * arq, Ttolken * tolken){
    TNewTolken ntk1, ntk2;

    ntk1 = parser_exp_arit(arq, tolken);
    if(tolken->classificacao == IGUAL || tolken->classificacao == MAIOR_OU_IGUAL || tolken->classificacao == MAIOR_QUE || 
    tolken->classificacao == MENOR_OU_IGUAL || tolken->classificacao == MENOR_QUE || tolken->classificacao == DIFERENTE){
        
        switch(tolken->classificacao){
            case IGUAL:
            strcat(ntk1.lexema, "==");    
            break;
            case MAIOR_OU_IGUAL:
            strcat(ntk1.lexema, ">=");
            break;
            case MAIOR_QUE:
            strcat(ntk1.lexema, ">");
            break;
            case MENOR_OU_IGUAL:
            strcat(ntk1.lexema, "<=");
            break;
            case MENOR_QUE:
            strcat(ntk1.lexema, "<");
            break;
            case DIFERENTE:
            strcat(ntk1.lexema, "!=");
            break;
        }
       *tolken = scan(arq);
        ntk2 = parser_exp_arit(arq, tolken);
        //===========================================
        if(ntk1.tipo == TIPO_INT && ntk2.tipo == TIPO_CHAR){
                //erro tipos incompativeis
                semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
        }else if(ntk1.tipo == TIPO_FLOAT && ntk2.tipo == TIPO_CHAR){
                //erro tipos incompativeis
                semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
        }else if(ntk1.tipo == TIPO_CHAR && ntk2.tipo != TIPO_CHAR){
                //erro tipos incompativeis
                semantico_erro(tolken, linha, coluna, ERRO_TIPOS_INCOMPATIVEIS);
        }
        //====================================================
        strcat(ntk1.lexema, ntk2.lexema);
        printf("\nT%d = %s", varTemp, ntk1.lexema);
        sprintf(ntk1.lexema, "T%d", varTemp);
    }else{
        parser_erro(tolken, linha,coluna, OP_RELACIONAL);
        //ERRO ESPERAVA UM OP RELACIONAL
    }
    return ntk1;
}

void parser_cmd_if(FILE * arq, Ttolken * tolken){
    TNewTolken ntk;
    int l1 = label;
    if(tolken->classificacao == IF){
        *tolken = scan(arq);
        if(tolken->classificacao == PARENTESIS_ABERTO){
            *tolken = scan(arq);
            ntk = parser_exp_rela(arq, tolken);
            printf("\n\nif %s == 0 goto L%d", ntk.lexema, l1);
            label++; 
            l1++;
            if(tolken->classificacao == PARENTESIS_FECHADO){
               
                *tolken = scan(arq);
                parser_comando(arq, tolken);
                if(tolken->classificacao !=ELSE){
                    l1--;
                    printf("\nL%d: ", l1);
                }
                if(tolken->classificacao == ELSE){ 
                    printf("\ngoto L%d", l1);
                    l1--;
                    printf("\nL%d: ", l1);
                    label++;
                    l1++;

                    *tolken = scan(arq);
                    parser_comando(arq, tolken);                   
                    printf("\nL%d: ", l1);
                    l1--;
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
