CC = gcc
CFLAGS = -Wall
OBJS = sharedMemory.o semaphore.o queue.o logging.o

all:		startClean probSemSharedMemAvHandicraft semSharedMemEntrp semSharedMemCust \
		semSharedMemCraft endClean

all64EPCTCF:	startClean probSemSharedMemAvHandicraft semSharedMemEntrp64 semSharedMemCust64 \
		semSharedMemCraft64 endClean

all64CTCF:	startClean probSemSharedMemAvHandicraft semSharedMemEntrp semSharedMemCust64 \
		semSharedMemCraft64 endClean

all64CF:	startClean probSemSharedMemAvHandicraft semSharedMemEntrp semSharedMemCust \
		semSharedMemCraft64 endClean

all32EPCTCF:	startClean probSemSharedMemAvHandicraft semSharedMemEntrp32 semSharedMemCust32 \
		semSharedMemCraft32 endClean

all32CTCF:	startClean probSemSharedMemAvHandicraft semSharedMemEntrp semSharedMemCust32 \
		semSharedMemCraft32 endClean

all32CF:	startClean probSemSharedMemAvHandicraft semSharedMemEntrp semSharedMemCust \
		semSharedMemCraft32 endClean

probSemSharedMemAvHandicraft:	probSemSharedMemAvHandicraft.o $(OBJS)
				$(CC) -o $@ $^ -lm
				mv probSemSharedMemAvHandicraft ../run/probSemSharedMemAvHandicraft

semSharedMemEntrp:		semSharedMemEntrp.o $(OBJS)
				$(CC) -o $@ $^ -lm
				mv semSharedMemEntrp ../run/entrepreneur

semSharedMemEntrp64:
				cp ../run/entrepreneur_64 ../run/entrepreneur

semSharedMemEntrp32:
				cp ../run/entrepreneur_32 ../run/entrepreneur

semSharedMemCust:		semSharedMemCust.o $(OBJS)
				$(CC) -o $@ $^ -lm
				mv semSharedMemCust ../run/customer

semSharedMemCust64:
				cp ../run/customer_64 ../run/customer

semSharedMemCust32:
				cp ../run/customer_32 ../run/customer

semSharedMemCraft:		semSharedMemCraft.o $(OBJS)
				$(CC) -o $@ $^ -lm
				mv semSharedMemCraft ../run/craftsman

semSharedMemCraft64:
				cp ../run/craftsman_64 ../run/craftsman

semSharedMemCraft32:
				cp ../run/craftsman_32 ../run/craftsman

startClean:
		rm -f *.o probSemSharedMemAvHandicraft semSharedMemEntrp semSharedMemCust \
			semSharedMemCraft
		rm -f ../run/probSemSharedMemAvHandicraft ../run/entrepreneur ../run/customer \
			../run/craftsman ../run/error*

endClean:
		rm -f *.o
