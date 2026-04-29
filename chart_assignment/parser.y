%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

void yyerror(const char *s);
int yylex();

extern char* yytext;

Node* createNode(char* name, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->name = strdup(name);
    node->left = left;
    node->right = right;
    return node;
}

void printTree(Node* node, int indent) {
    if (node == NULL) return;
    for (int i = 0; i < indent; i++) printf("  ");
    if (indent > 0) printf("├── ");
    printf("%s\n", node->name);
    printTree(node->left, indent + 1);
    printTree(node->right, indent + 1);
}

Node* root;

%}

%union {
    int ival;
    char* sval;
    struct Node* nval;
}

%token <ival> CONST
%token <sval> ID STRING
%token INT MAIN IF ELSE RETURN PRINTF RELOP ASSIGN COMMA SEMI LPAREN RPAREN LBRACE RBRACE

%type <nval> program function declaration statement if_stmt block call stmt_list expr

%%

program: function { root = $1; }
       ;

function: INT MAIN LPAREN RPAREN block { $$ = createNode("FUNC_MAIN", $5, NULL); }
        ;

block: LBRACE stmt_list RBRACE { $$ = $2; }
     | statement { $$ = $1; }
     ;

stmt_list: statement stmt_list { $$ = createNode("STMT_LIST", $1, $2); }
         | statement { $$ = $1; }
         ;

statement: declaration SEMI { $$ = $1; }
         | if_stmt { $$ = $1; }
         | call SEMI { $$ = $1; }
         | RETURN expr SEMI { $$ = createNode("RET", $2, NULL); }
         ;

declaration: INT ID ASSIGN expr { $$ = createNode("ASSIGN", createNode($2, NULL, NULL), $4); }
           | declaration COMMA ID ASSIGN expr { $$ = createNode("STMT_LIST", $1, createNode("ASSIGN", createNode($3, NULL, NULL), $5)); }
           ;

if_stmt: IF LPAREN expr RELOP expr RPAREN block { 
           Node* cond = createNode("RELOP", $3, $5);
           $$ = createNode("IF", cond, $7); 
       }
       | IF LPAREN expr RPAREN block {
           $$ = createNode("IF", $3, $5);
       }
       | IF LPAREN expr RELOP expr RPAREN block ELSE block { 
           Node* cond = createNode("RELOP", $3, $5);
           Node* branches = createNode("BRANCHES", $7, $9);
           $$ = createNode("IF_ELSE", cond, branches); 
       }
       | IF LPAREN expr RPAREN block ELSE block {
           Node* branches = createNode("BRANCHES", $5, $7);
           $$ = createNode("IF_ELSE", $3, branches);
       }
       ;

call: PRINTF LPAREN STRING RPAREN { $$ = createNode("PRINT", createNode($3, NULL, NULL), NULL); }
    ;

expr: ID { $$ = createNode($1, NULL, NULL); }
    | CONST { char buf[10]; sprintf(buf, "%d", $1); $$ = createNode(buf, NULL, NULL); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s at %s\n", s, yytext);
}
