#include "Scanner_Library.h"
#include "Parser_Library.h"
#include "Globals.h"


FILE * criar_Arquivo(char nome[]) {
	FILE * aux;
	aux = fopen(nome, "r+b");
	return aux;
}


int main(int argc, char * argv[]) {
	FILE * arq;
	Ttolken tk;
	linha = 1;
	coluna = 0;
	pilha = NULL;
	escopo = 0;
	if (argc != 2) {
		printf("\nComando nao reconhecido favor digitar o executavel e o arquivo a ser compilado.");
		return 0;
	}
	else {
		arq = criar_Arquivo(argv[1]);
		
		parser_programa(arq);
		return 0;
	}
}