#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "letter.dat"  // 편지 내용이 저장될 파일 이름
#define FILE_SIZE 1024          // 파일의 크기 정의

// 새로운 편지 쓰기 함수
void write_new_letter(int fd) {
    // 파일 크기를 0으로 설정하여 기존 내용을 삭제
    ftruncate(fd, 0);
    // 파일 크기를 다시 설정
    ftruncate(fd, FILE_SIZE);

    // 파일을 메모리에 매핑
    char *mapped = (char *)mmap(0, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(1);
    }

    // 사용자로부터 입력받아 매핑된 메모리 영역에 저장
    printf("✏️ 편지로 보낼 내용을 적어보세요 >> \n");
    fgets(mapped, FILE_SIZE, stdin);

    // 매핑 해제
    munmap(mapped, FILE_SIZE);
}

// 기존 편지에 이어서 쓰기 함수
void append_letter(int fd) {
    // 파일을 메모리에 매핑
    char *mapped = (char *)mmap(0, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(1);
    }

    char buffer[FILE_SIZE];
    // 사용자로부터 입력받아 버퍼에 저장
    printf("✏️ 추가로 작성할 내용을 적어주세요 >> \n");
    fgets(buffer, FILE_SIZE, stdin);

    // 기존 내용 뒤에 새로운 내용을 추가
    strncat(mapped, buffer, FILE_SIZE - strlen(mapped) - 1);

    // 매핑 해제
    munmap(mapped, FILE_SIZE);
}

// 메인 함수
int main() {
    // 파일 열기
    int fd = open(FILE_NAME, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // 파일 크기 설정
    ftruncate(fd, FILE_SIZE);

    // 사용자에게 선택지 제공
    printf("1. 새로운 편지 쓰기 ✏️\n");
    printf("2. 이어서 쓰기 ✏️\n");
    int choice;
    scanf("%d", &choice);
    getchar();  // 개행 문자 제거

    // 사용자의 선택에 따라 함수 호출
    if (choice == 1) {
        write_new_letter(fd);
    } else if (choice == 2) {
        append_letter(fd);
    } else {
        printf("❗잘못된 선택입니다.\n");
    }

    // 파일 닫기
    close(fd);
    return 0;
}
