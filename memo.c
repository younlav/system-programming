#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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

// 메시지를 전송하는 함수
void send_message(const char *message) {
    key_t key;
    int msqid;
    struct msgbuf msg;

    // 메시지 큐 키 생성
    key = ftok(SHM_FILE, 1);
    // 메시지 큐 식별자 획득
    msqid = msgget(key, 0666);

    // 메시지 내용 설정
    msg.mtype = 1;
    strncpy(msg.mtext, message, SHM_SIZE);
    // 메시지 큐에 메시지 전송
    msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
}

int main() {
    key_t key;
    int shmid;
    void *shmaddr;
    char buf[SHM_SIZE];

    // 공유 메모리 키 생성
    key = ftok(SHM_FILE, 1);
    // 공유 메모리 식별자 획득
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    // 공유 메모리 연결
    shmaddr = shmat(shmid, NULL, 0);

    // 현재 IPC 상태 출력
    system("ipcs");

    // 사용자로부터 메모 입력 받기
    printf("메모를 입력하세요 >> ");
    printf("\n\n****************************\n\t");
    fgets(buf, SHM_SIZE, stdin);
    printf("\n****************************\n\n");

    // 입력 받은 메모를 공유 메모리에 저장
    strncpy(shmaddr, buf, SHM_SIZE);
    // 메시지 큐에 메모 전송
    send_message(buf);

    // 공유 메모리 분리
    shmdt(shmaddr);

    return 0;
}
