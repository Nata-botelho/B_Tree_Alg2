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

    /*printf("Gravado no RRN: %ld\n", newIndex->RNN);*/
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
            printf("registro adicionado com sucesso!\n");
            addIndexToTree(newIndex);
        }
    }
}

Node *getRoot(FILE* index_file){
    if(!index_file) return NULL;

    fseek(index_file, 0, SEEK_END);
    if(!ftell(index_file)) return createNode(TRUE);
    rewind(index_file);
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

/*read on page*/
Node *readPageFromFile(FILE *index_file, long RRN){
    if(!index_file) return NULL;

    Node *auxNode = (Node*) malloc (sizeof(Node));

    fseek(index_file, RRN*PAGESIZE, SEEK_SET);
    fread(auxNode, sizeof(Node), 1, index_file);

    return auxNode;
}

void getRegister(){
    long RRN;
    Register auxReg = readRegisterFromFile(RRN);
    printRegister(auxReg);
}

/*write in page*/
int _writePageOnFile(FILE *index_file, Node *page, long RRN){
    if(!index_file) return -1;
    if(!page) return -2;
    if(RRN < 0) return -3;

    fseek(index_file, RRN, SEEK_SET);
    fwrite(page, sizeof(Node), 1, index_file);
    fwrite(page,sizeof(char),freeSpaceOnPage(),index_file);
    return 1;
}

void writePageOnFile(FILE*arq,Node*page,long rrn){
    if(_writePageOnFile(arq,page,rrn)){
        printf("Pagina Inserida\n");
    }else{
        printf("Erro ao inserir pagina\n");
    }
}


void addIndexToTree(FILE* file, Node*root,Index *newIndex){
    FILE *index_file = fopen("index.dat", "ab+");
    if(!index_file) printf("Erro no index file!\n");

    /*Node *auxNode = getRoot(file); */
    if(root->is_leaf){

    }

    
}

/*leva até a rnn e pega uma pagina*/
Node *getPageOnFile(FILE* file, long rrn){
    if(!file) return NULL;
    if(rrn<0)return NULL;
    Node *new = createNode(TRUE); 
    fseek(file,rrn*PAGESIZE,SEEK_SET);
    fread(&new,sizeof(Node),1,file);
    return new;
}


/*calculo do espraco sobrando pra fazer 4kb*/
int freeSpaceOnPage(){ 
    return  (PAGESIZE - (5 + ((ORDER-1)*12) + ((ORDER)*8)));
}

long bTfreeSelect(btPage *node, int key, FILE *fp){
    int pos;
    /*printPageNode(node);*/
    btPage *nextPage;
    for(pos = 0; pos < node->numberOfKeys && node->records[pos].key < key; pos++);
    if(pos == node->numberOfKeys) pos--;
    
    if(node->records[pos].key == key)
        return node->records[pos].recordRRN;
    if(node->isLeaf) 
        return -1;

    if(key < node->records[pos].key)
        nextPage = getPage(node->childs[pos], fp);
    else
        nextPage = getPage(node->childs[pos+1], fp);
    return bTreeSelect(nextPage, key, fp);
}


long bTreeSeach(FILE*file,Node*page,int key){
    int pos;
    Node *NextPage;
    if(page->keys[pos].prim_key == key)
        return page->keys[pos].RNN;
    
    if
    return bTreeSeach(file,NextPage,file);
 }


