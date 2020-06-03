/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#ifndef _btree_c
#define _btree_c

#define SIZE 16
#define ORDER 5
#define TRUE 1
#define FALSE 0
#define PAGESIZE 4096

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
    bool is_leaf;
    int key_count;
    long *children;
    Index *keys;
}Node;



/*Functions*/
Node *createNode(bool);

Register *createRegister();

int writeRegisterOnFile(Register*);

void addRegister();

Node *getRoot(FILE*);


void writePageOnFile(FILE*,Node*,long);

Node *readPageFile(FILE*);

#endif