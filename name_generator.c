#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char *getRandomName(int flag){

    static char **sampleList;
    static int counter = 0;

    if(!sampleList){
        FILE *sample_file = fopen("namelist.txt", "r+");
        if(!sample_file){printf("Erro no arquivo de amostras de nomes!"); exit(0);}

        sampleList = (char**) malloc (sizeof(char*));
        char *aux = (char*) malloc (sizeof(char)*16);

        while(fscanf(sample_file, "%s\n", aux) == 1){
            sampleList[counter] = strdup(aux);
            counter++;
            sampleList = (char**) realloc (sampleList, sizeof(char*)*(counter+1));
        }

        fclose(sample_file);
    }

    if(flag)
        return sampleList[rand()%counter];
    else{
        for(int i = 0; i < counter; i++)
            free(sampleList[i]);
        free(sampleList);
        return NULL;
    }
}

char *getRandomLastName(int flag){

    static char **sampleList;
    static int counter = 0;

    if(!sampleList){
        FILE *sample_file = fopen("lastnamelist.txt", "r+");
        if(!sample_file){printf("Erro no arquivo de amostras de sobrenomes!"); exit(0);}

        sampleList = (char**) malloc (sizeof(char*));
        char *aux = (char*) malloc (sizeof(char)*16);

        while(fscanf(sample_file, "%s\n", aux) == 1){
            sampleList[counter] = strdup(aux);
            counter++;
            sampleList = (char**) realloc (sampleList, sizeof(char*)*(counter+1));
        }

        fclose(sample_file);
    }

    if(flag)
        return sampleList[rand()%counter];
    else{
        for(int i = 0; i < counter; i++)
            free(sampleList[i]);
        free(sampleList);
        return NULL;
    }
}

char *getRandomCourse(int flag){
    
    static char **sampleList;
    static int counter = 0;

    if(!sampleList){
        FILE *sample_file = fopen("courselist.txt", "r+");
        if(!sample_file){printf("Erro no arquivo de amostras de cursos!"); exit(0);}

        sampleList = (char**) malloc (sizeof(char*));
        char *aux = (char*) malloc (sizeof(char)*16);

        while(fscanf(sample_file, "%s\n", aux) == 1){
            sampleList[counter] = strdup(aux);
            counter++;
            sampleList = (char**) realloc (sampleList, sizeof(char*)*(counter+1));
        }

        fclose(sample_file);
    }

    if(flag)
        return sampleList[rand()%counter];
    else{
        for(int i = 0; i < counter; i++)
            free(sampleList[i]);
        free(sampleList);
        return NULL;
    }
}


int main(){

    FILE *data_file = fopen("entries.txt", "w+");
    int quantity, i;

    srand(time(NULL));
    printf("Digite a quantidade de registros que devem ser criados:\n");
    scanf("%d", &quantity);

    for(i = 0; i < quantity; i++)
        fprintf(data_file, "1\n%d\n%s\n%s\n%s\n%.2f", (i+200000), getRandomName(1), getRandomLastName(1), getRandomCourse(1), (float)(rand()%11));

    if(data_file)   printf("Criado arquivo entries.txt com %d registros!\n", quantity);
    fclose(data_file);
    getRandomName(0);
    getRandomLastName(0);
    getRandomCourse(0);

    return 0;
}