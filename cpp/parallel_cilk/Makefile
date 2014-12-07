CC = /opt/tools/libraries/openmpi/1.6-gcc-4.6.3/bin/mpic++
CFLAGS = -Wall -g
LDFLAGS = -lm

FILE = fractal

all: fractal 
	${CC} ${CFLAGS} ${LDFLAGS} ${FILE}.cpp -o ${FILE}

clean:
	rm -rf *.o *~ fractal *.pgm
