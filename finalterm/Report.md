# Data Structure Finalterm Assignment

###### 2017920044 이관희
<hr> 

### Code

```c
// LRU의 hit-ratio를 출력하는 프로그램
// 탐색 시 Hash table 사용

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
```

### 코드 설명

소스코드에 주석으로 매겨진 번호 순서에 따라 코드 설명을 진행하고, `hash table`을 사용하기 위해 추가된 부분에 대한 설명만 작성.

1. __라이브러리__ <br>
프로그램 실행 시간을 측정하기 위한 time.h를 추가하였다.

2. __매크로__ <br>
HASH_SIZE를 6151로 선언하였다. 8192의 75%는 6144인데, 6144를 그대로 사용하기보다는 그 주위 소수를 사용하는 것이 프로그램 성능에 도움이 될 것이라 생각했기 때문이다.

3. __typedef__ <br>
`hash table`을 유지하기 위한 포인터인 `hash_next`, `hash_prev`를 기존의 `struct buffer`에 추가했다.

4. __전역변수__ <br>
`hash_table`배열을 선언했다. 이 배열의 원소는 `buffer`이다. 각 원소는 이중연결리스트(chaining)의 헤드노드 역할을 한다. 또한, 중간고사 때 제출한 lru코드에는 hit_num, data_num, count_block 변수들을 전역변수로 선언하였는데, `main`에서만 사용하는 변수를 전역변수로 선언할 필요는 없다고 판단하여 `main`의 지역변수로 자리를 옮겼다(전역변수로 선언하면 참조하는 데에 더 많은 시간이 들 것이라 생각했다).

5. __함수 프로토타입__ <br>
`hash function`에 해당하는 함수 `get_value`를 새로 정의했기 때문에, `get_value`에 대한 프로토타입을 선언했다.

6. __main__ <br>
`main`에 추가된 것들은 전역변수였던 hit_num, data_num, count_block변수, 프로그램 실행 시간을 출력하기 위한 start, stop변수, 실행 시간을 출력하는 `print()`함수 이외에는 없다. 왜냐하면 `hash_table`를 유지하는 코드를 기존의 `delete()`, `insert()`, `move()`함수에 추가했기 때문이다. 가독성이나 모듈화를 위해 `hash table` 유지에 사용되는 함수들을 따로 선언할까 고민도 했지만, `lrulist`를 유지하기 위해 작성한 코드와 `hash table`을 유지하기 위해 작성할 코드가 거의 유사하기 때문에, 불필요한 함수 호출을 추가하는 것보다는 기존의 함수들에 작성할 코드를 합치는 것이 좋다고 판단했다. 

7. __함수 정의__ <br>

- __get_value()__ <br>
인자로 `key`값을 받아 modulo연산을 수행한 후 `value`를 리턴하는 함수이다.
- __char_to_element()__ <br>
입력받은 문자열을 element 형으로 변환하는 함수.
- __isthere()__ <br>
parameter로 받은 data가 `hash_table`에 존재하는지 확인하는 함수.
- __init()__ <br>
기존 `lrulist` 초기화와, `hash_table` 초기화를 같이 수행하는 함수.
- __delete()__ <br>
기존의 `lrulist`에서 했던 것처럼, 원형이중연결리스트에서 특정 노드를 제거하는 동작을 똑같이 수행하는데, 포인터가 `hash_next`, `hash_prev`로 바뀌었다.
- __insert()__ <br>
`get_value()`를 호출하여 `value`를 리턴받는다. 해당 `value`는 `hash_table`의 인덱스이므로, `hash_table[value]`를 이용하여 해당 원형이중연결리스트에 노드를 연결한다.
- __move()__ <br>
이 함수의 역할은 __LRU__ 의 교체정책에 따라, 참조된 노드를 리스트 맨 앞으로 옮기는 함수이다. `hash_table`에 연결되어있는 리스트에도 이러한 동작이 필요할까 고민했지만, `MAX_CACHE_SIZE`보다 `HASH_SIZE`가 유의미하게 작다면 이러한 동작이 성능 향상에 도움이 된다고 생각하여 추가하였다. 기존 `lrulist` 코드와 다른 점은 헤드노드가 `hash_table[value]`로 바뀌었다는 것 외에는 없다.

### 실행 결과

#### hash table 사용 전

>Let's start simulation!<br>
>Hit-ratio of LRU : 88.446525%<br>
>Runtime without Hashing : 16.329000sec<br>

#### hash table 사용 후

>Let's start simulation!<br>
>Hit-ratio of LRU : 88.446525%<br>
>Runtime with Hashing : 0.219000sec<br>

### 결론

기존의 `lrulist`를 순회하며 데이터가 캐시에 존재하는지 확인하는 방법은 매우 비효율적인 방법이었다. 실행 시간이 이렇게까지 차이가 날 줄은 몰랐다. 따라서 `hash_table`을 선언할 수 있는 메모리의 여유가 된다면, `hash_table`을 사용하는 것이 합리적일 것이다.