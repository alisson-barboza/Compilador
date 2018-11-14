#include "TabelaSimb.h"
#include "Parser_Library.h"

void push(TStack ** stack, TNewTolken tolken);
int buscarNoEscopo(TStack * stack, TNewTolken tolken);
int buscaGeral(TStack * stack, TNewTolken tolken);
void limparEscopo(TStack ** stack, int escopo);

int buscarTipo(TStack * stack, Ttolken tolken);
