#include "question.h"
#include <stdio.h>
#include <stdlib.h>

extern struct choiceQuestion* choiceQuestionList;

void show_pratice(struct choiceQuestion* node){
    if(node == NULL) return;
    printf("------------------选择题--------------------\n");
    printf("%s\n", node->Question);
    printf("A.%s\n", node->Option[0]);
    printf("B.%s\n", node->Option[0]);
    printf("C.%s\n", node->Option[0]);
    printf("D.%s\n", node->Option[0]);
    printf("-1-上一题------2-返回------3-作答-------3-下一题-\n");
}

void pratice(){
    struct list_head* head = &choiceQuestionList->list;

    int i = 0;
    int select = 0;
    while(1){
        if(i = 0) {
        head = head->next;
        if(head == &choiceQuestionList->list) head = head->next;
        }
        if(i = 1){
            head = head->prev;
            if(head == &choiceQuestionList->list) head = head->prev;
        }

        struct choiceQuestion* cQ = list_entry(head, struct choiceQuestion, list);
        show_pratice(cQ);
    
        printf("请输入操作:\n");
        scanf("%d", &select);
        if(select == 1) i = 1;
        if(select == 4) i = 0;
        if(select == 3){
            char asw;
            printf("请输入:\n");
            scanf("%c", &asw);
            if((asw-'A') == cQ->answer){
                printf("正确");
                i = 0;
            } 
            else {
                printf("错误");
                i = 0;
            }    
        }
        if(select == 2) break;
    }
    return select;
}