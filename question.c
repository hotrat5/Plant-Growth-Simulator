#include "question.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct choiceQuestion* choiceQuestionList = NULL;
static struct JudgeQuestion* judgeHead = NULL;
static struct BlankQuestion* blankHead = NULL;
static struct ShortQuestion* shortHead = NULL;
static struct ProgrammingQuestion* programmingHead = NULL;

void delay_ms(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + (milliseconds * CLOCKS_PER_SEC / 1000));
}

void load_file(){
    FILE *file = fopen("./cquestion.txt", "r");
    if(file == NULL) return;
    char *p = NULL;
    char linebuffer[512] = {0};
    while(p = fgets(linebuffer, 512, file)){
        struct choiceQuestion* node = malloc(sizeof(struct choiceQuestion));
        INIT_LIST_HEAD(&node->list);
        sscanf(linebuffer,
            "%[^,],%[^,],%[^,],%[^,],%[^,],%d",
            node->Question, 
            node->Option[0],node->Option[1],node->Option[2],node->Option[3],
            &node->answer);
        //把node插入到head后面
        list_add(&node->list, &choiceQuestionList->list);
    }
    fclose(file);
}

void save_file(){
    //打开文件， 写文件内容（一行一行读取）， 关闭文件
    FILE *file = fopen("./cquestion.txt","w") ;//r只读， w只写（会清空文件）， a+(追加读写)，  w+读写（会清空文件）, r+读写
    if(file == NULL) return ;

    struct list_head  *head = &choiceQuestionList->list;
    struct list_head  *p = head->next;
    while(p != head){
        //根据链表指针head获取数据指针
        struct choiceQuestion* cqhead = list_entry(p, struct choiceQuestion, list);

        char linebuffer[512] = {0};
        int bytes_written = snprintf(linebuffer, sizeof(linebuffer), 
            "%s,%s,%s,%s,%s,%d\n", 
            cqhead->Question, 
            cqhead->Option[0],cqhead->Option[1],cqhead->Option[2],cqhead->Option[3],
            cqhead->answer);
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

//创建
struct choiceQuestion* create_choicequestion_head(){

    if(choiceQuestionList != NULL) return choiceQuestionList;

    choiceQuestionList = malloc(sizeof(struct choiceQuestion));
    if(choiceQuestionList == NULL) return NULL;

    INIT_LIST_HEAD(&choiceQuestionList->list);
    return choiceQuestionList;
}

//插入
void insert_choicequestion(struct choiceQuestion* head){
    if(head == NULL) return;

    struct choiceQuestion* node = malloc(sizeof(struct choiceQuestion));
    if(node == NULL) return;
    INIT_LIST_HEAD(&node->list);

    printf("请输入题干:\n"); scanf("%s[^\n]", node->Question);
    while(getchar() != '\n');
    printf("请输入A选项:\n");scanf("%s[^\n]", node->Option[0]);
    while(getchar() != '\n');
    printf("请输入B选项:\n"); scanf("%s[^\n]", node->Option[1]);
    while(getchar() != '\n');
    printf("请输入C选项:\n"); scanf("%s[^\n]", node->Option[2]);
    while(getchar() != '\n');
    printf("请输入D选项:\n"); scanf("%s[^\n]", node->Option[3]);
    while(getchar() != '\n');
    printf("请输入答案:\n"); scanf("%d", &node->answer);
    
    list_add(&node->list, &head->list);
    printf("选择题添加成功！\n");
}

//删除
void delete_choicequestion(struct choiceQuestion* node){
    if(node == NULL) return;

    list_del(&node->list);
    free(node);
}

void search_choicequestion(struct choiceQuestion* node);
void modify_choicequestion(struct choiceQuestion* node);


void show_choicequestion(struct choiceQuestion* node){
    if(node == NULL) return;
    
    printf("%s\n", node->Question);
    printf("A.%s %s\n", node->Option[0], (node->answer==0)?"✔":"");
    printf("B.%s %s\n", node->Option[1], (node->answer==1)?"✔":"");
    printf("C.%s %s\n", node->Option[2], (node->answer==2)?"✔":"");
    printf("D.%s %s\n", node->Option[3], (node->answer==3)?"✔":"");
}


int view_choicequestion(){
    struct list_head* head = &choiceQuestionList->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        printf("----------------------选择题-------------------------\n");
        if(i==0) {
            head = head->next;
            if(head == &choiceQuestionList->list) head = head->next;
        }
        if(i==1) {
            head = head->prev;
            if(head == &choiceQuestionList->list) head = head->prev;
        }
        struct choiceQuestion* cQ = list_entry(head, struct choiceQuestion, list);
        
        show_choicequestion(cQ);
        printf("-1-上一题              2-返回                3-下一题-\n");   
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else break;
    }
    return select;
}

int manage_choicequestion(){
    struct list_head* head = &choiceQuestionList->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        printf("----------------------选择题-------------------------\n");
        if(i==0) {
            head = head->next;
            if(head == &choiceQuestionList->list) head = head->next;
        }
        if(i==1) {
            head = head->prev;
            if(head == &choiceQuestionList->list) head = head->prev;
        }
        
        struct choiceQuestion* cQ = list_entry(head, struct choiceQuestion, list);
        show_choicequestion(cQ);
        printf("-1-上一题    2-返回    4-添加     5-删除     3-下一题-\n");   
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else if(select == 2) break;
        else if(select == 4) insert_choicequestion(cQ);
        else if(select == 5) {
            struct choiceQuestion* p = list_entry(head, struct choiceQuestion, list); 
            head = head->next;
            delete_choicequestion(p);
        }
        else {
            printf("输入错误\n");
            break;
        }
    }
    save_file();
    return select;
}






