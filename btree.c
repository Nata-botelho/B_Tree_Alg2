/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"


Node *createNode(bool is_leaf){
    int i;
    Node *newNode = (Node*) malloc (PAGESIZE);
    
    newNode->is_leaf = is_leaf;
    newNode->key_count = 0;

    for(i=0; i<ORDER; i++)
        newNode->children[i] = (long)-1;

    for(i = 0; i < (ORDER-1); i++){
        newNode->keys[i].prim_key = (int)i;
        newNode->keys[i].RNN = (long)i;
    }
    memset(newNode->thrash, '@', THRASHSIZE);

    return newNode;
}

Register *createRegister(){
    Register *newReg = (Register*) calloc(1, sizeof(Register));

    printf("nUSP:");
	scanf("%d", &newReg->numUSP);

	printf("Nome:");
	scanf(" %[^\n]", newReg->name);

	printf("Sobrenome:");
	scanf(" %[^\n]", newReg->surname);

	printf("Curso:");
	scanf(" %[^\n]", newReg->course);

	printf("Nota");
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
    fwrite(newReg, sizeof(Register), 1, data_file);
    fclose(data_file);
    return newIndex;
}

/*add registers on */
void addRegister(){
    Register *newReg = createRegister();
    Index *newIndex;

    if(newReg){
        newIndex = writeRegisterOnFile(newReg);
        if(newIndex){
            addIndexToTree(newIndex);
            printf("\nRegistro inserido");
        }
    }
}

long getRootRRN(){
    FILE *index_file = fopen("index.dat", "rb+");
    if(!index_file) {fopen("index.dat", "wb+"); printf("arquivo de dados n existe.. criado\n");}
    long header = 1;
    char thrash[PAGESIZE - sizeof(long)];
    memset(thrash, '@', PAGESIZE-sizeof(long));
    

    /*se arquivo estiver vazio retorna 1 que sera a pos da root*/
    fseek(index_file, 0, SEEK_END);
    if(!ftell(index_file)) {
        fwrite(&header, sizeof(long), 1, index_file);
        fwrite(&thrash, sizeof(thrash), 1, index_file);
    }
    else{
        rewind(index_file);
        fread(&header, sizeof(long), 1, index_file);
    }

    fclose(index_file);
    return header;
}

Node *getRoot(){

    long rootRRN = getRootRRN();
    
    FILE *index_file = fopen("index.dat", "rb+");
    fseek(index_file, rootRRN*PAGESIZE, SEEK_SET);

    Node *root = (Node*) malloc (sizeof(Node));

    fread(root, PAGESIZE, 1, index_file);
    fclose(index_file);

    if(root->key_count > 0)    return root;
    else    return createNode(TRUE);
}

Register readRegisterFromFile(long RRN){
    Register auxReg;

    FILE *data_file = fopen("dados.txt", "rb");
    if(!data_file)  printf("Erro no arquivo de dados!\n");

    fseek(data_file, RRN, SEEK_SET);
    fread(&auxReg, sizeof(Register), 1, data_file);

    fclose(data_file);
    return auxReg;
}

void printRegister(Register reg){
    printf("Nusp: %d\n", reg.numUSP);
    printf("Name: %s\n", reg.name);
    printf("Sobrenome: %s\n", reg.surname);
    printf("Curso: %s\n", reg.course);
    printf("Nota: %.2f\n", reg.grade);
}

/*read on page*/
Node *readPageFromFile(long RRN){
    FILE *index_file = fopen("index.dat", "rb");
    if(!index_file) return NULL;

    Node *auxNode = createNode(TRUE);

    fseek(index_file, RRN*PAGESIZE, SEEK_SET);
    fread(auxNode, sizeof(Node), 1, index_file);
    fclose(index_file);

    return auxNode;
}

void getRegister(long RRN){
    Register auxReg = readRegisterFromFile(RRN);
    if(RRN){
    printRegister(auxReg);
    }
}

