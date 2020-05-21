/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#ifndef _btree_c
#define _btree_c

#define SIZE 16
#define ORDER 4

#include <stdbool.h>

/*Structs*/
typedef struct _register{
    int numUSP;
    char name[SIZE]; 
    char surname[SIZE]; 
    char course[SIZE];
    float grade;
}Register;

typedef struct _index{
    int prim_key;
    long RNN;
}Index;

typedef struct _node{
    struct _node *children[ORDER];
    Index *keys[ORDER-1];
    int key_count;
    bool is_leaf;
}Node;

typedef struct _tree{
    int order;
    Node **root;
}Tree;


/*Functions*/

#endif