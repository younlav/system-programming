#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

// 상수 정의
#define SHM_SIZE 1024          // 공유 메모리 크기
#define SHM_FILE "shmfile"     // 공유 메모리 파일 이름
#define LETTER_FILE "letter.dat"  // 편지 파일 이름

// 메모 기능을 실행하는 함수
void memo() {
    execl("./memo", "memo", NULL);
}

// 편지 작성 기능을 실행하는 함수
void write_letter() {
    execl("./write_letter", "write_letter", NULL);
}

// 편지 보내기 기능을 실행하는 함수
void send_letter() {
    execl("./send_letter", "send_letter", NULL);
}

// 공유 메모리 정리 함수
void cleanup_shared_memory() {
    key_t key;
    int shmid;
    void *shmaddr;

    // 공유 메모리 생성 및 연결
    key = ftok(SHM_FILE, 1);
    shmid = shmget(key, SHM_SIZE, 0666);
    shmaddr = shmat(shmid, NULL, 0);

    // 공유 메모리 내용 출력
    printf("\n\n****************************\n");
    printf("\t%s", (char *)shmaddr);
    printf("\n****************************\n\n");

    // 공유 메모리 초기화
    memset(shmaddr, 0, SHM_SIZE);

    // 공유 메모리 분리
    shmdt(shmaddr);
}

// 자식 프로세스 종료 처리 함수
void handle_child_exit() {
    int status;
    wait(&status);

    if (WIFEXITED(status)) {
        cleanup_shared_memory();

        while (1) {
            // 부모 프로세스의 메뉴 출력
            printf("[[현재 : 부모 프로세스]]\n옵션을 선택하세요 >>> \n");
            printf("1. ✏️  편지 보기\n");
            printf("2. 📄  지금까지의 메모 보기\n");
            printf("3. 🛠️  프로그램 종료하기\n");
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                printf("🤗  편지가 도착했습니다  🤗 ==>\n");

                // 파일 정보 : 편지 파일의 수정 시간을 출력
                struct stat file_stat;
                if (stat(LETTER_FILE, &file_stat) == 0) {
                    printf("편지 파일 수정 시간: %s", ctime(&file_stat.st_mtime));
                } else {
                    perror("stat");
                }

                // 시스템 정보 : 로그인 사용자명 출력
                char *login_user = getenv("USER");
                printf("\nTo. %s\n", login_user);

                // 편지 파일 내용 출력
                execl("/bin/cat", "cat", "letter.dat", NULL);
                perror("execl");

                printf("\n\n");

            } else if (choice == 2) {
                execl("./read_conversations", "read_conversations", NULL);
            } else if (choice == 3) {
                exit(0);
            } else {
                printf("Invalid choice\n");
            }
        }
    }
}

// 메인 함수
int main() {
    int msqid;
    key_t key;

    // 공유 메모리 및 메시지 큐 생성
    key = ftok(SHM_FILE, 1);
    msqid = msgget(key, IPC_CREAT | 0666);

    // 현재 IPC 상태 출력
    system("ipcs");

    while (1) {
        // 자식 프로세스의 메뉴 출력
        printf("[[현재 : 자식 프로세스]]\n옵션을 선택하세요 >>> \n");
        printf("1. 📄  메모하기\n");
        printf("2. ✏️  편지 작성하기\n");
        printf("3. ✏️  편지 내용 확인하기\n");
        printf("4. ➡️  다음으로 넘어가기\n");
        int choice;
        scanf("%d", &choice);

        // 자식 프로세스 생성
        pid_t pid = fork();
        if (pid == 0) { // 자식 프로세스
            switch (choice) {
                case 1: memo(); break;
                case 2: write_letter(); break;
                case 3: send_letter(); exit(2); break;
                case 4: exit(0); break;
                default: printf("❗ 지정된 메뉴만 선택해주세요\n"); exit(1);
            }
        } else { // 부모 프로세스
            handle_child_exit();
        }
    }

    return 0;
}
