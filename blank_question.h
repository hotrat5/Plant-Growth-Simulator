#ifndef BLANK_QUESTION_H
#define BLANK_QUESTION_H

#include "list.h"

struct BlankQuestion{
    char Question[512];
    char answer[64];
    struct list_head list;
};

struct BlankQuestion* create_blankquestion_head();
void insert_blankquestion(struct BlankQuestion* head);
void delete_blankquestion(struct BlankQuestion* node);
void show_blankquestion(struct BlankQuestion* node);

void load_bfile();
void save_bfile();
int view_blankquestion();
int manage_blankquestion();

#endif