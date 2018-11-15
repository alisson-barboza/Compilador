#include "Scanner_Library.h"
#include "Parser_Library.h"
#include "Globals.h"


FILE * abrirArquivo(char nome[]) {
	FILE * aux;
	aux = fopen(nome, "r+b");
	return aux;
}


int main(int argc, char * argv[]) {
	FILE * arq;


	if (argc != 2) {
		printf("\nComando nao reconhecido favor digitar o executavel e o arquivo a ser compilado.");
		return 0;
	}
	else {
		arq = abrirArquivo(argv[1]);		
		parser_programa(arq);

		fclose(arq);
		return 0;
	}
}