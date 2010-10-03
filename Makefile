all: oct

oct: main.c
	${CROSS_COMPILE}gcc -static -o oct main.c

.PHONY:
	clean

clean:
	-rm -f -r *.o *~ *.out oct
