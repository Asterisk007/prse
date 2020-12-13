/* Definitions */
%option noyywrap
%option nodefault
%option never-interactive
%option nounistd

%{
    #include <iostream>
    #include <algorithm>
    #include "parser.h"

    using namespace std;

    int line_count = 1;
%}

%%

 /* ======== Importing ======== */

use                     { return USE; }

 /* ======== Basic keywords ====== */

let                     { return LET; }
class                   { return CLASS; }
func|function           { return FUNCTION; }
return                  { return RETURN; }
public                  { return PUBLIC; }
private                 { return PRIVATE; }

 /* ======== Flow control ======= */

if                      { return IF; }
else                    { return ELSE; }
for                     { return FOR; }
while                   { return WHILE; }

 /* ======== Data types ======== */

bool|boolean            { return BOOL; }
int                     { return INT; }
doub|double             { return DOUBLE; }
char                    { return CHAR; }
string                  { return STRING; }
void                    { return VOID; }
null                    { return MYNULL; }

 /* ======== Arithmetic ======== */

\=                               { return ASSIGN; }

\+                           { return PLUS; }
\-                           { return MINUS; }
\*                           { return STAR; }
\/                           { return SLASH; }
\^                           { return CARET; }
\%                           { return MODULO; }

"++"                         { return INCREMENT; }
"--"                         { return DECREMENT; }

"+="                         { return PLUS_ASSIGN; }
"-="                         { return MINUS_ASSIGN; }
"*="                         { return STAR_ASSIGN; }
"/="                         { return SLASH_ASSIGN; }

 /* ======= Logical operators ======= */

true                         { yylval.union_bool = true; return LOGIC_TRUE; }
false                        { yylval.union_bool = false; return LOGIC_FALSE; }

"=="                         { return LOGIC_EQ; }
"!="                         { return LOGIC_NE; }
"!"                          { return LOGIC_NOT; }
"&&"                         { return LOGIC_AND; }
"||"                         { return LOGIC_OR; }
">"                          { return LOGIC_GREATER; }
"<"                          { return LOGIC_LESS; }
">="                         { return LOGIC_GREATER_EQUAL; }
"<="                         { return LOGIC_LESS_EQUAL; }

 /* Other tokens */

\.                           { return PERIOD; }
\,                           { return COMMA; }
\:                           { return COLON; } /* for denoting variable types and function return types */
\;                           { return SEMICOLON; } /* semicolon, denotes end of statement */
["]                          { return DOUBLE_QUOTE; }
\'                           { return SINGLE_QUOTE; }
\(                           { return L_PAREN; }
\)                           { return R_PAREN; }
\[                           { return L_SQUARE_BRACKET; }
\]                           { return R_SQUARE_BRACKET; }
\{                           { return L_CURLY_BRACKET; }
\}                           { return R_CURLY_BRACKET; }
[$]                          { return DOLLAR; }
in                           { return IN; }

\/\/.*                                  {} /* single line comment */
[/][*][^*]*[*]+([^/*][^*]*[*]+)*[/]     {} /* multiline comment. Thanks to this stackoverflow post: https://stackoverflow.com/a/22390133 */
                            
[0-9]+                       { yylval.union_int = std::stoi(yytext); return INT_CONST; } /* integer constant */
[0-9]*\.[0-9]*               { yylval.union_double = std::stod(yytext); return DOUBLE_CONST; } /* integer constant */

["][^\n\r"{}]*["]            {    /* string constant */
                                    string s = string(yytext);
                                    s.erase(remove(s.begin(), s.end(), '\"'), s.end());
                                    yylval.union_string = new string(s);
                                    return STRING_CONST;
                             }
['].[']                      { /* character constant */
                                    string s = string(yytext);
                                    s.erase(remove(s.begin(), s.end(), '\''), s.end());
                                    yylval.union_string = new string(s);
                                    return CHAR_CONST;
                             }

[A-Za-z_][a-zA-Z0-9_]*       { yylval.union_string = new string(yytext); return ID; } /* variable */

(\r\n?|\n)                   { line_count++; } /* newlines and CRLF */
[\t\ ]                       {} /* whitespace: tabs and spaces */
.                            { return ERROR; }

%%
