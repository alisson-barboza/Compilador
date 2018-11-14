#include "TolkenStack.h"
#include "TabelaSimb.h"

void push(TStack ** stack, TNewTolken tolken){
    TStack * novo;

    novo = (TStack *) malloc(sizeof(TStack));

    novo->ntk.escopo = tolken.escopo;
    strcpy(novo->ntk.lexema, tolken.lexema);
    novo->ntk.tipo = tolken.tipo;
    novo->prox = NULL;

    if((*stack) == NULL){
        (*stack) = novo;            
    }else{
        novo->prox = (*stack);
        (*stack) = novo;        
    }
}

int buscarNoEscopo(TStack * stack, TNewTolken tolken){
    TStack * passeio;    
    
    if(stack != NULL){
        
        passeio = stack;
        while(passeio != NULL && passeio->ntk.escopo == tolken.escopo ){        
            if(strcmp(passeio->ntk.lexema, tolken.lexema) == 0){
                return 1;
            }else{
                passeio = passeio->prox;
            }
        }        
    }
    return 0;
}

int buscarTipo(TStack * stack, Ttolken tolken){
    TStack * aux;
    aux = stack;
    while(aux != NULL){
        if(strcmp(aux->ntk.lexema, tolken.lexema) == 0){
            return aux->ntk.tipo;
        }else{
            aux = aux->prox;
        }
    }
    return -1;
}

int buscaGeral(TStack * stack, TNewTolken tolken){
    TStack * passeio;
    if(stack != NULL){
        passeio = stack;

        while ( passeio != NULL){
            if(strcmp(passeio->ntk.lexema, tolken.lexema) == 0){
                return 1;
            }else{
                passeio = passeio->prox;
            }
        }        
    }
    return 0;
}

void limparEscopo(TStack ** stack, int escopo){
    TStack * aux, * lixo;
    if(stack != NULL){
        aux = (*stack);
        while(aux != NULL && aux->ntk.escopo == escopo){
            lixo = aux;
            aux = aux->prox;
            free(lixo);
        }
        (*stack) = aux;
    }
}