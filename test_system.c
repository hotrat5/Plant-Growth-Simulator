#include "question.h"
#include <stdio.h>
#include <stdlib.h>

int user_select(void){
    int num = 0;
    printf("请输入:\n");
    scanf("%d", &num);
    return num;
}

void init_system(){
    create_choicequestion_head();
    create_judgequestion_head();
    create_blankquestion_head();
    create_shortquestion_head();
    create_programmingquestion_head();

    load_file();
    load_jfile();
    load_bfile();
    load_sfile();
    load_pfile();
}

//查看试题
int view_test_ui(){

    system("clear");
    printf("---------------------试题查看------------------------\n");
    printf("--------------------1.选择题-------------------------\n");
    printf("--------------------2.填空题-------------------------\n");
    printf("--------------------3.判断题-------------------------\n");
    printf("--------------------4.简答题-------------------------\n");
    printf("--------------------5.编程题-------------------------\n");
    printf("--------------------6.返回---------------------------\n");

    int exitflag = 0;
    int select = user_select();

    switch(select){
        case 1: exitflag = view_choicequestion();break;
        case 2: exitflag = view_blankquestion();break;
        case 3: exitflag = view_judgequestion();break;
        case 4: exitflag = view_shortquestion();break;
        case 5: exitflag = view_programmingquestion();break;
        case 6: exitflag = select;break;
    }
            
    return exitflag;
}

//管理试题
int manage_test_ui(){
    system("clear");
    printf("---------------------试题管理------------------------\n");
    printf("--------------------1.选择题-------------------------\n");
    printf("--------------------2.填空题-------------------------\n");
    printf("--------------------3.判断题-------------------------\n");
    printf("--------------------4.简答题-------------------------\n");
    printf("--------------------5.编程题-------------------------\n");
    printf("--------------------6.返回---------------------------\n");

    int exitflag = 0;
    int select = user_select();
            
    switch(select){
        case 1: exitflag = manage_choicequestion();break;
        case 2: exitflag = manage_blankquestion();break;
        case 3: exitflag = manage_judgequestion();break;
        case 4: exitflag = manage_shortquestion();break;
        case 5: exitflag = manage_programmingquestion();break;
        case 6: exitflag = select;break;
    }

    return exitflag;
}

void init_ui(){
    system("clear");
    printf("--------------------试题管理系统---------------------\n");
    printf("----------------------------------------------------\n");
    printf("--------------------1.查看试题-----------------------\n");
    printf("--------------------2.管理试题-----------------------\n");
    printf("--------------------3.练习---------------------------\n");
    printf("--------------------4.退出---------------------------\n");
    printf("----------------------------------------------------\n");   
}
