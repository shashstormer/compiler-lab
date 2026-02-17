#include<stdio.h>
#include<ctype.h>
#include<string.h>

int count;
char production[10][10];
char first_res[10][20];
int first_count[10];
char follow_res[10][20];
int follow_count[10];
char nonterminals[10];
int nt_count = 0;

int is_nonterminal(char c) {
    return isupper(c);
}

int nt_index(char c) {
    int i;
    for(i = 0; i < nt_count; i++)
        if(nonterminals[i] == c) return i;
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

void compute_first(char c, char *result, int *res_count) {
    int i;
    if(!is_nonterminal(c)) {
        add_to_set(result, res_count, c);
        return;
    }
    for(i = 0; i < count; i++) {
        if(production[i][0] == c) {
            int rhs_start = 2;
            int j = rhs_start;
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
                    char temp[20];
                    int temp_count = 0;
                    compute_first(sym, temp, &temp_count);
                    int k;
                    for(k = 0; k < temp_count; k++)
                        if(temp[k] != '#')
                            add_to_set(result, res_count, temp[k]);
                    if(!in_set(temp, temp_count, '#'))
                        break;
                    j++;
                    if(production[i][j] == '\0')
                        add_to_set(result, res_count, '#');
                }
            }
        }
    }
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
                    int k = j + 1;
                    while(production[i][k] != '\0') {
                        char next = production[i][k];
                        if(!is_nonterminal(next)) {
                            add_to_set(follow_res[idx], &follow_count[idx], next);
                            break;
                        } else {
                            char temp[20];
                            int temp_count = 0;
                            compute_first(next, temp, &temp_count);
                            int m;
                            for(m = 0; m < temp_count; m++)
                                if(temp[m] != '#')
                                    add_to_set(follow_res[idx], &follow_count[idx], temp[m]);
                            if(!in_set(temp, temp_count, '#'))
                                break;
                            k++;
                            if(production[i][k] == '\0') {
                                int pidx = nt_index(production[i][0]);
                                if(pidx >= 0 && production[i][0] != c) {
                                    compute_follow(production[i][0]);
                                    int m2;
                                    for(m2 = 0; m2 < follow_count[pidx]; m2++)
                                        add_to_set(follow_res[idx], &follow_count[idx], follow_res[pidx][m2]);
                                }
                            }
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

int main() {
    int i;
    char ch;
    printf("Enter number of productions:\n");
    scanf("%d", &count);
    for(i = 0; i < count; i++) {
        scanf("%s%c", production[i], &ch);
        if(nt_index(production[i][0]) == -1)
            nonterminals[nt_count++] = production[i][0];
    }

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

    return 0;
}
