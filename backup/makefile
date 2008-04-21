CC=gcc
LDLIBS = -L/usr/X11R6/lib -lglut -lGL -lGLU -lXmu -lX11 -lXi -lm

run: runprac5

all: prac5

prac5: prac5.o
	$(CC) $@.o $(LDLIBS) -o $@
	
.o:
	$(CC) $@.cpp $(LDLIBS) -o $@
	
#	$(CC) $@.cpp $(LDLIBS) -o $@

#main:
#	$(CC) main.o $(LDLIBS) -o $@

runprac5: prac5
	./prac5

clean:
	rm -rf prac5.o prac5
