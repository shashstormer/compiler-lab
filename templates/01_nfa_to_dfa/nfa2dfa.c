#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_ALPHABET 5

int inputs;
int nfa_states;
int nfa_transitions[MAX_STATES][MAX_ALPHABET][MAX_STATES];

int dfa_states_count = 0;
int dfa_transitions[MAX_STATES][MAX_ALPHABET];
int dfa_states_sets[MAX_STATES][MAX_STATES];
int dfa_final_states[MAX_STATES];

void print_nfa() {
    printf("NFA Transitions:\n");
}

int main() {
    printf("NFA to DFA Conversion\n");
    printf("Enter number of NFA states: ");
    scanf("%d", &nfa_states);
    
    printf("Enter number of input symbols: ");
    scanf("%d", &inputs);
    
    printf("\n(Placeholder) DFA Table Constructed.\n");
    return 0;
}
