// FIFO의 hit-ratio를 출력하는 프로그램

// 1.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
buffer fifolist;
double hit_num;
double data_num;
// 삽입, 삭제할 cache_buffer의 offset추적
int pointer;
// cache_buffer가 가득 찼는지 판단
int count_block;

// 5.
// 함수 프로토타입
element char_to_element(char*);
bool isthere(element);
void init(buffer*);
void delete(buffer*);
void insert(element);

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
    char buf[MAX_DATA_LENGTH];
    
    init(&fifolist);
    for(char *p = fgets(buf, MAX_DATA_LENGTH, fp); p != NULL; p = fgets(buf, MAX_DATA_LENGTH, fp))
    {
        data_num += 1.0;
        element data = char_to_element(buf);
        bool check = isthere(data);
        if(!check)
        {
            if(count_block == MAX_CACHE_SIZE)
            {
                delete(fifolist.next);
                insert(data);
            }
            else
            {
                insert(data);
                count_block++;
            }
            if(++pointer == MAX_CACHE_SIZE)
                pointer = 0;
        }
        else
            hit_num += 1.0;
    }

    printf("Hit-ratio of FIFO : %f%%", (hit_num / data_num) * 100.0);

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

bool isthere(element data)
{
    for(buffer *p = fifolist.prev; p != &fifolist; p = p->prev)
        if(p->blkno == data)
            return true;
    return false;
}

void init(buffer *fifolist)
{
    fifolist->next = fifolist;
    fifolist->prev = fifolist;
}

void delete(buffer *removed)
{
    removed->prev->next = removed->next;
    removed->next->prev = removed->prev;
}

void insert(element data)
{
    buffer *newnode = cache_buffer + pointer;

    newnode->blkno = data;
    newnode->next = &fifolist;
    newnode->prev = fifolist.prev;
    fifolist.prev->next = newnode;
    fifolist.prev = newnode;
}