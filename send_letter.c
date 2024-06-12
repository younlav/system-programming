#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// 상수 정의
#define FILE_NAME "letter.dat"  // 편지 파일 이름
#define BUFFER_SIZE 1024        // 버퍼 크기

int main() {
    // 편지 파일을 읽기 전용으로 열기
    int fd = open(FILE_NAME, O_RDONLY);
    if (fd == -1) {  // 파일 열기에 실패한 경우
        perror("open");  // 오류 메시지 출력
        exit(1);  // 프로그램 종료
    }

    // 편지 파일 내용을 읽어 올 버퍼
    char buffer[BUFFER_SIZE];
    read(fd, buffer, BUFFER_SIZE);  // 파일 내용 읽기
    close(fd);  // 파일 닫기

    // 파이프 생성
    int pipefd[2];
    pipe(pipefd);

    // 자식 프로세스 생성
    if (fork() == 0) {  // 자식 프로세스
        close(pipefd[1]);  // 파이프 쓰기 끝 닫기
        dup2(pipefd[0], STDIN_FILENO);  // 파이프 읽기 끝을 표준 입력으로 복제
        execl("/bin/cat", "cat", NULL);  // cat 명령어 실행하여 편지 내용 출력
    } else {  // 부모 프로세스
        close(pipefd[0]);  // 파이프 읽기 끝 닫기
        write(pipefd[1], buffer, strlen(buffer));  // 버퍼 내용을 파이프로 쓰기
        close(pipefd[1]);  // 파이프 쓰기 끝 닫기
        wait(NULL);  // 자식 프로세스가 끝날 때까지 대기
    }

    return 0;  // 프로그램 종료
}
