/*
 * Ex. 10: Computation of LR(0) Items
 * Computes canonical collection of LR(0) item sets.
 * Includes: Augmented Grammar, Closure, Goto functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 20
#define MAX_LEN 50
#define MAX_ITEMS 100
#define MAX_STATES 50

// Production: lhs -> rhs
typedef struct {
    char lhs;
    char rhs[MAX_LEN];
} Production;

// An LR(0) item: production index + dot position
typedef struct {
    int prod_idx;  // Index into productions array
    int dot_pos;   // Position of dot in RHS
} Item;

// An item set (state)
typedef struct {
    Item items[MAX_ITEMS];
    int count;
} ItemSet;

Production prods[MAX_PROD];
int num_prods;

ItemSet states[MAX_STATES];
int num_states = 0;

// Goto transitions: goto_table[state][symbol] = target state (-1 if none)
int goto_table[MAX_STATES][128];

// Check if two items are equal
int items_equal(Item a, Item b) {
    return a.prod_idx == b.prod_idx && a.dot_pos == b.dot_pos;
}

// Check if item already exists in item set
int item_in_set(ItemSet *set, Item item) {
    for (int i = 0; i < set->count; i++) {
        if (items_equal(set->items[i], item))
            return 1;
    }
    return 0;
}

// Add item to set if not already present
int add_item(ItemSet *set, Item item) {
    if (!item_in_set(set, item)) {
        set->items[set->count++] = item;
        return 1;
    }
    return 0;
}

// Check if two item sets are equal
int sets_equal(ItemSet *a, ItemSet *b) {
    if (a->count != b->count) return 0;
    for (int i = 0; i < a->count; i++) {
        if (!item_in_set(b, a->items[i]))
            return 0;
    }
    return 1;
}

// Compute closure of an item set
void closure(ItemSet *set) {
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < set->count; i++) {
            int pi = set->items[i].prod_idx;
            int dp = set->items[i].dot_pos;
            char *rhs = prods[pi].rhs;

            // If dot is before a non-terminal
            if (dp < (int)strlen(rhs) && isupper(rhs[dp])) {
                char B = rhs[dp];
                // Add all productions of B with dot at start
                for (int j = 0; j < num_prods; j++) {
                    if (prods[j].lhs == B) {
                        Item new_item = {j, 0};
                        if (add_item(set, new_item))
                            changed = 1;
                    }
                }
            }
        }
    }
}

// Compute goto(I, X)
ItemSet compute_goto(ItemSet *I, char X) {
    ItemSet result;
    result.count = 0;

    for (int i = 0; i < I->count; i++) {
        int pi = I->items[i].prod_idx;
        int dp = I->items[i].dot_pos;
        char *rhs = prods[pi].rhs;

        // If dot is before symbol X
        if (dp < (int)strlen(rhs) && rhs[dp] == X) {
            Item new_item = {pi, dp + 1};
            add_item(&result, new_item);
        }
    }

    if (result.count > 0) {
        closure(&result);
    }

    return result;
}

// Find if an item set already exists in states; return index or -1
int find_state(ItemSet *set) {
    for (int i = 0; i < num_states; i++) {
        if (sets_equal(&states[i], set))
            return i;
    }
    return -1;
}

// Collect all grammar symbols used
void get_symbols(char *symbols, int *count) {
    *count = 0;
    int used[128] = {0};

    for (int i = 0; i < num_prods; i++) {
        if (!used[(int)prods[i].lhs]) {
            used[(int)prods[i].lhs] = 1;
            symbols[(*count)++] = prods[i].lhs;
        }
        for (int j = 0; j < (int)strlen(prods[i].rhs); j++) {
            char c = prods[i].rhs[j];
            if (!used[(int)c]) {
                used[(int)c] = 1;
                symbols[(*count)++] = c;
            }
        }
    }
    symbols[*count] = '\0';
}

// Print an item
void print_item(Item item) {
    Production p = prods[item.prod_idx];
    printf("    %c -> ", p.lhs);
    for (int i = 0; i <= (int)strlen(p.rhs); i++) {
        if (i == item.dot_pos) printf(".");
        if (i < (int)strlen(p.rhs)) printf("%c", p.rhs[i]);
    }
    printf("\n");
}

// Print an item set
void print_item_set(int idx) {
    printf("\nI%d:\n", idx);
    for (int i = 0; i < states[idx].count; i++) {
        print_item(states[idx].items[i]);
    }
}

int main() {
    int n;
    char original_start;

    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions:\n");
    // First production's LHS is the start symbol
    // Reserve index 0 for augmented production
    for (int i = 1; i <= n; i++) {
        char prod[MAX_LEN];
        scanf("%s", prod);
        prods[i].lhs = prod[0];
        // Skip "->" (3 characters)
        strcpy(prods[i].rhs, prod + 3);
    }

    original_start = prods[1].lhs;
    num_prods = n + 1;

    // Augment grammar: add S' -> S (use '\'' won't work, use a special char)
    // We'll use the character after the start symbol or 'Z' as augmented start
    // For display purposes, we track augmented start separately
    char aug_start = '\'';  // Placeholder — we print "S'" specially
    prods[0].lhs = aug_start;
    prods[0].rhs[0] = original_start;
    prods[0].rhs[1] = '\0';

    // Print numbered productions
    printf("\n1. Numbering the productions:\n");
    for (int i = 1; i < num_prods; i++) {
        printf("   %d. %c -> %s\n", i, prods[i].lhs, prods[i].rhs);
    }

    // Print augmented grammar
    printf("\n2. Augmented Grammar:\n");
    printf("   %c' -> %c\n", original_start, original_start);
    for (int i = 1; i < num_prods; i++) {
        printf("   %c -> %s\n", prods[i].lhs, prods[i].rhs);
    }

    // Initialize goto table
    memset(goto_table, -1, sizeof(goto_table));

    // Compute I0: closure({S' -> .S})
    states[0].count = 0;
    Item start_item = {0, 0};  // S' -> .S
    add_item(&states[0], start_item);
    closure(&states[0]);
    num_states = 1;

    // Get all grammar symbols
    char symbols[128];
    int sym_count;
    get_symbols(symbols, &sym_count);

    // Compute canonical collection
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < num_states; i++) {
            for (int j = 0; j < sym_count; j++) {
                char X = symbols[j];
                ItemSet g = compute_goto(&states[i], X);

                if (g.count > 0) {
                    int existing = find_state(&g);
                    if (existing == -1) {
                        // New state
                        states[num_states] = g;
                        goto_table[i][(int)X] = num_states;
                        num_states++;
                        changed = 1;
                    } else {
                        goto_table[i][(int)X] = existing;
                    }
                }
            }
        }
    }

    // Print LR(0) Items
    printf("\n3. LR(0) Items\n");

    for (int i = 0; i < num_states; i++) {
        // Print item set
        printf("\nI%d:", i);
        for (int j = 0; j < states[i].count; j++) {
            Production p = prods[states[i].items[j].prod_idx];
            int dp = states[i].items[j].dot_pos;

            if (p.lhs == aug_start)
                printf("\n    %c' -> ", original_start);
            else
                printf("\n    %c -> ", p.lhs);

            for (int k = 0; k <= (int)strlen(p.rhs); k++) {
                if (k == dp) printf(".");
                if (k < (int)strlen(p.rhs)) printf("%c", p.rhs[k]);
            }
        }
        printf("\n");
    }

    // Print Goto transitions
    printf("\nGoto Transitions:\n");
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < sym_count; j++) {
            char X = symbols[j];
            if (goto_table[i][(int)X] != -1) {
                if (X == aug_start)
                    printf("  Goto(I%d, %c') = I%d\n", i, original_start, goto_table[i][(int)X]);
                else
                    printf("  Goto(I%d, %c) = I%d\n", i, X, goto_table[i][(int)X]);
            }
        }
    }

    return 0;
}
