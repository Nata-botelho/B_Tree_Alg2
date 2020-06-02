/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

Node *createNode(bool is_leaf){
    Node *newNode;
    newNode = (Node*) malloc (sizeof(Node));
    
    newNode->is_leaf = is_leaf;
    newNode->key_count = 0;
    newNode->keys = (Index*) malloc ((ORDER-1) * sizeof(Index));
    newNode->children = (long*) malloc (ORDER * sizeof(long));

    return newNode;
}

Register *createRegister(){
    Register *newReg = (Register*) calloc(1, sizeof(Register));

    printf("Student's NUSP:");
	scanf("%u", &newReg->numUSP);

	printf("\nStudent's name:");
	scanf("%s", newReg->name);

	printf("\nStudent's surname:");
	scanf("%s", newReg->surname);

	printf("\nStudent's course:");
	scanf("%s", newReg->course);

	printf("\nStudent's grade:");
	scanf("%f", &newReg->grade);

    return newReg;
}

int writeRegisterOnFile(Register *newReg){
    FILE *data_file = fopen("dados.txt", "a+");
    if(!data_file)  printf("Erro no arquivo de dados!\n");
    fseek(data_file, 0, SEEK_END);
    fprintf(data_file, "%d%s%s%s%f", newReg->numUSP, newReg->name, newReg->surname, newReg->course, newReg->grade);

    fclose(data_file);
    return 1;
}

void addRegister(){
    Register *newReg = createRegister();
    if(newReg){
        if(writeRegisterOnFile(newReg)){
            printf("registro adicionado com sucesso!\n");
        }
    }
}