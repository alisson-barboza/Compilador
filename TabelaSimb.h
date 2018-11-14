#ifndef TABELA_DE_SIMBOLOS
#define TABELA_DE_SIMBOLOS

typedef struct TNewTolken {

    char lexema[30];
    int tipo;
    int escopo;

}TNewTolken;

typedef struct TStack {

    TNewTolken ntk; 
    struct TStack * prox;

}TStack;


#endif