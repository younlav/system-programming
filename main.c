#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//시그널 핸들러 지정
void handle_sigint(int sig) {
    (void)sig; // Unused parameter
    printf("\n시그널을 입력받았습니다. 프로그램을 실행합니다....\n");
    execl("./lobby", "lobby", NULL);
}

int main() {
    signal(SIGINT, handle_sigint);
    while (1) {
        printf("Ctrl+C를 눌러 프로그램을 실행하세요!\n");
        pause();
    }
    return 0;
}
