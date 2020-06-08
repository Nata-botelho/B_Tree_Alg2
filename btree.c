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
        newNode->keys[i].prim_key = -1;
        newNode->keys[i].RNN = (long)-1;
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
        if(newIndex)
            addIndexToTree(newIndex);
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

    FILE *index_file = fopen("index.dat", "rb+"); 
    fseek(index_file, RRN*PAGESIZE, SEEK_SET);
    fwrite(page, sizeof(Node), 1, index_file);

    fclose(index_file);
    return SUCCESS;
}


int addIndexToTree(Index *newIndex){

    Node *root = getRoot();

    if(root->key_count == ORDER-1){
        root = splitRoot(root);
        insertOnNode(root, newIndex, getRootRRN());

        return SUCCESS;
    }
    else{
        insertOnNode(root, newIndex, getRootRRN());
        return SUCCESS;
    }
}

long insertOnNode(Node *node, Index *newIndex, long RRN){
    int pos = node->key_count-1;
    long auxRRN;
    FILE *index_file = fopen("index.dat", "rb+");
    Node *auxNode;

    if(node->is_leaf){
        printf("folha\n");
        if(node->key_count == ORDER-1){
            printf("a folha ta cheia!\n");
        }
        else{
            while(pos >= 0 && (node->keys[pos].prim_key > newIndex->prim_key)){
                node->keys[pos+1] = node->keys[pos];
                pos--;
            }
            pos++;
            node->keys[pos] = *newIndex;
            node->key_count++;

            writePageOnFile(node, RRN);
            fclose(index_file);
            return RRN;
        }
    }
    else{
        while(pos >= 0 && (node->keys[pos].prim_key > newIndex->prim_key))
            pos--;

        pos++;
        fseek(index_file, node->children[pos], SEEK_SET);

        auxRRN = node->children[pos];
        auxNode = readPageFromFile(auxRRN);

        if(auxNode && auxNode->key_count == ORDER-1){
            splitNode(auxNode, pos, node, auxRRN);
        }
        insertOnNode(node, newIndex, RRN);
    }

    return -1;
}

void splitNode(Node *childNode, int pos, Node *mainNode, long childRRN){
    int i, mid = (ORDER-1)/2;
    Node *newNode = createNode(TRUE);
    FILE *index_file = fopen("index.dat", "rb+");

    for(i=pos; i<mainNode->key_count; i++)
        mainNode->keys[i+1] = mainNode->keys[i];
    mainNode->keys[pos] = childNode->keys[mid];
    mainNode->key_count++;

    for(i=0; i<pos; i++){
        newNode->keys[i] = childNode->keys[i];
        childNode->key_count++;
    }

    for(i=0; i<pos; i++)
        childNode->keys[i] = childNode->keys[pos+1];
    for(i=pos; i<ORDER-1; i++){
        childNode->keys[i].prim_key = -1;
        childNode->key_count--;
    }
    fseek(index_file, 0, SEEK_END);
    mainNode->children[pos] = ftell(index_file);
    mainNode->children[pos+1] = childRRN;
    writePageOnFile(newNode, ftell(index_file));
}

Node* splitRoot(Node *node){
    int pos = (ORDER-1)/2, i, j;
    FILE *index_file = fopen("index.dat", "rb+");
    long auxRRN;
    Node *left, *right;

    if(node->children[0] != -1) left = createNode(FALSE);
    else    left = createNode(TRUE);
    if(node->children[node->key_count-1] != -1) right = createNode(FALSE);
    else    right = createNode(TRUE);
    
    fseek(index_file, 0, SEEK_END);
    auxRRN = (ftell(index_file)/PAGESIZE);

    for(i=0; i < pos; i++){
        left->keys[i] = node->keys[i];
        left->children[i] = node->children[i];
        node->key_count--;
        left->key_count++;
        node->keys[i].prim_key = -1;
    }
    node->children[0] = auxRRN;
    writePageOnFile(left, auxRRN);

    fseek(index_file, 0, SEEK_END);
    auxRRN = (ftell(index_file)/PAGESIZE);

    for(j = 0, i=pos+1; i < ORDER-1; j++, i++){
        right->keys[j] = node->keys[i];
        right->children[j] = node->children[i];
        node->key_count--;
        right->key_count++;
        node->keys[i].prim_key = -1;
    }
    node->children[1] = auxRRN;

    writePageOnFile(right, auxRRN);

    node->keys[0] = node->keys[pos];
    node->keys[pos].prim_key = -1;
    node->is_leaf = FALSE;

    auxRRN = getRootRRN();
    writePageOnFile(node, auxRRN);
    
    fclose(index_file);
    return node;
}

long _bTreeSearch(FILE *file, Node *page, int key){
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