#include "Scanner_Library.h"



void printError(Ttolken tk, int linha, int coluna) {
	
	printf("\nERRO: LINHA %d COLUNA: %d", linha, coluna);
	switch (tk.classificacao) {
	case ERRO_CARACTER_INVALIDO:
		printf("\nCARACTER INVALIDO %s", tk.lexema);
		break;
	case ERRO_DIFERENTE:
		printf("\nESPERADO '=' APOS O '!' ");
		break;
	case ERRO_PONTO_FLUTUANTE:
		printf("\nESPERADO UM NUMERO APOS O '.' ");
		break;
	case ERRO_COMENTARIO_NAO_TERMINADO:
		printf("\nESPERADO UM TERMINO DE COMENTARIO");
		break;
	case ERRO_TIPO_CHAR:
		printf("\nESPERADO UM CHAR (CHAR MAL FORMADO)");
		break;
	}
	printf("\nULTIMO CARACTER LIDO %s", tk.lexema);
	exit(-1);
}

Ttolken scan(FILE * arq) {
	static char c = ' ';
	char string[30];
	int i = 0;
	
	Ttolken tk;

	while (1) {
		while (isspace(c)) {
			c = fgetc(arq);
			coluna++;
			if (c == '\n') {
				coluna = 0;
				linha++;
			}
			if (c == '\t') {
				coluna = coluna + 3;
			}
			
		}
		if (c == ';') {
			tk.classificacao = PONTO_E_VIRGULA;
			strcpy(tk.lexema, "PONTO_E_VIRUGLA");
			c = fgetc(arq);
			coluna++;
			return tk;
		}
		else if (isdigit(c)) {
			while (isdigit(c)) {
				string[i] = c;
				i++;
				c = fgetc(arq);
				coluna++;
			}
			if (c == '.') {//O NUMERO LIDO � UM PONTO FLUTUANTE (PROVAVELMENTE)
				string[i] = c;
				i++;
				c = fgetc(arq);
				coluna++;
				if (isdigit(c)) {
					while (isdigit(c)) {
						string[i] = c;
						i++;
						c = fgetc(arq);
						coluna++;
					}
					string[i] = '\0';
					tk.classificacao = TIPO_FLOAT;
					strcpy(tk.lexema, string);
					return tk;
				}
				else {
					string[i] = '\0';
					strcpy(tk.lexema, string);
					tk.classificacao = ERRO_PONTO_FLUTUANTE;
					printError(tk, linha, coluna);

					//DEVOLVER UMA MENSAGEM DE ERRO J� QUE EU ESTAVA ESPERANDO UM NUMERO AP�S O . E ELE N�O APARECEU
					
				}
			}
			else {// O NUMERO LIDO � UM INTEIRO (PROVAVELMTENTE RS)
				string[i] = '\0';
				tk.classificacao = TIPO_INT;
				strcpy(tk.lexema, string);
				return tk;
			}
		}
		else if (c == '.') {
			string[i] = c;
			i++;
			c = fgetc(arq);
			coluna++;
			if (isdigit(c)) {
				while (isdigit(c)) {
					string[i] = c;
					i++;
					c = fgetc(arq);
					coluna++;
				}
				string[i] = '\0';
				tk.classificacao = TIPO_FLOAT;
				strcpy(tk.lexema, string);
				c = fgetc(arq);
				coluna++;
				return tk;
			}
			else {
				string[i] = '\0';
				strcpy(tk.lexema, string);
				tk.classificacao = ERRO_PONTO_FLUTUANTE;
				printError(tk, linha,coluna);
				//DEVOLVER UMA MENSAGEM DE ERRO J� QUE EU ESTAVA ESPERANDO UM NUMERO AP�S O . E ELE N�O APARECEU
			}
		}
		else if (isalpha(c) || c == '_') {//IDENTIFICADOR OU PALAVRA RESERVADA
			while (isalpha(c) || c == '_') {
				string[i] = c;
				i++;
				c = fgetc(arq);
				coluna++;
			}
			if (isdigit(c)) {//CHECANDO SE TEM DIGITO POIS SE TIVER ELE J� N�O � MAIS UMA PALAVRA RESERVADA
				while (isalpha(c) || isdigit(c) || c == '_') {
					string[i] = c;
					i++;
					c = fgetc(arq);
					coluna++;
				}
				string[i] = '\0';
				tk.classificacao = IDENTIFICADOR;
				strcpy(tk.lexema, string);
				return tk;
			}
			else {
				string[i] = '\0';
				//PALAVRAS RESERVADAS ============================================
				if (strcmp(string, "for") == 0) {
					tk.classificacao = FOR;
					strcpy(tk.lexema, "FOR");

					return tk;
				}
				else if (strcmp(string, "if") == 0) {
					tk.classificacao = IF;
					strcpy(tk.lexema, "IF");

					return tk;
				}
				else if (strcmp(string, "else") == 0) {
					tk.classificacao = ELSE;
					strcpy(tk.lexema, "ELSE");

					return tk;
				}
				else if (strcmp(string, "do") == 0) {
					tk.classificacao = DO;
					strcpy(tk.lexema, "DO");

					return tk;
				}
				else if (strcmp(string, "while") == 0) {
					tk.classificacao = WHILE;
					strcpy(tk.lexema, "WHILE");

					return tk;
				}
				else if (strcmp(string, "int") == 0) {
					tk.classificacao = INT;
					strcpy(tk.lexema, "INT");

					return tk;
				}
				else if (strcmp(string, "char") == 0) {
					tk.classificacao = CHAR;
					strcpy(tk.lexema, "CHAR");

					return tk;
				}
				else if (strcmp(string, "float") == 0) {
					tk.classificacao = FLOAT;
					strcpy(tk.lexema, "FLOAT");
					return tk;
				}
				else if (strcmp(string, "main") == 0) {
					tk.classificacao = MAIN;
					strcpy(tk.lexema, "MAIN");
					return tk;
				}
				//==================================================
				//SE PASSOU PELO LOOKUP ENT�O ELE � UM IDENTIFICADOR CUJO NOME N�O CONT�M NUMEROS
				tk.classificacao = IDENTIFICADOR;
				strcpy(tk.lexema, string);
				return tk;
			}
		}
		else if (c == '/') {//CHECANDO SE � UM COMENT�RIO
			c = fgetc(arq);
			coluna++;
			if (c == '/') {//coment�rio duplo "//"
				while (c != '\n' && !feof(arq)) {
					c = fgetc(arq);
					coluna++;
				}
				if (c == '\n') {
					c = fgetc(arq);
					linha++;
					coluna = 0;
					continue;
				}
			}
			else if (c == '*') {//COMENTARIO COM '*'  ex.: /*
				c = fgetc(arq);
				coluna++;
			interno:
				while (c != '*' && !feof(arq)) {
					c = fgetc(arq);
					coluna++;
					if (c == '\n') {
						linha++;
						coluna = 0;
					}
					else if (c == '\t' ) {
						coluna = coluna + 3;
					}
				}
				if (c == '*') {
					c = fgetc(arq);
					coluna++;
					if (c == '/') {
						c = fgetc(arq);
						coluna++;
						continue;
					}
					else {
						goto interno;
					}
				}
				else if(feof(arq)){
					tk.classificacao = ERRO_COMENTARIO_NAO_TERMINADO;
					strcpy(tk.lexema, "/*");
					printError(tk,linha,coluna);
					//DEVOLVER UMA MENSAGEM DE ERRO POIS EU ESTAVA ESPERANDO O TÉRMINO DO COMENTÁRIO ANTES DO EOF

				}
			}
			else {//N�O � COMENT�RIO ENT�O � UMA DIVIS�O (O SINTATICO SE VIRA PRA SABER SE O S�MBOLO EST� NO LOCAL CERTO)
				tk.classificacao = DIVISAO;
				strcpy(tk.lexema, "DIVISAO");
				return tk;
			}
		}
		else if (c == '=') {
			c = fgetc(arq);
			coluna++;
			if (c == '=') {
				tk.classificacao = IGUAL;
				strcpy(tk.lexema, "IGUAL");
				c = fgetc(arq);
				coluna++;
				return tk;
			}
			else {
				tk.classificacao = ATRIBUICAO;
				strcpy(tk.lexema, "ATRIBUICAO");				
				return tk;
			}
		}
		else if(c == '!' ){
			c = fgetc(arq);
			coluna++;
			if (c == '=') {
				tk.classificacao = DIFERENTE;
				strcpy(tk.lexema, "DIFERENTE");
				c = fgetc(arq);
				coluna++;
				return tk;
			}
			else {
				tk.classificacao = ERRO_DIFERENTE;
				strcpy(tk.lexema, " ");
				printError(tk, linha, coluna);
				//MENSAGEM DE ERRO POIS EU ESTAVA ESPERANDO UM "IGUAL" E N�O O LI
			}

		}
		else if (c == '<' ) {
			c = fgetc(arq);
			coluna++;
			if (c == '=') {
				tk.classificacao = MENOR_OU_IGUAL;
				strcpy(tk.lexema, "MENOR_OU_IGUAL");
				c = fgetc(arq);
				coluna++;
				return tk;
			}
			tk.classificacao = MENOR_QUE;
			strcpy(tk.lexema, "MENOR_QUE");
			return tk;
		}
		else if (c == '>' ) {
			c = fgetc(arq);
			coluna++;
			if (c == '=') {
				tk.classificacao = MAIOR_OU_IGUAL;
				strcpy(tk.lexema, "MAIOR_OU_IGUAL");
				c = fgetc(arq);
				coluna++;
				return tk;
			}
			tk.classificacao = MAIOR_QUE;
			strcpy(tk.lexema, "MAIOR_QUE");
			return tk;
		}
		else if (c == '*') {
			c = fgetc(arq);
			coluna++;
			tk.classificacao = MULTIPLICACAO;
			strcpy(tk.lexema, "MULTIPLICACAO");
			return tk;
		}
		else if (c== '+' ) {
			c = fgetc(arq);
			coluna++;
			tk.classificacao = SOMA;
			strcpy(tk.lexema, "SOMA");
			return tk;

		}
		else if (c == '-') {
			c = fgetc(arq);
			coluna++;
			tk.classificacao = SUBTRACAO;
			strcpy(tk.lexema, "SUBTRACAO");
			return tk;
		}
		else if (c == '(') {
			c = fgetc(arq);
			coluna++;
			tk.classificacao = PARENTESIS_ABERTO;
			strcpy(tk.lexema, "PARENTESIS_ABERTO");
			return tk;
		}
		else if (c == ')') {
			c = fgetc(arq);
			coluna++;
			tk.classificacao = PARENTESIS_FECHADO;
			strcpy(tk.lexema, "PARENTESIS_FECHADO");
			return tk;
		}
		else if (c == '{') {
			c = fgetc(arq);
			coluna++;
			tk.classificacao = CHAVE_ABERTA;
			strcpy(tk.lexema, "CHAVE_ABERTA");
			return tk;
		}
		else if (c == '}') {
			c = fgetc(arq);
			coluna++;
			tk.classificacao = CHAVE_FECHADA;
			strcpy(tk.lexema, "CHAVE_FECHADA");
			return tk;
		}
		else if (c == 39) {
			string[i] = c;
			i++;
			c = fgetc(arq);
			coluna++;			
			if (isdigit(c) || isalpha(c)) {
				string[i] = c;
				i++;
				c = fgetc(arq);
				coluna++;
				if (c == 39) {
					string[i] = c;
					i++;
					string[i] = '\0';
					c = fgetc(arq);
					coluna++;
					tk.classificacao = TIPO_CHAR;					
					strcpy(tk.lexema, string);
					return tk;
				}
				else {
					tk.classificacao = ERRO_TIPO_CHAR;
					string[0] = 39;
					string[1] = c;
					string[2] = '\0';
					strcpy(tk.lexema, string);
					printError(tk, linha, coluna);
				}
			}
			else {
				tk.classificacao = ERRO_TIPO_CHAR;
				string[0] = c;
				string[1] = '\0';
				strcpy(tk.lexema, string);
				printError(tk, linha, coluna);				
			}
		}
		else if (c == ',') {
			c = fgetc(arq);
			coluna++;
			tk.classificacao = VIRGULA;
			strcpy(tk.lexema, "VIRGULA");
			return tk;
		}		
		else if (feof(arq)) {
			//RETORNAR UM TOLKIEN DE FIM DE ARQUIVO
			tk.classificacao = FIM_DE_ARQUIVO;
			strcpy(tk.lexema, "EOF");
			return tk;
		}
		else {
			tk.classificacao = ERRO_CARACTER_INVALIDO;
			string[0] = c;
			string[1] = '\n';
			strcpy(tk.lexema, string);
			printError(tk, linha, coluna);
			break;
		}		
		

	}//last condiction
}

