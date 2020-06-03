/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"


Node *createNode(bool is_leaf){
    int i;
    Node *newNode;
    newNode = (Node*) malloc (sizeof(Node));
    
    newNode->is_leaf = is_leaf;
    newNode->key_count = 0;
    newNode->keys = (Index*) malloc ((ORDER-1) * sizeof(Index));
    newNode->children = (long*) malloc (ORDER * sizeof(long));
    for(i = 0; i < ORDER; i++)
        newNode->children[i] = -1;

    return newNode;
}

Register *createRegister(){
    Register *newReg = (Register*) calloc(1, sizeof(Register));

    printf("Student's NUSP:");
	scanf("%d", &newReg->numUSP);

	printf("\nStudent's name:");
	scanf(" %[^\n]", newReg->name);

	printf("\nStudent's surname:");
	scanf(" %[^\n]", newReg->surname);

	printf("\nStudent's course:");
	scanf(" %[^\n]", newReg->course);

	printf("\nStudent's grade:");
	scanf("%f", &newReg->grade);

    return newReg;
}

Index *writeRegisterOnFile(Register *newReg){
    FILE *data_file = fopen("dados.txt", "ab+");
    if(!data_file)  printf("Erro no arquivo de dados!\n");
    fseek(data_file, 0, SEEK_END);

    Index *newIndex = (Index*) malloc (sizeof(Index));
    newIndex->prim_key = newReg->numUSP;
    newIndex->RNN = ftell(data_file);

    printf("Gravado no RRN: %ld\n", newIndex->RNN);
    fwrite(newReg, sizeof(Register), 1, data_file);

    fclose(data_file);
    return newIndex;
}

void addRegister(){
    Register *newReg = createRegister();
    Index *newIndex;
    if(newReg){
        newIndex = writeRegisterOnFile(newReg);
        if(newIndex){
            printf("registro adicionado com sucesso!\n");
        }
    }
}

Node *getRoot(FILE* index_file){
    if(!index_file) return NULL;

    fseek(index_file, 0, SEEK_END);
    if(!ftell(index_file)) return createNode(TRUE);

    Node *root = (Node*)malloc(sizeof(Node));
    long header;
    fread(&header, sizeof(long), 1, index_file);
    fseek(index_file, header*(PAGESIZE), SEEK_SET);
    fread(&root, sizeof(Node), 1, index_file);

    return root;
}

Register readRegisterFromFile(long RRN){
    Register auxReg;

    FILE *data_file = fopen("dados.txt", "ab+");
    if(!data_file)  printf("Erro no arquivo de dados!\n");

    fseek(data_file, RRN, SEEK_SET);
    fread(&auxReg, sizeof(Register), 1, data_file);

    return auxReg;
}

void printRegister(Register reg){
    printf("Nusp: %d\n", reg.numUSP);
    printf("Name: %s\n", reg.name);
    printf("Surname: %s\n", reg.surname);
    printf("Course: %s\n", reg.course);
    printf("Grade: %.2f\n", reg.grade);
}

void getRegister(){
    long RRN;
    /*printf("RRN: ");
    scanf("%ld", &RRN);*/
    Register auxReg = readRegisterFromFile(RRN);
    printRegister(auxReg);
}
