#ifndef NODE_H
#define NODE_H
#include <stdint.h>

#define MAX_CHILDREN 2

typedef enum Flexion { UNINIT, FLEXED, STRAIGHT } Flexion_t;
typedef struct Node {
    uint8_t numChildren;
    // May be good to initialize this to a constant value to avoid having to use
    // malloc or something?
    struct Node **children;
    uint8_t isNum;
    Flexion_t flex;
    uint8_t isChar;
    char *val;
    char *letter;
} Node_t;

/*
 * Traverses through a tree to get a letter or number from a set of flexed
 * fingers
 */
char *GetVal(Node_t node, Flexion_t *positions, uint8_t isNum);

/*
 * Creates the hardcoded tree for our alphabet and numbers
 */
void CreateTree(Node_t *dummy);

/*
 * Adds a child to a parent node. Allocates necessary memory for a pointer to
 * the child, but leaves the child in its memory location
 */
uint8_t AddChild(Node_t *parent, Node_t *child);

/*
 * Sets the number value of a node and updates it to be a number node.
 */
uint8_t SetValue(Node_t *node, char *val);

/*
 * Sets the letter value of a node and updates it to be letter node
 */
uint8_t SetLetter(Node_t *node, char *letter);

/*
 * Sets the flex state of a node
 */
uint8_t SetFlex(Node_t *node, Flexion_t flex);

/*
 * Creates a node with default initialization state: Straight finger, memory for
 * one child, no character value, etc. Allocates the space for one child pointer
 * and sets that memory to 0.
 */
Node_t DefaultInit();

/*
 * Frees all memory in a node's array of children. Not actually necessary when
 * running the code but potentially modifiable to delete individual children?
 */
uint8_t DeleteChildren(Node_t *node);

#endif
