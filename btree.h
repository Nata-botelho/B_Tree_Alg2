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
    int prim_key; /*  4 */
    long RNN; /* 8 */
}Index;

typedef struct _node{
    bool is_leaf; /*1*/
    int key_count; /* 4*/
    long *children; /* 8 */
    Index *keys; /* 12 */
}Node;



/*Functions*/
Node *createNode(bool);

Register *createRegister();

Index *writeRegisterOnFile(Register*);

void addRegister();

Node *getRoot(FILE*);

void writePageOnFile(FILE*,Node*,long);

Node *readPageFile(FILE*);

int freeSpaceOnPage();

Node *getPageOnFile(FILE*, long);

void addIndexToTree(FILE* , Node*,Index *);

long bTreeSeach(FILE*,Node*,int );

#endif