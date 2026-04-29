#ifndef NODE_H
#define NODE_H

typedef struct Node {
    char* name;
    struct Node *left, *right;
} Node;

Node* createNode(char* name, Node* left, Node* right);
void printTree(Node* node, int indent);

#endif
