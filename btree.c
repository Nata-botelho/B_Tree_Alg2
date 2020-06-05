/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"


Node *createNode(bool is_leaf){
    int i;
    Node *newNode = (Node*) malloc (sizeof(Node));
    
    newNode->is_leaf = is_leaf;
    newNode->key_count = 0;

    newNode->keys = (Index*) calloc ((ORDER-1), sizeof(Index));
    for(i = 0; i < (ORDER-1); i++)
        newNode->keys[i].prim_key = -1;

    newNode->children = (long*) malloc (ORDER * sizeof(long));
     for(i = 0; i < ORDER;i++)
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

    /*printf("Gravado no RRN: %ld\n", newIndex->RNN);*/
    fwrite(newReg, sizeof(Register), 1, data_file);

    fclose(data_file);
    return newIndex;
}

/*add registers on */
void addRegister(){
    Register *newReg = createRegister();
    Index *newIndex;
    long rootRRN = getRootRRN();
    Node *root = getRoot(rootRRN);

    if(newReg){
        newIndex = writeRegisterOnFile(newReg);
        if(newIndex){
            printf("registro adicionado com sucesso!\n");
            addIndexToTree(root, newIndex, rootRRN);

        }
    }
}

long getRootRRN(){
    FILE *index_file = fopen("index.dat", "ab+");

    /*se arquivo estiver vazio retorna 1 que sera a pos da root*/
    fseek(index_file, 0, SEEK_END);
    if(!ftell(index_file)) return 1;

    rewind(index_file);
    long header = fread(&header, sizeof(long), 1, index_file);

    fclose(index_file);
    return header;
}

Node *getRoot(long rootRRN){

    if(rootRRN == 1) return createNode(TRUE);

    Node *root = (Node*)malloc(sizeof(Node));
    
    FILE *index_file = fopen("index.dat", "ab+");
    fseek(index_file, rootRRN*(PAGESIZE), SEEK_SET);
    fread(&root, sizeof(Node), 1, index_file);
    fclose(index_file);

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

/*read on page*/
Node *readPageFromFile(long RRN){
    FILE *index_file = fopen("index.dat", "ab+");
    if(!index_file) return NULL;

    Node *auxNode = createNode(TRUE);

    fseek(index_file, RRN*PAGESIZE, SEEK_SET);
    fread(auxNode, sizeof(Node), 1, index_file);
    fclose(index_file);

    return auxNode;
}

void getRegister(long RRN){
    Register auxReg = readRegisterFromFile(RRN);
    printRegister(auxReg);
}

/*write in page*/
int _writePageOnFile(Node *page, long RRN){
    if(!page) return -2;
    if(RRN < 0) return -3;

    FILE *index_file = fopen("index.dat", "ab+"); 

    fseek(index_file, RRN, SEEK_SET);
    fwrite(page, sizeof(Node), 1, index_file);
    fwrite("@", sizeof(char), freeSpaceOnPage(), index_file);
    fclose(index_file);

    return 1;
}

void writePageOnFile(Node*page, long rrn){
    if(_writePageOnFile(page, rrn)){
        printf("Pagina Inserida\n");
    }else{
        printf("Erro ao inserir pagina\n");
    }
}


int addIndexToTree(Node *node, Index *newIndex, long RRN){

    int pos = node->key_count;

    while(pos >= 0 && newIndex->prim_key < node->keys[pos].prim_key){
        node->keys[pos+1] = node->keys[pos];
        pos--;
    }

    /*checa se o no se trata de uma folha*/
    if(node->is_leaf){
        /*checa se esta cheio, caso nao esteja insere o indice e retorna sucesso*/
        if(node->key_count < ORDER-1){

            pos++;
            node->keys[pos] = *newIndex;
            node->key_count++;

            writePageOnFile(node, RRN);
            return SUCCESS;
        }
        else{
            /*caso o no seja folha mas esteja cheio...*/

        }
    }
    else{
        /*caso o no tenha filhos*/
        if(node->children[pos] > -1){
            node = readPageFromFile(node->children[pos]);
            addIndexToTree(readPageFromFile(node->children[pos]), newIndex, node->children[pos]);
        }

    }

    
}

/*calculo do espraco sobrando pra fazer 4kb*/
int freeSpaceOnPage(){ 
    return  (PAGESIZE - (5 + ((ORDER-1)*12) + ((ORDER)*8)));
}


long bTreeSearch(FILE*file, Node*page, int key){
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

    return bTreeSearch(file, NextPage, key);
 }


