// Data Structure Assignment
// Coding Hanoi-Tower by Recursion
// 2017920044 이관희

#include <stdio.h>

// 함수 프로토타입
void hanoi_tower(int, char, char, char);

// 전역변수 선언
int move_number = 0;

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

// 하노이탑을 구현하는 함수
void hanoi_tower(int n, char from, char temp, char to)
{
    move_number++;

    if(n == 1)
        // 재귀함수 마지막 호출

        // (n == 1)일 때, 남은 원판은 제일 위에 있는 원판 '1' 이다
        printf("원판 1을 %c에서 %c로 옮긴다.\n", from, to);
    else
    {
        // (n >= 2)일 때의 실행

        // 'n - 1'개의 원판을 'A' -> 'B'로 옮긴다
        hanoi_tower(n - 1, from, to, temp);
        // n에 해당하는 원판을 (from -> to) 로 옮긴다
        printf("원판 %d을 %c에서 %c로 옮긴다.\n", n, from, to);
        // 'n - 1'개의 원판을 'B' -> 'C'로 옮긴다
        hanoi_tower(n - 1, temp, from, to);
    }
}