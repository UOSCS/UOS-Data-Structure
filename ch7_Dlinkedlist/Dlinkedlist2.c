// Chapter 7. Doubly LinkedList
// Using predefined size of array of nodes
// Incompletion

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// SIZE = max # of nodes, NUM = # of iterations in loop of main
#define SIZE 8
#define NUM 8

typedef int element;
typedef struct DListNode
{
    element data;
    struct DListNode *next;
    struct DListNode *prev;
} DListNode;

DListNode nodes[SIZE];
bool flag[SIZE] = {false};

int find_empty_index();
void init(DListNode *head);
void dinsert(DListNode *before, element data);
void ddelete(DListNode *head, DListNode *removed);
void print_dlist(DListNode *head);
DListNode* search(DListNode *head, element data);

int main(void)
{
    int index = find_empty_index();
    flag[index] = true;
    DListNode *head = nodes + index;
    element data;

    init(head);

    printf("\nADDING\n");
    for(int i = 0; i < NUM - 1; i++)
    {
        dinsert(head, i);
        print_dlist(head);
    }

    ddelete(head, head->next->next->next);
    print_dlist(head);
    dinsert(head, 9);
    print_dlist(head);
    ddelete(head, head->next->next);
    print_dlist(head);
    dinsert(head, 8);
    print_dlist(head);
    ddelete(head, head->next);
    print_dlist(head);

    printf("\nInput integer : ");
    scanf("%d", &data);
    if(search(head, data))
        printf("%d is in the list.\n", data);
    else
        printf("%d is not in the list.\n", data);

    printf("\nSUBTRACTING\n");
    for(int i = 0; i < NUM; i++)
    {
        print_dlist(head);
        ddelete(head, head->next);
    }

    return 0;
}

int find_empty_index()
{
    int i, half, quater;

    half = SIZE / 2;
    quater = half / 2;
    for(i = half; i >= 0; i--)
        if(!flag[i])
            return i;
    for(i = half + quater; i > half ; i--)
        if(!flag[i])
            return i;
    for(i = SIZE - 1; i > half + quater; i--)
        if(!flag[i])
            return i;
    return -1;
}

void init(DListNode *head)
{
    head->next = head;
    head->prev = head;
}

void dinsert(DListNode *before, element data)
{
    int index;
    if((index = find_empty_index()) == -1)
    {
        fprintf(stderr, "The List is full.\n");
        exit(1);
    }
    flag[index] = true;
    DListNode *node = nodes + index;

    node->data = data;
    node->next = before->next;
    node->prev = before;
    before->next->prev = node;
    before->next = node;
}

void ddelete(DListNode *head, DListNode *removed)
{
    if(removed == head)
    {
        fprintf(stderr, "The List is empty.\n");
        exit(1);
    }
    removed->prev->next = removed->next;
    removed->next->prev = removed->prev;
    flag[removed - nodes] = false;
}

void print_dlist(DListNode *head)
{
    for(DListNode *p = head->next; p != head; p = p->next)
        printf("<-| %d |->", p->data);
    printf("\n");
}

DListNode* search(DListNode *head, element data)
{
    for(DListNode *p = head->next; p != head; p = p->next)
        if(p->data == data)
            return p;
    return NULL;
}