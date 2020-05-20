/* Definitions */

%{
	#include<stdio.h>
%}

%option noyywrap

%%
/* Rules/tokens */

/* BASIC KEYWORDS */

var 			{ return VAR; } /* variable keyword */
func|fn 		{ return FUNCTION; }
return 			{ return RETURN; }

/* ======== DATA TYPES ======== */

bool|boolean 	{ return BOOL; }
int 			{ return INT; }
float 			{ return FLOAT; }
doub|double 	{ return DOUBLE; }
char 			{ return CHAR; }
string 			{ return STRING; }
void 			{ return VOID; }

/* ======== Arithmetic ======== */

\+ 				{ return PLUS; }
\- 				{ return MINUS; }
\* 				{ return STAR; }
\/ 				{ return SLASH; }
\^ 				{ return CARET; }

\+\+ 			{ return INCREMENT; }
\-\- 			{ return DECREMENT; }

/* ======= Logical operators ======= */

== 				{ return LOGIC_EQ; }
!= 				{ return LOGIC_NE; }
&& 				{ return LOGIC_AND; }
|| 				{ return LOGIC_OR; }

/* Other tokens */

: 				{ return COLON; } /* single colon */

; 				{ return SEMICOLON; } /* semicolon, denotes end of statement */

\"				{ return DOUBLE_QUOTE; }
\' 				{ return SINGLE_QUOTE; }

\[ 				{ return L_SQUARE_BRACKET; }
\] 				{ return R_SQUARE_BRACKET; }
\{ 				{ return L_CURLY_BRACKET; }
\} 				{ return R_CURLY_BRACKET; }

\/\/ 			{} /* single line comment */
\/\*[\s\S]*?\*\/ {} /* multiline comment */

_*[A-Za-z-_]+[0-9]* { return VARIABLE_NAME; } /* variable */

[\	]* {  } /* whitespace (tabs, spaces) */
[\n]* {  } /* whitespace (\n) */

%%

/* C code */

int main(void){
	yylex();
	return 0;
}