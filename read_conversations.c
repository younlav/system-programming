#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// 상수 정의
#define SHM_SIZE 1024  // 공유 메모리 크기
#define SHM_FILE "shmfile"  // 공유 메모리 파일 이름

// 메시지 구조체 정의
struct msgbuf {
    long mtype;  // 메시지 유형
    char mtext[SHM_SIZE];  // 메시지 내용
};

// 대화를 표시하는 함수
void view_conversations() {
    key_t key;
    int msqid;
    struct msgbuf msg;
    int count = 0;

    // 메시지 큐 키 생성
    key = ftok(SHM_FILE, 1);
    // 메시지 큐 식별자 획득
    msqid = msgget(key, 0666);

    // 메시지 큐에서 메시지를 읽어와 출력
    while (msgrcv(msqid, &msg, sizeof(msg.mtext), 0, IPC_NOWAIT) != -1) {
        printf("<<<Message>> %s\n", msg.mtext);
        count++;
    }

    // 총 메시지 수 출력
    printf("총 메모 수 :  %d\n", count);
}

// 메인 함수
int main() {
    view_conversations();  // 대화 내용 보기 함수 호출
    return 0;  // 프로그램 종료
}
