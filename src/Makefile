main.out: main.o schedule.o process.o Makefile
	gcc -std=c99 main.o schedule.o process.o -o main.out

main.o: main.c schedule.h process.h Makefile
	gcc -std=c99 -c main.c

schedule.o: schedule.c schedule.h process.h Makefile
	gcc -std=c99 -c schedule.c

process.o: process.c schedule.h process.h Makefile
	gcc -std=c99 -c process.c

.PHONY=run
run:
	./main.out
