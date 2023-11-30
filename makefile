CC = gcc


CFLAGS = -Wall -Wextra -pedantic

CLIBS = -lSDL2

CSRC = main.c

COUT = Space_Invaders

all: app

app: ${CSRC}
	${CC} ${CFLAGS} ${CSRC} -o ${COUT} ${CLIBS}

test: ${COUT}
	./${COUT}

clean:
	rm ${COUT}

.PHONY: test clean
