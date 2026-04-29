#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "parser.tab.h"

extern FILE* yyin;
extern int yyparse();
extern Node* root;

/* Quadruple Structure */
struct quadruple {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} quads[200];
int q_idx = 0;

void add_quad(char* op, char* a1, char* a2, char* res) {
    strcpy(quads[q_idx].op, op);
    strcpy(quads[q_idx].arg1, a1);
    strcpy(quads[q_idx].arg2, a2);
    strcpy(quads[q_idx].result, res);
    q_idx++;
}

/* Generators for names */
int t_cnt = 1;
int l_cnt = 1;
char* new_temp() {
    char* s = malloc(10);
    sprintf(s, "t%d", t_cnt++);
    return s;
}
char* new_label() {
    char* s = malloc(10);
    sprintf(s, "L%d", l_cnt++);
    return s;
}

/* REAL RECURSIVE GENERATOR (ICG) */
char* generate_icg(Node* n) {
    if (!n) return "";

    if (strcmp(n->name, "ASSIGN") == 0) {
        char* val = generate_icg(n->right);
        add_quad("=", val, " ", n->left->name);
        return n->left->name;
    } 
    else if (strcmp(n->name, "IF") == 0) {
        char* cond = generate_icg(n->left);
        char* l1 = new_label();
        char* l2 = new_label();
        add_quad("IF_GOTO", cond, " ", l1);
        add_quad("GOTO", " ", " ", l2);
        add_quad("LABEL", " ", " ", l1);
        generate_icg(n->right);
        add_quad("LABEL", " ", " ", l2);
    }
    else if (strcmp(n->name, "IF_ELSE") == 0) {
        char* cond = generate_icg(n->left);
        char* l_true = new_label();
        char* l_false = new_label();
        char* l_end = new_label();
        
        add_quad("IF_GOTO", cond, " ", l_true);
        add_quad("GOTO", " ", " ", l_false);
        
        add_quad("LABEL", " ", " ", l_true);
        generate_icg(n->right->left); // True branch
        add_quad("GOTO", " ", " ", l_end);
        
        add_quad("LABEL", " ", " ", l_false);
        generate_icg(n->right->right); // False branch
        add_quad("LABEL", " ", " ", l_end);
    }
    else if (strcmp(n->name, "RELOP") == 0) {
        char* t = new_temp();
        add_quad(">", n->left->name, n->right->name, t);
        return t;
    }
    else if (strcmp(n->name, "PRINT") == 0) {
        add_quad("PRINT", n->left->name, " ", " ");
    }
    else if (strcmp(n->name, "RET") == 0) {
        char* val = generate_icg(n->left);
        add_quad("RET", val, " ", " ");
    }
    else if (strcmp(n->name, "STMT_LIST") == 0 || strcmp(n->name, "FUNC_MAIN") == 0) {
        generate_icg(n->left);
        generate_icg(n->right);
    }
    else {
        /* Base case: Constant or ID */
        return n->name;
    }
    return "";
}

/* Phase 4: ICG Output */
void print_icg() {
    printf("========================================\n");
    printf("4. INTERMEDIATE CODE GENERATION\n");
    printf("========================================\n");
    printf("Index\tOp\tArg1\tArg2\tResult\n");
    printf("----------------------------------------\n");
    for(int i=0; i<q_idx; i++) {
        printf("%d\t%s\t%s\t%s\t%s\n", i, quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }
}

/* Phase 6: Target Code Logic (Dynamic based on Quads) */
void generate_target() {
    printf("\n========================================\n");
    printf("6. TARGET CODE GENERATION\n");
    printf("========================================\n");
    for(int i=0; i<q_idx; i++) {
        if (strcmp(quads[i].op, "=") == 0) {
            printf("MOV R1, %s\nMOV %s, R1\n", quads[i].arg1, quads[i].result);
        } else if (strcmp(quads[i].op, ">") == 0) {
            printf("CMP %s, %s\n", quads[i].arg1, quads[i].arg2);
        } else if (strcmp(quads[i].op, "IF_GOTO") == 0) {
            printf("JG %s\n", quads[i].result);
        } else if (strcmp(quads[i].op, "GOTO") == 0) {
            printf("JMP %s\n", quads[i].result);
        } else if (strcmp(quads[i].op, "LABEL") == 0) {
            printf("%s:\n", quads[i].result);
        } else if (strcmp(quads[i].op, "PRINT") == 0) {
            printf("PRINT %s\n", quads[i].arg1);
        } else if (strcmp(quads[i].op, "RET") == 0) {
            printf("RET %s\n", quads[i].arg1);
        }
    }
    printf("HALT\n");
}

int main() {
    yyin = fopen("main.c", "r");
    if (!yyin) return 1;

    printf("--- RUNNING REAL COMPILER FRONTEND ---\n");
    if (yyparse() == 0) {
        printf("\n--- PARSE TREE ---\n");
        printTree(root, 0);

        /* Real ICG Extraction */
        generate_icg(root);

        print_icg();
        generate_target();
    }

    fclose(yyin);
    return 0;
}
