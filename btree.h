/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#ifndef _btree_c
#define _btree_c

#define SIZE 16
#define ORDER 171 
#define TRUE 1
#define FALSE 0
#define PAGESIZE 4096
#define SUCCESS 1
#define THRASHSIZE PAGESIZE - ((ORDER-1)*sizeof(Index) + ORDER*sizeof(long) + 8)

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
    long RNN; /* 8 */
    int prim_key; /* 8 */
    
}Index;

typedef struct _node{
    int is_leaf; /*4*/
    int key_count; /* 4*/
    long children[ORDER]; /* 8 */
    Index keys[ORDER-1]; /* 12 */
    char thrash[THRASHSIZE];
}Node;



/*Functions*/
Node *createNode(bool);

Register *createRegister();

Index *writeRegisterOnFile(Register*);

void addRegister();

long getRootRRN();

Node *getRoot();

int _writePageOnFile(Node *, long);

/*void writePageOnFile(Node*, long);*/

void getRegister(long RRN);

Node *readPageFile(FILE*);

int freeSpaceOnPage();

Node *getPageFromFile(FILE*, long);

int addIndexToTree(Index*);

long insertOnNode(Node *, Index *, long);

void splitNode(Node *, int, Node *, long);

Node* splitRoot(Node*);

long _bTreeSearch(FILE*, Node*, int);

void bTreeSearch();

void menu();

#endif