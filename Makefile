# 컴파일러 및 컴파일 옵션 정의
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# 기본 타겟 정의
all: main lobby memo write_letter send_letter read_conversations

# 실행 파일 생성 규칙
main: main.o
	$(CC) $(CFLAGS) -o main main.o

lobby: lobby.o
	$(CC) $(CFLAGS) -o lobby lobby.o

memo: memo.o
	$(CC) $(CFLAGS) -o memo memo.o

write_letter: write_letter.o
	$(CC) $(CFLAGS) -o write_letter write_letter.o

send_letter: send_letter.o
	$(CC) $(CFLAGS) -o send_letter send_letter.o

read_conversations: read_conversations.o
	$(CC) $(CFLAGS) -o read_conversations read_conversations.o

# 오브젝트 파일 생성 규칙
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

lobby.o: lobby.c
	$(CC) $(CFLAGS) -c lobby.c

memo.o: memo.c
	$(CC) $(CFLAGS) -c memo.c

write_letter.o: write_letter.c
	$(CC) $(CFLAGS) -c write_letter.c

send_letter.o: send_letter.c
	$(CC) $(CFLAGS) -c send_letter.c

read_conversations.o: read_conversations.c
	$(CC) $(CFLAGS) -c read_conversations.c

# 오브젝트 파일 및 실행 파일 삭제
clean:
	rm -f *.o main lobby memo write_letter send_letter read_conversations
