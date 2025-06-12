#include "blank_question.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct BlankQuestion* blankHead = NULL;

void load_bfile(){
    
    FILE *file = fopen("./bquestion.txt", "r");
    if(file == NULL) return;
    char *p = NULL;
    char linebuffer[512] = {0};
    while(p = fgets(linebuffer, 512, file)){
        struct BlankQuestion* node = malloc(sizeof(struct BlankQuestion));
        INIT_LIST_HEAD(&node->list);
        sscanf(linebuffer,
            "%[^,],%[^,]",
            node->Question, 
            node->answer);
        //把node插入到head后面
        list_add(&node->list, &blankHead->list);
    }
    fclose(file);
}

void save_bfile(){
    //打开文件， 写文件内容（一行一行读取）， 关闭文件
    FILE *file = fopen("./bquestion.txt","w") ;//r只读， w只写（会清空文件）， a+(追加读写)，  w+读写（会清空文件）, r+读写
    if(file == NULL) return ;

    struct list_head  *head = &blankHead->list;
    struct list_head  *p = head->next;
    while(p != head){
        //根据链表指针head获取数据指针
        struct BlankQuestion* bqhead = list_entry(p, struct BlankQuestion, list);
        char linebuffer[512] = {0};
        int bytes_written = snprintf(linebuffer, sizeof(linebuffer), 
            "%s,%s\n", 
            bqhead->Question, 
            bqhead->answer);
        if (bytes_written >= sizeof(linebuffer)) {
        // 处理截断情况
            fprintf(stderr, "警告: 数据被截断，可能不完整\n");
        }
        //写入文件
        fwrite(linebuffer, 1, strlen(linebuffer), file);//（要写入的数据，一个元素的大小， 多少个元素， 要写入的文件)
        p = p->next;
    }
    fclose(file);//关闭文件
}

struct BlankQuestion* create_blankquestion_head(){
    if(blankHead != NULL) return blankHead;
    blankHead = malloc(sizeof(struct BlankQuestion));
    if(blankHead == NULL) return NULL;
    INIT_LIST_HEAD(&blankHead->list);
    return blankHead;
}
void insert_blankquestion(struct BlankQuestion* head){
    if(head == NULL) return;
    struct BlankQuestion* node = malloc(sizeof(struct BlankQuestion));
    INIT_LIST_HEAD(&node->list);

    printf("请输入题干:\n"); scanf("%s[^\n]", node->Question);
    while(getchar() != "\n");
    printf("请输入答案:\n"); scanf("%s[^\n]", node->answer);


    list_add(&node->list, &head->list);
}
void delete_blankquestion(struct BlankQuestion* node){
    if(node == NULL) return;
    list_del(&node->list);
    free(node);
}
void show_blankquestion(struct BlankQuestion* node){
    if(node == NULL) return;
    printf("%s %s\n", node->Question, node->answer);
}


int view_blankquestion(){
    struct list_head* head = &blankHead->list;
    int i = 0;
    int select = 0;

    while(1){
        system("clear");

        printf("----------------------填空题-------------------------\n");
        if(i == 0){
            head = head->next;
            if(head == &blankHead->list) head = head->next;
        }
        if(i = 1){
            head = head->prev;
            if(head == &blankHead->list) head = head->prev;
        }
        struct BlankQuestion* bQ = list_entry(head, struct BlankQuestion, list);
        show_blankquestion(bQ);
        printf("-1-上一题              2-返回                3-下一题-\n");
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else break;
    }
    return select;
}
int manage_blankquestion(){
    struct list_head* head = &blankHead->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        printf("----------------------填空题-------------------------\n");
        if(i==0) {
            head = head->next;
            if(head == &blankHead->list) head = head->next;
        }
        if(i==1) {
            head = head->prev;
            if(head == &blankHead->list) head = head->prev;
        }
        struct BlankQuestion* bQ = list_entry(head, struct BlankQuestion, list);
        
        show_blankquestion(bQ);
        printf("-1-上一题    2-返回    4-添加     5-删除     3-下一题-\n");   
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else if(select == 2) break;
        else if(select == 4) insert_blankquestion(bQ);
        else if(select == 5) {
            struct BlankQuestion* p = list_entry(head, struct BlankQuestion, list); 
            head = head->next;
            delete_blankquestion(p);
        }
        else printf("输入错误\n");
    }
    save_bfile();
    return select;
}