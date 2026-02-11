#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int n_nfa_states, n_inputs, n_nfa_final;
int nfa_final_states[20];
int nfa_table[20][10][20];
int nfa_table_count[20][10];

int dfa_states[20][20];
int dfa_state_count = 0;
int dfa_table[20][10];
int dfa_final[20];

void read_nfa() {
    int i, j, k, n_trans, target;
    printf("Enter number of NFA states: ");
    scanf("%d", &n_nfa_states);
    printf("Enter number of input symbols: ");
    scanf("%d", &n_inputs);
    printf("Enter number of NFA final states: ");
    scanf("%d", &n_nfa_final);
    printf("Enter NFA final states:\n");
    for(i=0; i<n_nfa_final; i++) {
        scanf("%d", &nfa_final_states[i]);
    }
    printf("Enter NFA transition table:\n");
    for(i=0; i<n_nfa_states; i++) {
        for(j=0; j<n_inputs; j++) {
            printf("From state %d on symbol %d, number of transitions: ", i, j);
            scanf("%d", &n_trans);
            nfa_table_count[i][j] = n_trans;
            for(k=0; k<n_trans; k++) {
                scanf("%d", &target);
                nfa_table[i][j][k] = target;
            }
        }
    }
}

int find_dfa_state(int state_set[], int count) {
    int i, j, match;
    for(i=0; i<dfa_state_count; i++) {
        int temp_count = 0;
        for(j=0; j<20; j++) {
            if(dfa_states[i][j] != -1) temp_count++;
        }
        if(temp_count != count) continue;
        
        match = 1;
        for(j=0; j<count; j++) {
            int found = 0;
            for(int l=0; l<20; l++) {
                if(dfa_states[i][l] == state_set[j]) {
                    found = 1;
                    break;
                }
            }
            if(!found) {
                match = 0;
                break;
            }
        }
        if(match) return i;
    }
    return -1;
}

void add_dfa_state(int state_set[], int count) {
    int i;
    for(i=0; i<20; i++) dfa_states[dfa_state_count][i] = -1;
    for(i=0; i<count; i++) dfa_states[dfa_state_count][i] = state_set[i];
    
    int is_final = 0;
    for(i=0; i<count; i++) {
        for(int k=0; k<n_nfa_final; k++) {
            if(state_set[i] == nfa_final_states[k]) {
                is_final = 1;
                break;
            }
        }
    }
    dfa_final[dfa_state_count] = is_final;
    dfa_state_count++;
}

void sort_set(int set[], int n) {
    int i, j, temp;
    for(i=0; i<n-1; i++) {
        for(j=0; j<n-i-1; j++) {
            if(set[j] > set[j+1]) {
                temp = set[j];
                set[j] = set[j+1];
                set[j+1] = temp;
            }
        }
    }
}

void subset_construction() {
    int start_set[1] = {0};
    add_dfa_state(start_set, 1);
    
    int current_state = 0;
    while(current_state < dfa_state_count) {
        for(int input=0; input<n_inputs; input++) {
            int next_set[20], next_count = 0;
            int visited[20] = {0};
            
            for(int i=0; i<20; i++) {
                int nfa_s = dfa_states[current_state][i];
                if(nfa_s == -1) break;
                
                int trans_count = nfa_table_count[nfa_s][input];
                for(int k=0; k<trans_count; k++) {
                    int target = nfa_table[nfa_s][input][k];
                    if(!visited[target]) {
                        next_set[next_count++] = target;
                        visited[target] = 1;
                    }
                }
            }
            
            sort_set(next_set, next_count);
            
            int dfa_idx = find_dfa_state(next_set, next_count);
            if(dfa_idx == -1) {
                add_dfa_state(next_set, next_count);
                dfa_idx = dfa_state_count - 1;
            }
            dfa_table[current_state][input] = dfa_idx;
        }
        current_state++;
    }
}

void print_dfa() {
    printf("\nDFA Transition Table:\n");
    for(int i=0; i<dfa_state_count; i++) {
        printf("State %d: ", i);
        for(int j=0; j<n_inputs; j++) {
            printf("%d ", dfa_table[i][j]);
        }
        if(dfa_final[i]) printf("(Final)");
        printf("\n");
    }
}

int main() {
    read_nfa();
    subset_construction();
    print_dfa();
    return 0;
}
