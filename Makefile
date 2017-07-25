all: time
time: time.c
	gcc time.c -o time
clean:
	 rm -f *.o   
	 rm -f time
	 rm -f *~

