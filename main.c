/*---------------------------------------------*/
/*--Igor Guilherme Pereira Lorendo - 11275071--*/
/*-------Nata Silva Botelho - 11275105---------*/
/*---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"
 
int main(){

    int option = 0;

    do{
        scanf("%d", &option);

        if(option == 1)
            addRegister();
        else if(option == 2)
            getRegister();
            
    }while(option);
    
    
    return 0;
};