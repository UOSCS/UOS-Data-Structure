// Data Structure Assignment
// Coding Hanoi-Tower by Loop & Stack
// 2017920044 이관희

#include <stdio.h>

// stack배열의 크기
#define MAX 100

// 함수 프로토타입
void move(int, int, int);
void init_stack(void);
int push(int);
int pop(void);
int is_stack_empty(void);
void hanoi_tower(int, int, int, int);

// 전역변수 선언
int move_number = 0;
int stack[MAX];
int top;

int main(void)
{
    // 원판 개수를 저장하는 변수
    int n;

    // 원판 개수 사용자로부터 입력 받기
    printf("원판 개수를 입력하세요 : ");
    scanf("%d", &n);

    // 하노이탑 재귀함수 호출
    hanoi_tower(n, 'A', 'B', 'C');
    // 원판을 움직인 횟수 출력
    printf("원판을 %d번 옮기셨습니다.\n", move_number);

    // 프로그램 종료.
    return 0;
}

// "원반을 from에서 to로 옮긴다" 를 출력하는 함수
void move(int n, int from, int to)
{
    printf("원판 %d을 %c에서 %c로 옮긴다.\n", n, from, to);
    move_number++;
}

// 스택을 초기화하는 함수
void init_stack(void)
{
    top = -1;
}

// 스택 push 함수
int push(int t)
{
    if(top >= MAX - 1)
    {
        printf("\nStack overflow");
        return -1;
    }
    stack[++top] = t;

    return t;
}

// 스택 pop 함수
int pop(void)
{
    if(top < 0)
    {
        printf("\nStack underflow");
        return -1;
    }
    return stack[top--];
}

// 스택이 비었는지 확인하는 함수
int is_stack_empty()
{
    return (top > -1) ? 0 : 1;
}

// 하노이탑을 구현하는 함수
void hanoi_tower(int n, int from, int temp, int to)
{
    init_stack();

    // move_number++;
    // if(n == 1)
    // printf("원판 1을 %c에서 %c로 옮긴다.\n", from, to);
    // else
    // {
    //     hanoi_tower(n - 1, from, to, temp);
    //     printf("원판 %d을 %c에서 %c로 옮긴다.\n", n, from, to);
    //     hanoi_tower(n - 1, temp, from, to);
    // }

    // 위에 주석처리된 코드를 loop & stack으로 구현한 코드
    // 재귀함수의 호출과 리턴 과정을 추적하여 stack에 쌓는다
    while(1)
    {
        while(n > 1)
        {
            push(to);
            push(temp);
            push(from);
            push(n);
            n--;
            push(to);
            to = temp;
            temp = pop();
        }

        printf("원판 1을 %c에서 %c로 옮긴다.\n", from, to);
        move_number++;

        if(!is_stack_empty())
        {
            n = pop();
            from = pop();
            temp = pop();
            to = pop();

            move(n, from, to);

            n--;
            push(from);
            from = temp;
            temp = pop();
        }
        else
            break;
    }
}