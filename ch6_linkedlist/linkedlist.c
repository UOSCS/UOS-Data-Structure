#include <stdio.h>
#include <stdlib.h>

typedef int element;
typedef struct ListNode
{
    element data;
    struct ListNode *link;
} ListNode;

void error(char *message)
{
    printf("%s\n", message);
    exit(1);
}

ListNode *insert_first(ListNode *head, element value)
{
    ListNode *p = (ListNode *)malloc(sizeof(ListNode));

    p->data = value;
    p->link = head;
    head = p;

    return head;
}

ListNode *insert(ListNode *head, ListNode *pre, element value)
{
    ListNode *p = (ListNode *)malloc(sizeof(ListNode));

    p->data = value;
    p->link = pre->link;
    pre->link = p;

    return head;
}

ListNode *insert_sorted(ListNode *head, element value)
{
    if(head == NULL)
        return insert_first(head, value);
    else
    {
        ListNode *p = (ListNode *)malloc(sizeof(ListNode));
        ListNode *previous = NULL;

        for(ListNode *search = head; search; search = search->link)
        {
            if(value > search->data)
                previous = search;
            else
            {
                p->data = value;
                p->link = search;
                if(previous == NULL)
                    head = p;
                else
                    previous->link = p;

                return head;
            }
        }
        return insert(head, previous, value);
    }
}

// Professor's code
// Error : when Linkedlist has 1 element.

// ListNode *insert_sorted(ListNode *head, element value)
// {
//     ListNode *p = (ListNode *)malloc(sizeof(ListNode));
//     ListNode *cur;

//     p->data = value;
//     p->link = NULL;

//     if(head == NULL)
//     {
//         head = p;
//         return head;
//     }

//     cur = head;
//     while(cur->link != NULL && cur->link->data < p->data)
//         cur = cur->link;

//     p->link = cur->link;
//     cur->link = p;

//     return head;
// }

ListNode *delete_first(ListNode *head)
{
    ListNode *removed = head;

    if(head == NULL)
        return NULL;

    head = removed->link;
    free(removed);

    return head;
}

ListNode *delete(ListNode *head, ListNode *pre)
{
    ListNode *removed = pre->link;

    pre->link = removed->link;
    free(removed);
    
    return head;
}

void print_list(ListNode *head)
{
    for(ListNode *p = head; p; p = p->link)
        printf("%d->", p->data);
    printf("NULL\n");
}

int main(void)
{
    ListNode *head = NULL;

    // for(int i = 0; i < 10; i++)
    // {
    //     head = insert_sorted(head, i);
    //     print_list(head);
    // }
    
    head = insert_sorted(head, 3);
    print_list(head);
    head = insert_sorted(head, 1);
    print_list(head);
    head = insert_sorted(head, 9);
    print_list(head);
    head = insert_sorted(head, 4);
    print_list(head);
    head = insert_sorted(head, 8);
    print_list(head);
    head = insert_sorted(head, 7);
    print_list(head);
    head = insert_sorted(head, 6);
    print_list(head);

    for(int i = 0; i < 5; i++)
    {
        head = delete_first(head);
        print_list(head);
    }

    return 0;
}