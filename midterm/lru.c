// LRU의 hit-ratio를 출력하는 프로그램

// 1.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 2.
#define MAX_CACHE_SIZE 8192
#define TRACE_FILE_NAME "ref_stream.txt"
// unsigned long은 최대 10자리 + 마지막 '\n'문자
#define MAX_DATA_LENGTH 11

// 3.
typedef unsigned long element;
typedef struct buffer
{
    element blkno;
    struct buffer *next;
    struct buffer *prev;
} buffer;

// 4.
// 전역변수 선언
buffer cache_buffer[MAX_CACHE_SIZE];
buffer lrulist;
double hit_num;
double data_num;
// cache_buffer가 가득 찼는지 판단
int count_block;

// 5.
// 함수 프로토타입
element char_to_element(char*);
buffer* isthere(element);
void init(buffer*);
int delete(buffer*);
void insert(element, int);
void move(buffer*);

// 6.
int main(int argc, char *argv[])
{
    // 파일이 정상적으로 열리는지 확인
    FILE *fp;

    if((fp = fopen(TRACE_FILE_NAME, "r")) == NULL)
    {
        fprintf(stderr, "File open error!\n");
        exit(1);
    }
    else
        printf("Let's start simulation!\n");

    // 시뮬레이션 시작
    double start, stop;
    char buf[MAX_DATA_LENGTH];

    init(&lrulist);

    start = clock();
    for(char *p = fgets(buf, MAX_DATA_LENGTH, fp); p != NULL; p = fgets(buf, MAX_DATA_LENGTH, fp))
    {
        data_num += 1.0;
        element data = char_to_element(buf);
        buffer *check = isthere(data);
        if(check == NULL)
        {
            if(count_block == MAX_CACHE_SIZE)
            {
                int offset = delete(lrulist.prev);
                insert(data, offset);
            }
            else
            {
                insert(data, count_block);
                count_block++;
            }
        }
        else
        {
            hit_num += 1.0;
            move(check);
        }
    }
    stop = clock();

    printf("Hit-ratio of LRU : %f%%\n", (hit_num / data_num) * 100.0);
    printf("Time consuming without Hashing : %.2fsec\n", (double)(stop - start) / CLOCKS_PER_SEC);

    fclose(fp);

    return 0;
}

// 7.
element char_to_element(char *buf)
{
    element data = 0UL;

    for(int i = 0; i < MAX_DATA_LENGTH; i++)
        if(buf[i] == '\n')
            break;
        else
            data = data * 10UL + (element)(buf[i] - '0');

    return data;
}

buffer* isthere(element data)
{
    for(buffer *p = lrulist.next; p != &lrulist; p = p->next)
        if(p->blkno == data)
            return p;
    return NULL;
}

void init(buffer *lrulist)
{
    lrulist->next = lrulist;
    lrulist->prev = lrulist;
}

int delete(buffer *removed)
{
    removed->prev->next = removed->next;
    removed->next->prev = removed->prev;

    return removed - cache_buffer;
}

void insert(element data, int offset)
{
    buffer *newnode = cache_buffer + offset;

    newnode->blkno = data;
    newnode->next = lrulist.next;
    newnode->prev = &lrulist;
    lrulist.next->prev = newnode;
    lrulist.next = newnode;
}

void move(buffer *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;

    node->next = lrulist.next;
    node->prev = &lrulist;

    lrulist.next->prev = node;
    lrulist.next = node;
}