/*write in page*/
int writePageOnFile(Node *page, long RRN){
    if(!page) return -2;
    if(RRN < 0) return -3;
    int i;
    FILE *index_file = fopen("index.dat", "rb+"); 

    fseek(index_file, RRN*PAGESIZE, SEEK_SET);
    fwrite(page, sizeof(Node), 1, index_file);

    fclose(index_file);

    return SUCCESS;
}


int addIndexToTree(Index *newIndex){

    Node *root = getRoot();

    if(root->key_count == ORDER-1){
        splitNode(root);
        insertOnIncompleteNode(root, newIndex, getRootRRN());

        return SUCCESS;
    }
    else{
        insertOnIncompleteNode(root, newIndex, getRootRRN());
        return SUCCESS;
    }
}

long insertOnIncompleteNode(Node *node, Index *newIndex, long RRN){
    int pos = node->key_count-1;
    FILE *index_file = fopen("index.dat", "rb+");

    if(node->is_leaf){
        while(pos >= 0 && (node->keys[pos].prim_key > newIndex->prim_key)){
            node->keys[pos+1] = node->keys[pos];
            pos--;
        }
        
        pos++;
        node->keys[pos] = *newIndex;
        node->key_count++;

        writePageOnFile(node, RRN);
        
        return RRN;
    }
    else{
        while(pos >= 0 && (node->keys[pos].prim_key > newIndex->prim_key))
            pos--;

        pos++;
        fseek(index_file, node->children[pos], SEEK_SET);
        RRN = node->children[pos];
        fread(node, sizeof(node), 1, index_file);
        insertOnIncompleteNode(node, newIndex, RRN);
    }

    return -1;
}

void splitNode(Node *node){
    int pos = ORDER/2, i;
    FILE *index_file = fopen("index.dat", "ab+");
    long auxRRN;

    Node *left = createNode(TRUE);
    fseek(index_file, 0, SEEK_END);
    auxRRN = ftell(index_file);

    for(i=pos; i >= 0; i--){
        insertOnIncompleteNode(left, &node->keys[i], auxRRN);
        node->key_count--;
        node->keys[i].prim_key = -1;
    }
    node->children[0] = auxRRN;

    Node *right = createNode(TRUE);
    fseek(index_file, 0, SEEK_END);
    auxRRN = ftell(index_file);

    for(i=pos; i <= ORDER-1; i++){
        insertOnIncompleteNode(right, &node->keys[i], auxRRN);
        node->key_count--;
        node->keys[i].prim_key = -1;
    }
    node->children[1] = auxRRN;
    node->keys[0] = node->keys[pos];
    node->keys[pos].prim_key = -1;
    node->is_leaf = FALSE;
    
}

/*calculo do espraco sobrando pra fazer 4kb*/
int freeSpaceOnPage(){ 
    return  (PAGESIZE - (5 + ((ORDER-1)*12) + ((ORDER)*8)));
}


long _bTreeSearch(FILE*file, Node*page, int key){
    int pos = 0 ;
    Node *NextPage;

    /*percorre até atingir o pos equivalente ou menor*/
    while(page->key_count > pos && page->keys[pos].prim_key < key){
        pos++;
    }

    if(page->keys[pos].prim_key == key)
        return page->keys[pos].RNN;

    if(page->is_leaf) 
        return -1;

    /*procura a posição para entrar para o filho*/
    pos = 1;
    while(pos-1 > page->keys[pos].prim_key){
        pos++;
    }

    NextPage = readPageFromFile(page->children[pos-1]);

    return _bTreeSearch(file, NextPage, key);
 }


void bTreeSearch(){
    FILE *index_file = fopen("index.dat", "rb+");
    Node *root = getRoot();
    int key;
    printf("\n     Digite o nUSP para buscar:");
    scanf("%d",&key);
    long rrnReg = _bTreeSearch(index_file,root,key);
    if(rrnReg>-1){
         getRegister(rrnReg);
        
    }else{
        printf("        Registro não exite\n");
    }
}

 void menu(){
     printf("       \nOpções:\n");
     printf("       Inserir - 1\n");
     printf("       Buscar  - 2\n");
     printf("       Sair    - 3\n");
 }