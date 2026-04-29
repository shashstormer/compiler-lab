%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"

void print_token(const char* lexeme, const char* token) {
    printf("%-15s | %-20s\n", lexeme, token);
}
%}

%option noyywrap

%%

"#include"[ ]*<[^>]+>    { print_token(yytext, "Directive"); }
"int"                    { print_token(yytext, "Keyword"); return INT; }
"main"                   { print_token(yytext, "Identifier"); return MAIN; }
"if"                     { print_token(yytext, "Keyword"); return IF; }
"else"                   { print_token(yytext, "Keyword"); return ELSE; }
"return"                 { print_token(yytext, "Keyword"); return RETURN; }
"printf"                 { print_token(yytext, "Identifier"); return PRINTF; }
[a-zA-Z_][a-zA-Z0-9_]*   { print_token(yytext, "Identifier"); yylval.sval = strdup(yytext); return ID; }
-?[0-9]+                 { print_token(yytext, "Constant"); yylval.ival = atoi(yytext); return CONST; }
\"[^\"]*\"               { print_token(yytext, "String Literal"); yylval.sval = strdup(yytext); return STRING; }
"=="|"!="|">="|"<="|">"|"<" { print_token(yytext, "Relational Operator"); return RELOP; }
"="                      { print_token(yytext, "Operator"); return ASSIGN; }
","                      { print_token(yytext, "Separator"); return COMMA; }
";"                      { print_token(yytext, "Delimiter"); return SEMI; }
"("                      { print_token(yytext, "Symbol"); return LPAREN; }
")"                      { print_token(yytext, "Symbol"); return RPAREN; }
"{"                      { print_token(yytext, "Symbol"); return LBRACE; }
"}"                      { print_token(yytext, "Symbol"); return RBRACE; }
[ \t\n]                  { /* ignore whitespace */ }
.                        { printf("Unknown character: %s\n", yytext); }

%%
