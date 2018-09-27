#include "tk.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Globals.h"


Ttolken scan(FILE * arq);
void printError(Ttolken tk, int linha, int coluna);
