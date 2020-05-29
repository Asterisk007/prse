/* Definitions */

%{
	#include <iostream>
	#include "prse.tab.h"

	int line_count = 1;
%}

%option noyywrap

%%

 /* ======== Importing ======== */

use 						{ return USE; }

 /* ======== Basic keywords ====== */

var 						{ return VAR; }
class						{ return CLASS; }
func	 					{ return FUNCTION; }
main						{ return MAIN; }
return 						{ return RETURN; }

 /* ======== Flow control ======= */

if 							{ return IF; }
else 						{ return ELSE; }
for							{ return FOR; }

 /* ======== Data types ======== */

bool|boolean 				{ return BOOL; }
int 						{ return INT; }
float 						{ return FLOAT; }
doub|double 				{ return DOUBLE; }
char 						{ return CHAR; }
string 						{ return STRING; }
void 						{ return VOID; }

 /* ======== Arithmetic ======== */

\=							{ retturn ASSIGN; }

\+ 							{ return PLUS; }
\- 							{ return MINUS; }
\* 							{ return STAR; }
\/ 							{ return SLASH; }
\^ 							{ return CARET; }

"++" 						{ return INCREMENT; }
"--" 						{ return DECREMENT; }

"+="						{ return PLUS_ASSIGN; }
"-="						{ return MINUS_ASSIGN; }

 /* ======= Logical operators ======= */

true						{ yylval.union_bool = true; return LOGIC_TRUE; }
false						{ yylval.union_bool = false; return LOGIC_FALSE; }

"==" 						{ return LOGIC_EQ; }
"!=" 						{ return LOGIC_NE; }
"!"							{ return LOGIC_NOT; }
"&&" 						{ return LOGIC_AND; }
"||" 						{ return LOGIC_OR; }
">"							{ return LOGIC_GEATER; }
"<"							{ return LOGIC_LESS; }
">="						{ return LOGIC_GREATER_EQUAL; }
"<="						{ return LOGIC_LESS_EQUAL; }

 /* Other tokens */

\.							{ return PERIOD; }
\,							{ return COMMA; }
\: 							{ return COLON; } /* for denoting variable types and function return types */
\; 							{ return SEMICOLON; } /* semicolon, denotes end of statement */
\"							{ return DOUBLE_QUOTE; }
\' 							{ return SINGLE_QUOTE; }
\(							{ return L_PAREN; }
\)							{ return R_PAREN; }
\[ 							{ return L_SQUARE_BRACKET; }
\] 							{ return R_SQUARE_BRACKET; }
\{ 							{ return L_CURLY_BRACKET; }
\} 							{ return R_CURLY_BRACKET; }

\/\/ 						{} /* single line comment */
\/\*[\s\S]*?\*\/ 			{} /* multiline comment */
							
[0-9]+						{ yylval.union_int = std::stoi(yytext); return INT_CONST; } /* integer constant */
[0-9]*\.[0-9]*				{ yylval.union_double = std::stod(yytext); return DOUBLE_CONST; } /* integer constant */
["][^\n\r"]*["] 			{	/* string constant */
								std::string s = std::string(yytext);
								s.erase(std::remove(s.begin(), s.end(), '\"'), s.end());
								yylval.union_string = new string(s);
								return STRING_CONST;
							}
['].[']						{ /* character constant */
								std::string s = std::string(yytext);
								s.erase(std::remove(s.begin(), s.end(), '\''), s.end());
								yylval.union_char = char(s);
								return CHAR_CONST;
							}
[A-Za-z_][a-zA-Z0-9_]* 		{ yylval.union_string=new std::string(yytext); return ID; } /* variable */

[\	]*						{ /* return TAB; */ } /* whitespace: tabs */
[\ ]* 						{ /* return SPACE; */ } /* whitespace: space */
[\n]* 						{ line_count++; /* return NEWLINE; */ } /* whitespace: \n */
.							{ return ERROR; }
%%

/* C code */

int main(void){
	yylex();
	return 0;
}