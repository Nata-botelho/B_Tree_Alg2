#ifndef _btree_c
#define _btree_c

#define TAM 16



struct aluno{
    int numUSP;
    char nome[TAM]; // 16
    char sobrenome[TAM]; // 16
    char curso[TAM]; // 16
    float nota;
};

struct index{
    int key;
    long RNN;
};



typedef struct aluno Aluno;
typedef struct index Index;


#endif

