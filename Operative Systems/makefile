CC = gcc
FLAGS = -Wall -pthread -g
PROG = Simulador_Corridas
OBJS = malfunctionManager.o teamManager.o raceManager.o raceSimulator.o SO_Project.o

all:	${PROG}

clean:
	rm ${OBJS} *~ ${PROG}

${PROG}: ${OBJS}
	${CC} ${FLAGS} ${OBJS} -lm -o $@

.c.o:
	${CC} ${FLAGS} $< -c -o $@

##########################

malfunctionManager.o:	raceSimulator.h malfunctionManager.c

teamManager.o:	raceSimulator.h teamManager.c

raceManager.o:	raceSimulator.h raceManager.c

raceSimulator.o:	raceSimulator.h raceManager.h malfunctionManager.h teamManager.h raceSimulator.c

SO_Project.o:	SO_Project.c
