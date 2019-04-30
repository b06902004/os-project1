FLAG=-std=c99 -DDEBUG
main.out: main.o schedule.o process.o Makefile
	gcc ${FLAG} main.o schedule.o process.o -o main.out

main.o: main.c schedule.h process.h Makefile
	gcc ${FLAG} -c main.c

schedule.o: schedule.c schedule.h process.h Makefile
	gcc ${FLAG} -c schedule.c

process.o: process.c schedule.h process.h Makefile
	gcc ${FLAG} -c process.c

.PHONY=run
run:
	./main.out