void load_jfile(){
    FILE *file = fopen("./jquestion.txt", "r");
    if(file == NULL) return;
    char *p = NULL;
    char linebuffer[512] = {0};
    while(p = fgets(linebuffer, 512, file)){
        struct JudgeQuestion* node = malloc(sizeof(struct JudgeQuestion));
        INIT_LIST_HEAD(&node->list);
        sscanf(linebuffer,
            "%[^,],%d",
            node->Question, 
            &node->answer);
        //把node插入到head后面
        list_add(&node->list, &judgeHead->list);
    }
    fclose(file);
}
void save_jfile(){
    //打开文件， 写文件内容（一行一行读取）， 关闭文件
    FILE *file = fopen("./cquestion.txt","w") ;//r只读， w只写（会清空文件）， a+(追加读写)，  w+读写（会清空文件）, r+读写
    if(file == NULL) return ;

    struct list_head  *head = &judgeHead->list;
    struct list_head  *p = head->next;
    while(p != head){
        //根据链表指针head获取数据指针
        struct JudgeQuestion* jqhead = list_entry(p, struct JudgeQuestion, list);
        char linebuffer[512] = {0};
        int bytes_written = snprintf(linebuffer, sizeof(linebuffer), 
            "%s,%d\n", 
            jqhead->Question, 
            jqhead->answer);
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







struct JudgeQuestion* create_judgequestion_head(){
    if(judgeHead != NULL) return judgeHead;
    judgeHead = malloc(sizeof(struct JudgeQuestion));
    if(judgeHead == NULL) return NULL;
    INIT_LIST_HEAD(&judgeHead->list);
    return judgeHead;
}

void insert_judgequestion(struct JudgeQuestion* head){
    if(head == NULL) return;
    struct JudgeQuestion* node = malloc(sizeof(struct JudgeQuestion*));
    INIT_LIST_HEAD(&node->list);

    printf("请输入题干:"); scanf("%s[^\n]", node->Question);
    while(getchar() != '\n');
    printf("请输入答案(1-正确,0-错误):\n");scanf("%d", &node->answer);

    list_add(&node->list, &head->list);
}
void delete_judgequestion(struct JudgeQuestion* node){
    if(node == NULL) return;

    list_del(&node->list);
    free(node);
}

void show_judgequestion(struct JudgeQuestion* node){
    if(node == NULL) return;
    printf("%s %s\n", node->Question, (node->answer == 1)? "✔":"");
}


int view_judgequestion(){
    struct list_head* head = &judgeHead->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        printf("----------------------判断题-------------------------\n");
        if(i==0) {
            head = head->next;
            if(head == &judgeHead->list) head = head->next;
        }
        if(i==1) {
            head = head->prev;
            if(head == &judgeHead->list) head = head->prev;
        }
        struct JudgeQuestion* jQ = list_entry(head, struct JudgeQuestion, list);
        
        show_judgequestion(jQ);
        printf("-1-上一题              2-返回                3-下一题-\n");   
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else break;
    }
    return select;
}

int manage_judgequestion(){
    struct list_head* head = &judgeHead->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        printf("----------------------判断题-------------------------\n");
        if(i==0) {
            head = head->next;
            if(head == &judgeHead->list) head = head->next;
        }
        if(i==1) {
            head = head->prev;
            if(head == &judgeHead->list) head = head->prev;
        }
        struct JudgeQuestion* jQ = list_entry(head, struct JudgeQuestion, list);
        
        show_judgequestion(jQ);
        printf("-1-上一题    2-返回    4-添加     5-删除     3-下一题-\n");   
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else if(select == 2) break;
        else if(select == 4) insert_judgequestion(jQ);
        else if(select == 5) {
            struct JudgeQuestion* p = list_entry(head, struct JudgeQuestion, list); 
            head = head->next;
            delete_judgequestion(p);
        }
        else {
            printf("输入错误\n");
            break;
        }
    }
    save_jfile();
    return select;

}


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
    while(getchar() != '\n');
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
        if(i == 1){
            head = head->prev;
            if(head == &blankHead->list) head = head->prev;
        }
        struct BlankQuestion* bQ = list_entry(head, struct BlankQuestion, list);
        show_blankquestion(bQ);
        printf("-1-上一题              -2-返回                -3-下一题-\n");
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
        else {
            printf("输入错误\n");
            break;
        }
    }
    save_bfile();
    return select;
}


