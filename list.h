#ifndef _LINUX_LIST_
#define _LINUX_LIST_

#include <stdbool.h>
#include <stdio.h>

//链表结构体
struct list_head {
    struct list_head *next, *prev;
};


#define offsetof(type, member)  \
        (char*)(&((type*)0)->member)
#define container_of(ptr, type, member) \
        ({  ((type *)((char*)ptr - offsetof(type, member))); })


//通过链表指针计算出数据结构体指针
/*
* @ptr:        链表指针.
* @type:       数据结构体类型
* @member:     链表结构体在数组结构体中定义成员名称.

*/
#define list_entry(ptr, type, member)   \
        container_of(ptr, type, member)


#define LIST_HEAD_INIT(name)  { &(name), &(name) }


#define LIST_HEAD(name)   struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    if(list == NULL) return;
    list->next=list;
    list->prev=list;
}

//插入节点
static inline void __list_add(struct list_head *new,
                        struct list_head *prev,  
                        struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next= new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
        __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
        __list_add(new, head->prev, head);
}

//删除节点
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
        next->prev = prev;
        prev->next = next;
}


static inline void __list_del_entry(struct list_head *entry)
{
        if(entry == NULL) return ;
        __list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
        __list_del_entry(entry);
        entry->next = entry;
        entry->prev = entry;
}


#define list_first_entry(ptr, type, member)   \
        list_entry((ptr)->next, type, member)

#define list_entry_is_head(pos, head, member)   \
        (&pos->member == (head))

#define list_next_entry(pos, member)  \
        list_entry((pos)->member.next, typeof(*(pos)), member)

//遍历--不能删除节点
/**
 * list_for_each_entry  -       iterate over list of given type
 * @pos:        the type * to use as a loop cursor.
 * @head:       the head for your list.
 * @member:     the name of the list_head within the struct.
 */
 #define list_for_each_entry(pos, head, member)                          \
 for (pos = list_first_entry(head, typeof(*pos), member);        \
      !list_entry_is_head(pos, head, member);                    \
      pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:        the type * to use as a loop cursor.
 * @n:          another type * to use as temporary storage
 * @head:       the head for your list.
 * @member:     the name of the list_head within the struct.
 */
 #define list_for_each_entry_safe(pos, n, head, member)                  \
 for (pos = list_first_entry(head, typeof(*pos), member),        \
         n = list_next_entry(pos, member);                       \
      !list_entry_is_head(pos, head, member);                    \
      pos = n, n = list_next_entry(n, member))



#endif
