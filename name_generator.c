#include <stdio.h>
#include <stdlib.h>
#include <time.h>


char **readNameList(int quantity){
    FILE *namesfile = fopen("namelist.txt", "r+");

    char **namelist = (char**) malloc (quantity*sizeof(char*));
    for(int i = 0; i<quantity; i++){
        namelist[i] = (char*) malloc (16);
        fscanf(namesfile, "%s\n", namelist[i]);
        if(i%51 == 0)   rewind(namesfile);
    }

    fclose(namesfile);
    return namelist;
}

char **readSurnameList(int quantity){
    FILE *surnamefile = fopen("surnamelist.txt", "r+");

    char **surnamelist = (char**) malloc (quantity*sizeof(char*));
    for(int i = 0; i<quantity; i++){
        surnamelist[i] = (char*) malloc (16);
        fscanf(surnamefile, "%s\n", surnamelist[i]);
        if(i%51 == 0)   rewind(surnamefile);
    }

    fclose(surnamefile);
    return surnamelist;
}

char **readCourseList(int quantity){
    FILE *coursefile = fopen("courselist.txt", "r+");

    char **courselist = (char**) malloc (quantity*sizeof(char*));
    for(int i = 0; i<quantity; i++){
        courselist[i] = (char*) malloc (16);
        fscanf(coursefile, "%s\n", courselist[i]);
        if(i%51 == 0)   rewind(coursefile);
    }

    fclose(coursefile);
    return courselist;
}

int main(){

    srand(time(NULL));

    FILE *data_file = fopen("entries.txt", "w+");

    int quantity, i;
    char **namelist;
    char **surnamelist;
    char **courselist;
    
    scanf("%d", &quantity);

    namelist = readNameList(quantity);
    surnamelist = readSurnameList(quantity);
    courselist = readCourseList(quantity);

    for(i = 0; i < quantity; i++){
        printf("%d\n", i);
        fprintf(data_file, "1\n%d\n%s\n%s\n%s\n%f\n", i, namelist[rand()%quantity], surnamelist[rand()%quantity], courselist[rand()%12], (float)(rand()%11));
    }

    fclose(data_file);

    return 0;
}
