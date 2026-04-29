#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"
#include "parser.tab.h"

extern FILE* yyin;
extern int yyparse();
extern Node* root;

/* Quadruple Structure */
struct quadruple {
    char op[100];
    char arg1[100];
    char arg2[100];
    char result[100];
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

/* Constant Propagation Table */
struct {
    char name[20];
    int value;
    int known;
} constants[100];
int c_idx = 0;

void set_const(char* name, int val) {
    for(int i=0; i<c_idx; i++) {
        if(strcmp(constants[i].name, name) == 0) {
            constants[i].value = val;
            constants[i].known = 1;
            return;
        }
    }
    strcpy(constants[c_idx].name, name);
    constants[c_idx].value = val;
    constants[c_idx].known = 1;
    c_idx++;
}

int get_const(char* name, int* val) {
    for(int i=0; i<c_idx; i++) {
        if(strcmp(constants[i].name, name) == 0 && constants[i].known) {
            *val = constants[i].value;
            return 1;
        }
    }
    return 0;
}

int opt_enabled = 0;

/* REAL RECURSIVE GENERATOR (ICG) */
char* generate_icg(Node* n) {
    if (!n) return "";

    if (strcmp(n->name, "ASSIGN") == 0) {
        char* val_str = generate_icg(n->right);
        add_quad("=", val_str, " ", n->left->name);
        
        if (opt_enabled) {
            /* Constant Propagation: Check if RHS is a constant */
            if (isdigit(val_str[0]) || (val_str[0] == '-' && isdigit(val_str[1]))) {
                set_const(n->left->name, atoi(val_str));
                printf("[OPTIMIZER] Constant Propagation: %s = %d\n", n->left->name, atoi(val_str));
            } else {
                /* If not constant, mark as unknown */
                for(int i=0; i<c_idx; i++) if(strcmp(constants[i].name, n->left->name) == 0) constants[i].known = 0;
            }
        }
        return n->left->name;
    } 
    else if (strcmp(n->name, "IF") == 0) {
        char* cond = generate_icg(n->left);
        if (opt_enabled) {
            if (strcmp(cond, "0") == 0) {
                printf("[OPTIMIZER] Dead Branch Elimination: if(0) detected. Skipping.\n");
                return "";
            }
            if (strcmp(cond, "1") == 0) {
                printf("[OPTIMIZER] Conditional Folding: if(1) detected. Inlining branch.\n");
                generate_icg(n->right);
                return "";
            }
        }
        
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
        
        if (opt_enabled) {
            if (strcmp(cond, "1") == 0) {
                printf("[OPTIMIZER] Conditional Folding: Condition is TRUE. Keeping only true branch.\n");
                generate_icg(n->right->left);
                return "";
            }
            if (strcmp(cond, "0") == 0) {
                printf("[OPTIMIZER] Conditional Folding: Condition is FALSE. Keeping only else branch.\n");
                generate_icg(n->right->right);
                return "";
            }
        }

        char* l_true = new_label();
        char* l_false = new_label();
        char* l_end = new_label();
        
        add_quad("IF_GOTO", cond, " ", l_true);
        add_quad("GOTO", " ", " ", l_false);
        
        add_quad("LABEL", " ", " ", l_true);
        generate_icg(n->right->left);
        add_quad("GOTO", " ", " ", l_end);
        
        add_quad("LABEL", " ", " ", l_false);
        generate_icg(n->right->right);
        add_quad("LABEL", " ", " ", l_end);
    }
    else if (strcmp(n->name, "RELOP") == 0) {
        if (opt_enabled) {
            int v1, v2;
            int k1 = get_const(n->left->name, &v1);
            int k2 = 0;
            if (isdigit(n->right->name[0]) || (n->right->name[0] == '-' && isdigit(n->right->name[1]))) {
                v2 = atoi(n->right->name);
                k2 = 1;
            } else {
                k2 = get_const(n->right->name, &v2);
            }

            if (k1 && k2) {
                int res = (v1 > v2);
                printf("[OPTIMIZER] Constant Folding: %d > %d evaluated to %d\n", v1, v2, res);
                return res ? "1" : "0";
            }
        }

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

/* Phase 5: Dead Code Elimination (Template Pattern) */
void dead_code_elimination() {
    printf("\n--- OPTIMIZATION: DEAD CODE ELIMINATION ---\n");
    int active_labels[200] = {0};
    
    /* Mark labels that are targets of jumps */
    for(int i=0; i<q_idx; i++) {
        if (strcmp(quads[i].op, "IF_GOTO") == 0 || strcmp(quads[i].op, "GOTO") == 0) {
            int l_num = atoi(quads[i].result + 1);
            if (l_num > 0) active_labels[l_num] = 1;
        }
    }
    
    /* Simple Elimination: Remove labels that are never jumped to */
    int new_q_idx = 0;
    for(int i=0; i<q_idx; i++) {
        if (strcmp(quads[i].op, "LABEL") == 0) {
            int l_num = atoi(quads[i].result + 1);
            if (l_num > 0 && active_labels[l_num] == 0) {
                printf("[DCE] Removing unreachable label: %s\n", quads[i].result);
                continue;
            }
        }
        quads[new_q_idx++] = quads[i];
    }
    q_idx = new_q_idx;
    printf("✔ Dead code elimination complete.\n");
}

int main(int argc, char** argv) {
    int dce_flag = (argc > 1 && strcmp(argv[1], "dead") == 0);
    opt_enabled = dce_flag;
    
    yyin = fopen("main.c", "r");
    if (!yyin) return 1;

    if (yyparse() == 0) {
        printf("\n--- COMPILER PHASES (DCE: %s) ---\n", dce_flag ? "ENABLED" : "DISABLED");
        
        generate_icg(root);
        
        if (dce_flag) {
            dead_code_elimination();
        }

        print_icg();
        generate_target();
    }

    fclose(yyin);
    return 0;
}
