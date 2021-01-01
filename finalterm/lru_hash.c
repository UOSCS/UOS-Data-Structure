// LRU의 hit-ratio를 출력하는 프로그램

// 1.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 2.
#define MAX_CACHE_SIZE 8192
#define HASH_SIZE 6151
#define TRACE_FILE_NAME "ref_stream.txt"
// unsigned long은 최대 10자리 + 마지막 '\n'문자
#define MAX_DATA_LENGTH 11

// 3.
typedef unsigned long element;
typedef struct buffer
{
    element blkno;
    struct buffer *next, *prev;
    struct buffer *hash_next, *hash_prev;
} buffer;

// 4.
// 전역변수 선언
buffer cache_buffer[MAX_CACHE_SIZE];
buffer hash_table[HASH_SIZE];
buffer lrulist;

// 5.
// 함수 프로토타입
int get_value(element);
element char_to_element(char*);
buffer* isthere(element);
void init();
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
    int hit_num = 0;
    int data_num = 0;
    int count_block = 0;

    init();

    start = clock();
    for(char *p = fgets(buf, MAX_DATA_LENGTH, fp); p != NULL; p = fgets(buf, MAX_DATA_LENGTH, fp))
    {
        data_num++;
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
            hit_num++;
            move(check);
        }
    }
    stop = clock();

    printf("Hit-ratio of LRU : %f%%\n", ((double)hit_num / (double)data_num) * 100.0);
    printf("Runtime with Hashing : %fsec\n", (stop - start) / CLOCKS_PER_SEC);

    fclose(fp);

    return 0;
}

// 7.
int get_value(element key)
{
    return key % HASH_SIZE;
}

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
    int value = get_value(data);

    for(buffer *p = hash_table[value].hash_next; p != &hash_table[value]; p = p->hash_next)
        if(p->blkno == data)
            return p;
    return NULL;
}

void init()
{
    lrulist.next = &lrulist;
    lrulist.prev = &lrulist;

    for(int i = 0; i < HASH_SIZE; i++)
    {
        hash_table[i].hash_next = &hash_table[i];
        hash_table[i].hash_prev = &hash_table[i];
    }
}

int delete(buffer *removed)
{
    removed->prev->next = removed->next;
    removed->next->prev = removed->prev;

    removed->hash_prev->hash_next = removed->hash_next;
    removed->hash_next->hash_prev = removed->hash_prev;

    return removed - cache_buffer;
}

void insert(element data, int index)
{
    buffer *newnode = cache_buffer + index;
    int value = get_value(data);

    newnode->blkno = data;

    newnode->next = lrulist.next;
    newnode->prev = &lrulist;
    lrulist.next->prev = newnode;
    lrulist.next = newnode;

    newnode->hash_next = hash_table[value].hash_next;
    newnode->hash_prev = &hash_table[value];
    hash_table[value].hash_next->hash_prev = newnode;
    hash_table[value].hash_next = newnode;
}

void move(buffer *node)
{
    int value = get_value(node->blkno);

    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = lrulist.next;
    node->prev = &lrulist;
    lrulist.next->prev = node;
    lrulist.next = node;

    node->hash_prev->hash_next = node->hash_next;
    node->hash_next->hash_prev = node->hash_prev;
    node->hash_next = hash_table[value].hash_next;
    node->hash_prev = &hash_table[value];
    hash_table[value].hash_next->hash_prev = node;
    hash_table[value].hash_next = node;
}