void load_sfile(){
    
    FILE *file = fopen("./squestion.txt", "r");
    if(file == NULL) return;
    char *p = NULL;
    char linebuffer[512] = {0};
    while(p = fgets(linebuffer, 512, file)){
        struct ShortQuestion* node = malloc(sizeof(struct ShortQuestion));
        if(node == NULL) return;
        INIT_LIST_HEAD(&node->list);
        sscanf(linebuffer,
            "%[^,],%[^,]",
            node->Question, 
            node->answer);
        //把node插入到head后面
        list_add(&node->list, &shortHead->list);
    }
    fclose(file);
}
void save_sfile(){
    //打开文件， 写文件内容（一行一行读取）， 关闭文件
    FILE *file = fopen("./squestion.txt","w") ;//r只读， w只写（会清空文件）， a+(追加读写)，  w+读写（会清空文件）, r+读写
    if(file == NULL) return ;

    struct list_head  *head = &shortHead->list;
    struct list_head  *p = head->next;
    while(p != head){
        //根据链表指针head获取数据指针
        struct ShortQuestion* sqhead = list_entry(p, struct ShortQuestion, list);
        char linebuffer[512] = {0};
        int bytes_written = snprintf(linebuffer, sizeof(linebuffer), 
            "%s,%s\n", 
            sqhead->Question, 
            sqhead->answer);
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


struct ShortQuestion* create_shortquestion_head(){
    if(shortHead != NULL) return shortHead;
    shortHead = malloc(sizeof(struct ShortQuestion));
    if(shortHead == NULL) return NULL;
    INIT_LIST_HEAD(&shortHead->list);
    return shortHead;
}

void insert_shortquestion(struct ShortQuestion* head){
    if(head == NULL) return;
    struct ShortQuestion* node = malloc(sizeof(struct ShortQuestion));
    INIT_LIST_HEAD(&node->list);

    printf("请输入题干:\n"); scanf("%s[^\n]", node->Question);
    while(getchar() != '\n');
    printf("请输入答案:\n"); scanf("%s[^\n]", node->answer);

    list_add(&node->list, &head->list);
}
void delete_shortquestion(struct ShortQuestion* node){
    if(node == NULL) return;
    list_del(&node->list);
    free(node);
}
void show_shortquestion(struct ShortQuestion* node){
    if(node == NULL) return;
    printf("题目: %s\n", node->Question);
    printf("解答: %s\n", node->answer);
}

int view_shortquestion(){
    struct list_head* head = &shortHead->list;
    int i = 0;
    int select = 0;

    while(1){
        system("clear");

        printf("----------------------填空题-------------------------\n");
        if(i == 0){
            head = head->next;
            if(head == &shortHead->list) head = head->next;
        }
        if(i == 1){
            head = head->prev;
            if(head == &shortHead->list) head = head->prev;
        }
        struct ShortQuestion* sQ = list_entry(head, struct ShortQuestion, list);
        show_shortquestion(sQ);
        printf("-1-上一题              2-返回                3-下一题-\n");
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else break;
    }
    return select;
}

int manage_shortquestion(){
    struct list_head* head = &shortHead->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        printf("----------------------填空题-------------------------\n");
        if(i==0) {
            head = head->next;
            if(head == &shortHead->list) head = head->next;
        }
        if(i==1) {
            head = head->prev;
            if(head == &shortHead->list) head = head->prev;
        }
        struct ShortQuestion* sQ = list_entry(head, struct ShortQuestion, list);
        
        show_shortquestion(sQ);
        printf("-1-上一题    2-返回    4-添加     5-删除     3-下一题-\n");   
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else if(select == 2) break;
        else if(select == 4) insert_shortquestion(sQ);
        else if(select == 5) {
            struct ShortQuestion* p = list_entry(head, struct ShortQuestion, list); 
            head = head->next;
            delete_shortquestion(p);
        }
        else {
            printf("输入错误\n");
            break;
        }
    }
    save_sfile();
    return select;
}


void load_pfile(){
    FILE *file = fopen("./pquestion.txt", "r");
    if(file == NULL) return;
    char *p = NULL;
    char linebuffer[512] = {0};
    while(p = fgets(linebuffer, 512, file)){    //fgets会读取文件的每一行，
                                                 //把内容存入linebuffer。一旦读取成功，就会执行循环体
        struct ProgrammingQuestion* node = malloc(sizeof(struct ProgrammingQuestion));
        if(node == NULL) return;
        INIT_LIST_HEAD(&node->list);
        sscanf(linebuffer,
            "%[^,],%[^,]",
            node->Question, 
            node->answer);
        //把node插入到head后面
        list_add(&node->list, &programmingHead->list);
    }
    fclose(file);
}


void save_pfile(){
        //打开文件， 写文件内容（一行一行读取）， 关闭文件
    FILE *file = fopen("./pquestion.txt","w") ;//r只读， w只写（会清空文件）， a+(追加读写)，  w+读写（会清空文件）, r+读写
    if(file == NULL) return ;

    struct list_head  *head = &programmingHead->list;
    struct list_head  *p = head->next;
    while(p != head){
        //根据链表指针head获取数据指针
        struct ProgrammingQuestion* pqhead = list_entry(p, struct ProgrammingQuestion, list);
        char linebuffer[512] = {0};
        int bytes_written = snprintf(linebuffer, sizeof(linebuffer), 
            "%s,%s\n", 
            pqhead->Question, 
            pqhead->answer);
        if (bytes_written >= sizeof(linebuffer)) {
        // 处理截断情况
            fprintf(stderr, "警告: 数据被截断，可能不完整\n");
        }
        //写入文件
        fwrite(linebuffer, 1, strlen(linebuffer), file);//（要写入的数据，一个元素的大小， 多少个元素， 要写入的文件）
        p = p->next;
    }
    fclose(file);//关闭文件
}


struct ProgrammingQuestion* create_programmingquestion_head(){
    if(programmingHead != NULL) return programmingHead;
    programmingHead = malloc(sizeof(struct ProgrammingQuestion));
    if(programmingHead == NULL) return NULL;
    INIT_LIST_HEAD(&programmingHead->list);
    return programmingHead;
}

void insert_programmingquestion(struct ProgrammingQuestion* head){
    if(head == NULL) return;
    struct ProgrammingQuestion* node = malloc(sizeof(struct ProgrammingQuestion));
    if(node == NULL) return;
    INIT_LIST_HEAD(&node->list);

    printf("请输入题干:\n"); scanf("%s[^\n]", node->Question);
    while(getchar() != '\n');
    printf("请输入答案:\n"); scanf("%s[^\n]", node->answer);

    list_add(&node->list, &head->list);
}
void delete_programmingquestion(struct ProgrammingQuestion* node){
    if(node == NULL) return;
    list_del(&node->list);
    free(node);
}
void show_programmingquestion(struct ProgrammingQuestion* node){
    if(node == NULL) return;
    printf("题目: %s\n", node->Question);
    printf("解答: %s\n", node->answer);
}
int view_programmingquestion(){
    struct list_head* head = &programmingHead->list;
    int i = 0;
    int select = 0;

    while(1){
        system("clear");

        printf("----------------------填空题-------------------------\n");
        if(i == 0){
            head = head->next;
            if(head == &programmingHead->list) head = head->next;
        }
        if(i == 1){
            head = head->prev;
            if(head == &programmingHead->list) head = head->prev;
        }
        struct ProgrammingQuestion* sQ = list_entry(head, struct ProgrammingQuestion, list);
        show_programmingquestion(sQ);
        printf("-1-上一题              2-返回                3-下一题-\n");
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else break;
    }
    return select;
}

int manage_programmingquestion(){
    struct list_head* head = &programmingHead->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        printf("----------------------填空题-------------------------\n");
        if(i==0) {
            head = head->next;
            if(head == &programmingHead->list) head = head->next;
        }
        if(i==1) {
            head = head->prev;
            if(head == &programmingHead->list) head = head->prev;
        }
        struct ProgrammingQuestion* sQ = list_entry(head, struct ProgrammingQuestion, list);
        
        show_programmingquestion(sQ);
        printf("-1-上一题    2-返回    4-添加     5-删除     3-下一题-\n");   
        printf("请输入:\n");
        scanf("%d", &select);
        if(select == 1) i = 0;
        else if(select == 3) i = 1;
        else if(select == 2) break;
        else if(select == 4) insert_programmingquestion(sQ);
        else if(select == 5) {
            struct ProgrammingQuestion* p = list_entry(head, struct ProgrammingQuestion, list); 
            head = head->next;
            delete_programmingquestion(p);
        }
        else {
            printf("输入错误\n");
            break;
        }
    }
    save_pfile();
    return select;
}


