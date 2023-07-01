#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *GetVal(Node_t node, Flexion_t *positions, uint8_t isNum) {
    int i, j;
    Node_t curNode = node;
    for (i = 0; i < 6; i++) {
        // Returns a letter based on whether the button is pressed
        if (curNode.isNum && isNum)
            return curNode.val;
        if (curNode.isChar && !isNum)
            return curNode.letter;

        Node_t *tempNode = &curNode;
        for (j = 0; j < curNode.numChildren; j++) {
            if (curNode.children[j]->flex == positions[i]) {
                tempNode = curNode.children[j];
                break;
            }
        }
        // Check for unhandled flex
        if (tempNode == &curNode)
            return 0;
        else
            curNode = *tempNode;
    }
    return 0;
}

uint8_t AddChild(Node_t *parent, Node_t *child) {
    if (parent == NULL || child == NULL)
        return 0;
    parent->numChildren++;
    /* printf("%d\r\n", parent->numChildren); */
    // Resize the array of children. Hope this doesn't cause issues down the
    // line.
    Node_t **temp =
        realloc(parent->children, sizeof(Node_t *) * parent->numChildren);
    if (temp == NULL)
        return 0;
    else
        parent->children = temp;
    parent->children[parent->numChildren - 1] = child;
    return 1;
}

uint8_t SetValue(Node_t *node, char *val) {
    if (node == NULL)
        return 0;
    node->isNum = 1;
    node->val = val;
    return 1;
}

uint8_t SetLetter(Node_t *node, char *letter) {
    if (node == NULL)
        return 0;
    node->isChar = 1;
    node->letter = letter;
    return 1;
}

uint8_t SetFlex(Node_t *node, Flexion_t flex) {
    if (node == NULL)
        return 0;
    node->flex = flex;
    return 1;
}

Node_t DefaultInit() {
    Node_t node;
    node.flex = STRAIGHT;
    node.numChildren = 0;
    node.val = 0;
    node.isNum = 0;
    node.isChar = 0;
    node.letter = 0;
    // Allocate just one child, this will be resized when more children are
    // added.
    node.children = malloc(sizeof(Node_t *));
    memset(node.children, 0, sizeof(Node_t *));
    return node;
}

// May not be necessary
uint8_t DeleteChildren(Node_t *node) {
    node->numChildren = 0;
    free(node->children);
    return 1;
}

void CreateTree(Node_t *dummy) {
    Node_t t_f = DefaultInit();
    Node_t t_s = DefaultInit();
    AddChild(dummy, &t_f);
    AddChild(dummy, &t_s);
    SetFlex(&t_f, FLEXED);
    SetFlex(&t_s, STRAIGHT);

    Node_t i_ff = DefaultInit();
    Node_t i_fs = DefaultInit();
    AddChild(&t_f, &i_ff);
    AddChild(&t_f, &i_fs);
    SetFlex(&i_ff, FLEXED);
    SetFlex(&i_fs, STRAIGHT);

    Node_t i_sf = DefaultInit();
    Node_t i_ss = DefaultInit();
    AddChild(&t_s, &i_sf);
    AddChild(&t_s, &i_ss);
    SetValue(&i_sf, "10");
    SetFlex(&i_sf, FLEXED);
    SetFlex(&i_ss, STRAIGHT);

    Node_t m_fsf = DefaultInit();
    Node_t m_fss = DefaultInit();
    Node_t m_ssf = DefaultInit();
    Node_t m_sff = DefaultInit();
    Node_t m_ffs = DefaultInit();
    AddChild(&i_fs, &m_fsf);
    AddChild(&i_fs, &m_fss);
    AddChild(&i_ss, &m_ssf);
    AddChild(&i_sf, &m_sff);
    AddChild(&i_ff, &m_ffs);
    SetFlex(&m_fsf, FLEXED);
    SetFlex(&m_fss, STRAIGHT);
    SetFlex(&m_ssf, FLEXED);
    SetFlex(&m_sff, FLEXED);
    SetFlex(&m_ffs, STRAIGHT);
    SetLetter(&m_ffs, "F");
    SetValue(&m_ffs, "9");
    SetLetter(&m_sff, "Y");
    SetLetter(&m_ssf, "G");

    Node_t m_sss = DefaultInit();
    AddChild(&i_ss, &m_sss);
    SetFlex(&m_sss, STRAIGHT);

    Node_t m_fff = DefaultInit();
    AddChild(&i_ff, &m_fff);
    SetFlex(&m_fff, FLEXED);

    Node_t r_fsff = DefaultInit();
    Node_t r_ffff = DefaultInit();
    Node_t r_fsfs = DefaultInit();
    AddChild(&m_fsf, &r_fsff);
    AddChild(&m_fsf, &r_fsfs);
    AddChild(&m_fff, &r_ffff);
    SetFlex(&r_fsff, FLEXED);
    SetFlex(&r_fsfs, STRAIGHT);
    SetFlex(&r_ffff, FLEXED);
    SetValue(&r_fsff, "1");
    SetLetter(&r_fsff, "D");
    SetValue(&r_fsfs, "8");

    Node_t r_fssf = DefaultInit();
    Node_t r_fsss = DefaultInit();
    AddChild(&m_fss, &r_fssf);
    AddChild(&m_fss, &r_fsss);
    SetFlex(&r_fssf, FLEXED);
    SetFlex(&r_fsss, STRAIGHT);

    Node_t r_sssf = DefaultInit();
    Node_t r_ssss = DefaultInit();
    AddChild(&m_sss, &r_sssf);
    AddChild(&m_sss, &r_ssss);
    SetFlex(&r_sssf, FLEXED);
    SetFlex(&r_ssss, STRAIGHT);
    SetValue(&r_sssf, "3");
    SetLetter(&r_sssf, "H");
    SetValue(&r_ssss, "5");

    Node_t p_fssff = DefaultInit();
    Node_t p_fssfs = DefaultInit();
    AddChild(&r_fssf, &p_fssff);
    AddChild(&r_fssf, &p_fssfs);
    SetFlex(&p_fssff, FLEXED);
    SetFlex(&p_fssfs, STRAIGHT);
    SetValue(&p_fssff, "2");
    SetLetter(&p_fssff, "R");
    SetValue(&p_fssfs, "7");

    Node_t p_fsssf = DefaultInit();
    Node_t p_fssss = DefaultInit();
    Node_t p_fffff = DefaultInit();
    Node_t p_ffffs = DefaultInit();
    AddChild(&r_fsss, &p_fsssf);
    AddChild(&r_fsss, &p_fssss);
    AddChild(&r_ffff, &p_fffff);
    AddChild(&r_ffff, &p_ffffs);
    SetFlex(&p_fsssf, FLEXED);
    SetFlex(&p_fssss, STRAIGHT);
    SetFlex(&p_fffff, FLEXED);
    SetFlex(&p_ffffs, STRAIGHT);
    SetLetter(&p_fffff, "O");
    SetValue(&p_fffff, "0");
    SetValue(&p_fsssf, "6");
    SetLetter(&p_fsssf, "W");
    SetValue(&p_fssss, "4");
    SetLetter(&p_fssss, "B");
    SetLetter(&p_ffffs, "I");
}
