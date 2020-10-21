// Chapter 7. Doubly LinkedList
// Test Doubly LinkedList run well

#include <stdio.h>
#include <stdlib.h>

typedef int element;
typedef struct DListNode
{
    element data;
    struct DListNode *next;
    struct DListNode *prev;
} DListNode;

void init(DListNode *head);
void dinsert(DListNode *before, element data);
void ddelete(DListNode *head, DListNode *removed);
void print_dlist(DListNode *head);
DListNode* search(DListNode *head, element data);

int main(void)
{
    DListNode *head = (DListNode *)malloc(sizeof(DListNode *));
    element data;

    init(head);

    printf("\nADDING\n");
    for(int i = 0; i < 5; i++)
    {
        dinsert(head, i);
        print_dlist(head);
    }

    printf("\nInput integer : ");
    scanf("%d", &data);
    if(search(head, data))
        printf("%d is in the list.\n", data);
    else
        printf("%d is not in the list.\n", data);

    printf("\nSUBTRACTING\n");
    for(int i = 0; i < 5; i++)
    {
        print_dlist(head);
        ddelete(head, head->next);
    }

    return 0;
}

void init(DListNode *head)
{
    head->next = head;
    head->prev = head;
}

void dinsert(DListNode *before, element data)
{
    DListNode *node = (DListNode *)malloc(sizeof(DListNode));

    node->data = data;
    node->next = before->next;
    node->prev = before;
    before->next->prev = node;
    before->next = node;
}

void ddelete(DListNode *head, DListNode *removed)
{
    if(removed == head) return;
    removed->prev->next = removed->next;
    removed->next->prev = removed->prev;
    free(removed);
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