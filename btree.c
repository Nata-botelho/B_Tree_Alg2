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
    newNode->keys = (Index*) calloc ((ORDER-1) , sizeof(Index));

    for(i = 0; i < ORDER - 1;i++)
        newNode->keys[i].prim_key = -1;

    newNode->children = (long*) malloc (ORDER * sizeof(long));
     for(i = 0; i < ORDER;i++)
        newNode->children[i] = -1;
    
    return newNode;
}

Register *createRegister(){
    Register *newReg = (Register*) calloc(1, sizeof(Register));

    printf("Student's NUSP:");
	scanf("%u", &newReg->numUSP);

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

Node *readPageFile(FILE*arq){
    if(!arq) return NULL;
    int i;
    Node *new = createNode(FALSE);
    fread(&new->is_leaf,sizeof(bool),1,arq);
    fread(&new->key_count,sizeof(int),1,arq);

    for(i = 0; i < ORDER; i++)
        fread(&new->children[i],sizeof(long int ),1,arq);

    for(i = 0; i < ORDER - 1; i++)
        fread(&new->keys[i].prim_key,sizeof(int),1,arq);

    for(i = 0; i < ORDER - 1; i++)
        fread(&new->keys[i].RNN,sizeof(long int),1,arq);

    return new;
}


Node *getRoot(FILE* arq){
    if(!arq) return NULL;
    fseek(arq, 0, SEEK_END);
    if(!ftell(arq)) return createNode(TRUE);
    rewind(arq);
    long header;
    fread(&header,sizeof(long),1,arq);
    fseek(arq,header*(PAGESIZE),SEEK_SET);
    Node *root = readPageFile(arq);
    return root;
}

/*write in page*/
int _writePageOnFile(FILE *arq,Node *page,long rrn){
    if(!arq) return -1;
    if(!page) return -2;
    if(rrn < 0) return -3;
    
    /*int i;
    fwrite(&page->is_leaf,sizeof(bool),1,arq);
    fwrite(&page->key_count,sizeof(int),1,arq);
    fseek(arq,rrn*PAGESIZE,SEEK_SET);
   
    for(i =0 ; i < ORDER ; i++)
        fwrite(&page->children[i],sizeof(long),1,arq);

    for(i =0 ; i < ORDER -1 ; i++)
        fwrite(&page->keys[i].prim_key,sizeof(int),1,arq);

    for(i =0 ; i < ORDER - 1; i++)
        fwrite(&page->keys[i].RNN,sizeof(long),1,arq);

    return 1; */

    
    fwrite(&page,sizeof(Node),1,arq);

}

void writePageOnFile(FILE*arq,Node*page,long rrn){
    if(_writePageOnFile(arq,page,rrn)){
        printf("Pagina Inserida\n");
    }else{
        printf("Erro ao inserir pagina\n");
    }
}