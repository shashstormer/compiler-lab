#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAX_PROD 20
#define MAX_LEN 20
#define MAX_NT 10
#define MAX_T 20

int count;
char production[MAX_PROD][MAX_LEN];

char nonterminals[MAX_NT];
int nt_count = 0;
char terminals[MAX_T];
int t_count = 0;

char first_res[MAX_NT][MAX_T];
int first_count[MAX_NT];
char follow_res[MAX_NT][MAX_T];
int follow_count[MAX_NT];

char table[MAX_NT][MAX_T][MAX_LEN];

int is_nonterminal(char c) { return isupper(c); }

int nt_index(char c) {
    int i;
    for(i = 0; i < nt_count; i++)
        if(nonterminals[i] == c) return i;
    return -1;
}

int t_index(char c) {
    int i;
    for(i = 0; i < t_count; i++)
        if(terminals[i] == c) return i;
    return -1;
}

int in_set(char *set, int sz, char c) {
    int i;
    for(i = 0; i < sz; i++)
        if(set[i] == c) return 1;
    return 0;
}

void add_to_set(char *set, int *sz, char c) {
    if(!in_set(set, *sz, c))
        set[(*sz)++] = c;
}

void add_terminal(char c) {
    if(c != '#' && !is_nonterminal(c) && t_index(c) == -1)
        terminals[t_count++] = c;
}

void compute_first(char c, char *result, int *res_count) {
    int i;
    if(!is_nonterminal(c)) {
        add_to_set(result, res_count, c);
        return;
    }
    for(i = 0; i < count; i++) {
        if(production[i][0] == c) {
            int j = 2;
            if(production[i][j] == '#') {
                add_to_set(result, res_count, '#');
                continue;
            }
            while(production[i][j] != '\0') {
                char sym = production[i][j];
                if(!is_nonterminal(sym)) {
                    add_to_set(result, res_count, sym);
                    break;
                } else {
                    char temp[MAX_T];
                    int tc = 0;
                    compute_first(sym, temp, &tc);
                    int k;
                    for(k = 0; k < tc; k++)
                        if(temp[k] != '#')
                            add_to_set(result, res_count, temp[k]);
                    if(!in_set(temp, tc, '#'))
                        break;
                    j++;
                    if(production[i][j] == '\0')
                        add_to_set(result, res_count, '#');
                }
            }
        }
    }
}

void compute_first_of_string(char *str, char *result, int *res_count) {
    int i = 0;
    while(str[i] != '\0') {
        char temp[MAX_T];
        int tc = 0;
        compute_first(str[i], temp, &tc);
        int k;
        for(k = 0; k < tc; k++)
            if(temp[k] != '#')
                add_to_set(result, res_count, temp[k]);
        if(!in_set(temp, tc, '#'))
            return;
        i++;
    }
    add_to_set(result, res_count, '#');
}

void compute_follow(char c) {
    int idx = nt_index(c);
    if(idx < 0) return;
    if(production[0][0] == c)
        add_to_set(follow_res[idx], &follow_count[idx], '$');
    int i, j;
    for(i = 0; i < count; i++) {
        for(j = 2; production[i][j] != '\0'; j++) {
            if(production[i][j] == c) {
                if(production[i][j+1] != '\0') {
                    char beta[MAX_LEN];
                    strcpy(beta, &production[i][j+1]);
                    char temp[MAX_T];
                    int tc = 0;
                    compute_first_of_string(beta, temp, &tc);
                    int m;
                    for(m = 0; m < tc; m++)
                        if(temp[m] != '#')
                            add_to_set(follow_res[idx], &follow_count[idx], temp[m]);
                    if(in_set(temp, tc, '#') && production[i][0] != c) {
                        int pidx = nt_index(production[i][0]);
                        if(pidx >= 0) {
                            compute_follow(production[i][0]);
                            for(m = 0; m < follow_count[pidx]; m++)
                                add_to_set(follow_res[idx], &follow_count[idx], follow_res[pidx][m]);
                        }
                    }
                } else {
                    if(production[i][0] != c) {
                        int pidx = nt_index(production[i][0]);
                        if(pidx >= 0) {
                            compute_follow(production[i][0]);
                            int m;
                            for(m = 0; m < follow_count[pidx]; m++)
                                add_to_set(follow_res[idx], &follow_count[idx], follow_res[pidx][m]);
                        }
                    }
                }
            }
        }
    }
}

void build_table() {
    int i;
    for(i = 0; i < count; i++) {
        char lhs = production[i][0];
        int ni = nt_index(lhs);
        char *rhs = &production[i][2];

        char first_rhs[MAX_T];
        int fc = 0;
        compute_first_of_string(rhs, first_rhs, &fc);

        int k;
        for(k = 0; k < fc; k++) {
            if(first_rhs[k] != '#') {
                int ti = t_index(first_rhs[k]);
                if(ti >= 0) {
                    sprintf(table[ni][ti], "%c->%s", lhs, rhs);
                }
            }
        }

        if(in_set(first_rhs, fc, '#')) {
            int fi;
            for(fi = 0; fi < follow_count[ni]; fi++) {
                char fc_sym = follow_res[ni][fi];
                int ti = t_index(fc_sym);
                if(ti >= 0 && table[ni][ti][0] == '\0') {
                    sprintf(table[ni][ti], "%c->%s", lhs, rhs);
                }
                if(fc_sym == '$') {
                    int di = t_index('$');
                    if(di >= 0 && table[ni][di][0] == '\0')
                        sprintf(table[ni][di], "%c->%s", lhs, rhs);
                }
            }
        }
    }
}

int main() {
    int i;
    char ch;

    printf("Enter number of productions:\n");
    scanf("%d", &count);
    for(i = 0; i < count; i++) {
        scanf("%s%c", production[i], &ch);
        if(nt_index(production[i][0]) == -1)
            nonterminals[nt_count++] = production[i][0];
        int j;
        for(j = 2; production[i][j] != '\0'; j++) {
            if(!is_nonterminal(production[i][j]) && production[i][j] != '#')
                add_terminal(production[i][j]);
        }
    }
    add_terminal('$');

    for(i = 0; i < nt_count; i++) {
        first_count[i] = 0;
        follow_count[i] = 0;
    }

    for(i = 0; i < nt_count; i++)
        compute_first(nonterminals[i], first_res[i], &first_count[i]);

    printf("\n");
    for(i = 0; i < nt_count; i++) {
        printf("FIRST(%c) = { ", nonterminals[i]);
        int j;
        for(j = 0; j < first_count[i]; j++)
            printf("%c ", first_res[i][j]);
        printf("}\n");
    }

    for(i = 0; i < nt_count; i++)
        compute_follow(nonterminals[i]);

    printf("\n");
    for(i = 0; i < nt_count; i++) {
        printf("FOLLOW(%c) = { ", nonterminals[i]);
        int j;
        for(j = 0; j < follow_count[i]; j++)
            printf("%c ", follow_res[i][j]);
        printf("}\n");
    }

    memset(table, 0, sizeof(table));
    build_table();

    printf("\n\nPREDICTIVE PARSING TABLE\n");
    printf("%-12s", "Non-Term");
    for(i = 0; i < t_count; i++)
        printf("%-15c", terminals[i]);
    printf("\n");

    int row;
    for(row = 0; row < nt_count; row++) {
        printf("%-12c", nonterminals[row]);
        int col;
        for(col = 0; col < t_count; col++) {
            if(table[row][col][0] != '\0')
                printf("%-15s", table[row][col]);
            else
                printf("%-15s", "");
        }
        printf("\n");
    }

    return 0;
}
