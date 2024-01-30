CC = g++
EXEC = simulation
CFLAGS = $(shell sdl-config --cflags) $(shell sdl-config --libs) -lSDL_ttf

SRCS = src/main.cc \
		src/block.cc \
		src/map.cc \
		src/vehicle.cc \
		src/miner.cc \
		src/scout.cc \
		src/rescuer.cc \
		src/map_graphics.cc \
		src/input.cc \
		src/button.cc \
		src/control_panel.cc \
		src/map_general_panel.cc \
		src/secondary_panel.cc

OBJS = $(subst .cc,.o,$(SRCS))
INC = include/

.cc.o:
	${CC}  -c $< -o $@ -I ${INC} ${CFLAGS}

all: ${EXEC}

${EXEC}: ${OBJS}
	${CC} -o $@ ${OBJS} -I ${INC} ${CFLAGS}

clean:
	rm -f ${EXEC} ${OBJS}
