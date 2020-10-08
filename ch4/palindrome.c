// 2020-09-23 Data Structure
// Checking the input string is palindrome
// 2017920044 이관희
// 미완성


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 100

typedef int element;
typedef struct
{
    element data[MAX_STACK_SIZE];
    int top;
} StackType;

void init_stack(StackType* s)
{
    s->top = -1;
}

int is_empty(StackType* s)
{
    return (s->top == -1);
}

int is_full(StackType* s)
{
    return (s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType* s, element item)
{
    if(is_full(s))
    {
        printf("Overflow!!");
        exit(1);
    }
    else
        s->data[++(s->top)] = item;
}

element pop(StackType* s)
{
    if(is_empty(s))
    {
        printf("Underflow!!");
        exit(1);
    }
    else
        return (s->data[(s->top)--]);
}

element peek(StackType* s)
{
    if(is_empty(s))
    {
        printf("Underflow!!");
        exit(1);
    }
    else
        return (s->data[(s->top)]);
}

int size(StackType* s)
{
    return s->top + 1;
}

// Mine

void is_palindrome(char* word)
{
    StackType* s;
    StackType s1;
    int length = strlen(word);
    s = &s1;
    
    init_stack(s);
    for(int i = 0; i < (length / 2); i++)
    {
        if(65 <= *(word + i) && *(word + i) <= 90)
            *(word + i) += 32;
        else if(*(word + i) > 122 && *(word + i) < 97)
            continue;

        push(s, *(word + i));
    }

    for(int i = (length % 2) ? (length / 2) + 1 : (length / 2); i < length; i++)
    {
        if(65 <= *(word + i) && *(word + i) <= 90)
            *(word + i) += 32;
        else if(*(word + i) > 122 && *(word + i) < 97)
            ;

        if(pop(s) != *(word + i))
        {
            printf("회문이 아닙니다.");
            return;
        }
    }
    printf("회문입니다.");
}

int main(void)
{
    char word[MAX_STACK_SIZE];

    printf("문자열을 입력하세요 : ");
    scanf("%s", word);

    is_palindrome(word);

    return 0;
}


// Code by Professor

// int main(void)
// {
//     int i;
//     char line[128];
//     StackType s;

//     init_stack(&s);
//     printf("input string\n");
//     gets_s(line, 128);

//     for(i = 0; i < strlen(line) / 2; i++)
//         push(&s, line[i]);

//     for(i = (int)((strlen(line) / 2.0) + 0.5); i < strlen(line); i++)
//     {
//         if(pop(&s) != line[i])
//         {
//             printf("No\n");
//             return -1;
//         }
//     }
//     printf("Yes\n");

//     return 0;
// }

// int main()
// {
//     StackType s;
//     char word[100];

//     bool is_true = true;

//     init_stack(&s);
//     printf("input string\n");
//     gets_s(word,100);
//     for (int i = 0; i < strlen(word) / 2; i++)
//         push(&s, word[i]);

//     for (int i = (int)((strlen(word) / 2.0) + 0.5); i < strlen(word); i++)
//     {
//         if (pop(&s) != word[i])
//         {
//             printf("no\n");
//             is_true = false;
//             return 0;
//         }

//     }
//     if (is_true == true)
//         printf("yes");
    
//     return 0;
// }