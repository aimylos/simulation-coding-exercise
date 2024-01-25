CC = g++
CFLAGS = 
EXEC = simulation

SRCS = main.cc \
		block.cc

OBJS = $(subst .cc,.o,$(SRCS))
INC = .

.cc.o:
		${CC} ${CFLAGS} -I ${INC} -c $< -o $@

all: ${EXEC}

${EXEC}: ${OBJS}
		${CC} -o $@ ${OBJS}

clean:
		rm ${EXEC} ${OBJS}