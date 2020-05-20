/* Definitions */

%{
	#include<stdio.h>
%}

%option noyywrap

%%
/* Rules/tokens */

/* BASIC KEYWORDS */

var { return VAR; } /* variable keyword */
: { return COLON; } /* single colon */
; { return SEMICOLON; } /* semicolon, denotes end of statement */

/* DATA TYPES */

bool|boolean ( return BOOL; )
int { return INT; }
float { return FLOAT; }
doub|double { return DOUBLE; }
char { return CHAR; }
string { return STRING; }
void { return VOID; }

/* Other tokens */

\[ { return L_SQUARE_BRACKET; }
\] { return R_SQUARE_BRACKET; }

\{ { return L_CURLY_BRACKET; }
\} { return R_CURLY_BRACKET; }

\/\/ {} /* single line comment */

\/\*[\s\S]*?\*\/ {} /* multiline comment */

_*[A-Za-z-_]+[0-9]* { return VARIABLE_NAME; } /* variable */

[\s]*[\n]* {} /* whitespace (tabs, spaces, \n) */

%%

/* C code */

int main(void){
	yylex();
	return 0;
}