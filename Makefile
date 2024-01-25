CC = g++
CFLAGS = 
EXEC = simulation

SRCS = main.cc \
		block.cc \
		map.cc \
		vehicle.cc \
		miner.cc \
		scout.cc \
		rescuer.cc

OBJS = $(subst .cc,.o,$(SRCS))
INC = .

.cc.o:
		${CC} ${CFLAGS} -I ${INC} -c $< -o $@

all: ${EXEC}

${EXEC}: ${OBJS}
		${CC} -o $@ ${OBJS}

clean:
		rm ${EXEC} ${OBJS}