#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char *getRandomName(){

    static char **sampleList;
    static int counter = 0;

    if(!sampleList){
        FILE *sample_file = fopen("namelist.txt", "r+");
        if(!sample_file){printf("Erro no arquivo de amostras de nomes!"); exit(0);}

        sampleList = (char**) malloc (sizeof(char*));

        char *aux = (char*) malloc (sizeof(char)*16);
        printf("miniboi ");
        while(fscanf(sample_file, "%s", aux)){
            printf("%d\n", counter);
            sampleList[counter] = strdup(aux);
            counter++;
            sampleList = (char**) realloc (sampleList, sizeof(char*)*counter);
        }

        fclose(sample_file);
    }

    return sampleList[rand()%counter];
}

char *getRandomLastName(){
    static char **sampleList;
    static int counter = 0;

    if(!sampleList){
        FILE *sample_file = fopen("lastnamelist.txt", "r+");
        if(!sample_file){printf("Erro no arquivo de amostras de sobrenomes!"); exit(0);}

        sampleList = (char**) malloc (sizeof(char*));
        char *aux = NULL;
        while(fscanf(sample_file, "%s\n", aux)){
            sampleList[counter] = strdup(aux);
            counter++;
            sampleList = (char**) realloc (sampleList, sizeof(char*)*counter);
        }

        fclose(sample_file);
    }

    return sampleList[rand()%counter];
}

char *getRandomCourse(){
    static char **sampleList;
    static int counter = 0;

    if(!sampleList){
        FILE *sample_file = fopen("courselist.txt", "r+");
        if(!sample_file){printf("Erro no arquivo de amostras de cursos!"); exit(0);}

        sampleList = (char**) malloc (sizeof(char*));
        char *aux = NULL;
        while(fscanf(sample_file, "%s\n", aux)){
            sampleList[counter] = strdup(aux);
            counter++;
            sampleList = (char**) realloc (sampleList, sizeof(char*)*counter);
        }

        fclose(sample_file);
    }

    return sampleList[rand()%counter];
}
/*
char **readNameList(int *counter){
    FILE *namesfile = fopen("namelist.txt", "r+");
    
    char **namelist = (char**) malloc (sizeof(char*));
    char *aux;

    
    fclose(namesfile);
    return namelist;
}

char **readSurnameList(int *counter){
    FILE *surnamefile = fopen("surnamelist.txt", "r+");

    char **surnamelist = (char**) malloc (sizeof(char*));
    char *aux;

    while(fscanf(surnamefile, "%s\n", aux)){
        surnamelist[*counter] = strdup(aux);
        (*counter)++;
        surnamelist = (char**) realloc (surnamelist, sizeof(char*)*(*counter));
    }

    fclose(surnamefile);
    return surnamelist;
}

char **readCourseList(int *counter){
    FILE *coursefile = fopen("courselist.txt", "r+");

    char **courselist = (char**) malloc (sizeof(char*));
    char *aux;

    while(fscanf(coursefile, "%s\n", aux)){
        courselist[*counter] = strdup(aux);
        (*counter)++;
        courselist = (char**) realloc (courselist, sizeof(char*)*(*counter));
    }

    fclose(coursefile);
    return courselist;
}*/

int main(){

    srand(time(NULL));

    FILE *data_file = fopen("entries.txt", "w+");

    int quantity, i;
    /*char **namelist;
    char **surnamelist;
    char **courselist;
    
    int namecount = 0, surnamecount = 0, coursecount = 0;*/
    char *auxname;
    char *auxlastname;
    char *auxcourse;
    scanf("%d", &quantity);

   // namelist = readNameList(&namecount);
   // surnamelist = readSurnameList(&surnamecount);
   // courselist = readCourseList(&coursecount);

    for(i = 0; i < quantity; i++){
        auxname = getRandomName();
        printf("%s\n", auxname);
        auxlastname = getRandomLastName();
        auxcourse = getRandomCourse();
        //fprintf(data_file, "1\n%d\n%s\n%s\n%s\n%f\n", i, namelist[rand()%namecount], surnamelist[rand()%surnamecount], courselist[rand()%coursecount], (float)(rand()%11));
        fprintf(data_file, "1\n%d\n%s%s%s%f\n", i, auxname, auxlastname, auxcourse, (float)(rand()%11));
    }

    fclose(data_file);

    return 0;
}