void show_choicepratice(struct choiceQuestion* node){
    if(node == NULL) return;
    printf("------------------选择题--------------------\n");
    printf("%s\n", node->Question);
    printf("A.%s\n", node->Option[0]);
    printf("B.%s\n", node->Option[1]);
    printf("C.%s\n", node->Option[2]);
    printf("D.%s\n", node->Option[3]);
    printf("-1-上一题------2-返回------3-作答-------3-下一题-\n");
}

void show_blankpratice(struct BlankQuestion* node){
    if(node == NULL) return;
    printf("------------------选择题--------------------\n");
    printf("%s ______\n", node->Question);
    
    printf("-1-上一题------2-返回------3-作答-------3-下一题-\n");
}



void choicepratice(){
    struct list_head* head = &choiceQuestionList->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        if(i == 0) {
        head = head->next;
        if(head == &choiceQuestionList->list) head = head->next;
        }
        if(i == 1){
            head = head->prev;
            if(head == &choiceQuestionList->list) head = head->prev;
        }

        struct choiceQuestion* cQ = list_entry(head, struct choiceQuestion, list);
        show_choicepratice(cQ);
    
        printf("请输入操作:\n");
        scanf("%d", &select);
        while(getchar() != '\n');
        if(select == 1) i = 1;
        if(select == 4) i = 0;
        if(select == 3){
            char asw;
            printf("请输入:\n");
            scanf("%c", &asw);
            if((asw-'A') == cQ->answer){
                printf("正确\n");
                i = 0;
            } 
            else {
                printf("错误\n");
                i = 0;
            }    
        }
        delay_ms(1000);  // 延时500毫秒
        if(select == 2) break;
    }
}

void blankpratice(){
    struct list_head* head = &blankHead->list;

    int i = 0;
    int select = 0;
    while(1){
        system("clear");
        if(i == 0) {
        head = head->next;
        if(head == &blankHead->list) head = head->next;
        }
        if(i == 1){
            head = head->prev;
            if(head == &blankHead->list) head = head->prev;
        }

        struct BlankQuestion* bQ = list_entry(head, struct BlankQuestion, list);
        show_blankpratice(bQ);
    
        printf("请输入操作:\n");
        scanf("%d", &select);
        while(getchar() != '\n');
        if(select == 1) i = 1;
        if(select == 4) i = 0;
        if(select == 3){
            char asw[128];
            printf("请输入:\n");
            scanf("%s", asw);
            printf("%s\n", bQ->answer);
            i = 0;   
        }
        delay_ms(1000);
        if(select == 2) break;
    }
}