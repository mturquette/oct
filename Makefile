all: oct

oct:
	gcc -o oct main.c

.PHONY:
	clean

clean:
	-rm -f -r *.o *~ *.out oct
