#include "question.h"
#include "test_system.h"
#include <stdio.h>
#include <stdlib.h>


int main(){

    init_system();

    int exitflag = 0;

    while(1){
        init_ui();        
        int select = 0;
        select = user_select();
        if(select == 1) {
            do{
                exitflag = view_test_ui();
            }while(exitflag == 2);  
        }
        else if(select == 2) {
            do{
                exitflag = manage_test_ui();
            }while(exitflag == 2);     
        }
        else if(select == 3) {
            choicepratice();
        }
        else if(select == 4) break;
    }

    printf("已退出系统\n");

    return 0;
}