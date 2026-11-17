CC = gcc
CFLAGS = -Wall -Wextra

sms: main.c
	$(CC) $(CFLAGS) -o sms main.c

clean:
	rm -f sms sms.exe students.dat
