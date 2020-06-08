/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"
 
int main(){

    int option = 0;
     system("clear");

    do{
        menu();
         scanf("%d", &option);
        if(option == 1){
            addRegister();
        }
        else if(option == 2){
            bTreeSearch();
        }else if (option != 3){
            printf("Opção errada!!\n");
        }        
    }while(option != 3);
    
    
    return 0;
};