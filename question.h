#ifndef QUESTION_H
#define QUESTION_H
#include "list.h"

typedef struct choiceQuestion{
    char Question[512];
    char Option[4][256];
    int answer;
    struct list_head list;
}ChoiceQuestion;

struct JudgeQuestion{
    char Question[512];
    int answer;
    struct list_head list;
};

struct BlankQuestion{
    char Question[512];
    char answer[128];
    struct list_head list;
};

struct ShortQuestion{
    char Question[512];
    char answer[512];
    struct list_head list;
};

struct ProgrammingQuestion{
    char Question[512];
    char answer[512];
    struct list_head list;
};

struct choiceQuestion* create_choicequestion_head();
void insert_choicequestion(struct choiceQuestion* head);
void delete_choicequestion(struct choiceQuestion* node);
void search_choicequestion(struct choiceQuestion* node);
void modify_choicequestion(struct choiceQuestion* node);
void show_choicequestion(struct choiceQuestion* node);
void show_choicepratice(struct choiceQuestion* node);
void choicepratice();

struct JudgeQuestion* create_judgequestion_head();
void insert_judgequestion(struct JudgeQuestion* head);
void delete_judgequestion(struct JudgeQuestion* node);
void show_judgequestion(struct JudgeQuestion* node);

struct BlankQuestion* create_blankquestion_head();
void insert_blankquestion(struct BlankQuestion* head);
void delete_blankquestion(struct BlankQuestion* node);
void show_blankquestion(struct BlankQuestion* node);

struct ShortQuestion* create_shortquestion_head();
void insert_shortquestion(struct ShortQuestion* head);
void delete_shortquestion(struct ShortQuestion* node);
void show_shortquestion(struct ShortQuestion* node);

struct ProgrammingQuestion* create_programmingquestion_head();
void insert_programmingquestion(struct ProgrammingQuestion* head);
void delete_programmingquestion(struct ProgrammingQuestion* node);
void show_programmingquestion(struct ProgrammingQuestion* node);

void load_file();
void save_file();
int view_choicequestion();
int manage_choicequestion();

void load_jfile();
void save_jfile();
int view_judgequestion();
int manage_judgequestion();

void load_bfile();
void save_bfile();
int view_blankquestion();
int manage_blankquestion();

void load_sfile();
void save_sfile();
int view_shortquestion();
int manage_shortquestion();

void load_pfile();
void save_pfile();
int view_programmingquestion();
int manage_programmingquestion();

#endif // QUESTION_H