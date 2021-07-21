CC = gcc
CFLAGS = -g -Wall

all: main1 main2

main1: uart.o main1.o

main2: uart.o printf.o main2.o

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<
