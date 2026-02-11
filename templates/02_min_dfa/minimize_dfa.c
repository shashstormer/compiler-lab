#include <stdio.h>

int n_states, n_inputs, n_final;
int final_states[20];
int dfa_table[20][10];
int partition[20]; 
int new_partition[20];
int n_partitions;

int is_final(int state) {
    for(int i=0; i<n_final; i++) {
        if(final_states[i] == state) return 1;
    }
    return 0;
}

void read_dfa() {
    int i, j;
    printf("Enter number of DFA states: ");
    scanf("%d", &n_states);
    printf("Enter number of input symbols: ");
    scanf("%d", &n_inputs);
    printf("Enter number of final states: ");
    scanf("%d", &n_final);
    printf("Enter final states:\n");
    for(i=0; i<n_final; i++) {
        scanf("%d", &final_states[i]);
    }
    printf("Enter DFA transition table:\n");
    for(i=0; i<n_states; i++) {
        for(j=0; j<n_inputs; j++) {
            scanf("%d", &dfa_table[i][j]);
        }
    }
}

void minimize() {
    int i, j;

    for(i=0; i<n_states; i++) {
        if(is_final(i)) partition[i] = 1;
        else partition[i] = 0;
    }

    int change = 1;
    while(change) {
        change = 0;
        
        for(i=0; i<n_states; i++) {
            new_partition[i] = partition[i];
        }

        for(i=0; i<n_states; i++) {
            for(j=i+1; j<n_states; j++) {
                if(partition[i] == partition[j]) {
                    int inputs_match = 1;
                    for(int k=0; k<n_inputs; k++) {
                        int t1 = dfa_table[i][k];
                        int t2 = dfa_table[j][k];
                        if(partition[t1] != partition[t2]) {
                            inputs_match = 0;
                            break;
                        }
                    }
                    if(!inputs_match) {
                        new_partition[j]++; 
                        change = 1;
                    }
                }
            }
        }
        
        for(i=0; i<n_states; i++) partition[i] = new_partition[i];
    }
    
    int mapped[20];
    int p_count = 0;
    for(i=0; i<20; i++) mapped[i] = -1;
    
    for(i=0; i<n_states; i++) {
        int p = partition[i];
        int found = -1;
        for(j=0; j<p_count; j++) {
            if(mapped[j] == p) {
                found = j;
                break;
            }
        }
        if(found == -1) {
            mapped[p_count] = p;
            partition[i] = p_count;
            p_count++;
        } else {
            partition[i] = found;
        }
    }
    n_partitions = p_count;
}

void print_minimized() {
    int i, j;
    printf("Minimized DFA State Groups:\n");
    for(i=0; i<n_states; i++) {
        printf("State %d -> Group %d\n", i, partition[i]);
    }
    
    printf("Minimized DFA Transition Table:\n");
    for(i=0; i<n_partitions; i++) {
        printf("Group %d: ", i);
        int rep = -1;
        for(j=0; j<n_states; j++) {
            if(partition[j] == i) {
                rep = j;
                break;
            }
        }
        for(int k=0; k<n_inputs; k++) {
            int target = dfa_table[rep][k];
            printf("%d ", partition[target]);
        }
        printf("\n");
    }
}

int main() {
    read_dfa();
    minimize();
    print_minimized();
    return 0;
}
