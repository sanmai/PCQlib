CC = gcc
CFLAGS = -O -Wall
DEBUG=-g
HDR = pcq.h
CFLAGS = $(DEFS) -pipe -O2 
#DEFS = -DHAVE_CONFIG_H
DEFS = 
#CPPFLAGS  = -I. -I${prefix}/include
CPPFLAGS  = -I.
PLIB = pcq.o

all: pcq_power.o pcq.o mygps

pcq_power.o: pcq_power.c pcq_power.h
	${CC} ${DEBUG} ${CFLAGS} -c pcq_power.c 
mygps: mygps.c pcq_power.h pcq_power.o
	${CC} ${DEBUG} ${CFLAGS} -o mygps pcq_power.o mygps.c 
pcq.o: pcq.c $(HDR) pcqinfo.h
	$(CC) $(CFLAGS) $(CPPFLAGS) ${DEBUG} -c pcq.c
pcq_print.o: pcq_print.c pcq.h pcqinfo.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEBUG) -c pcq_print.c
test: pcq_test.c pcq.h pcqinfo.h pcq.o pcq_power.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEBUG) -o pcq_test pcq.o pcq_power.o pcq_print.o pcq_test.c 
test2: pcq_test2.c pcq.h pcqinfo.h pcq.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEBUG) -o pcq_test2 pcq.o pcq_power.o pcq_test2.c 
testall: pcq_testALL.c pcq.h pcqinfo.h pcq.o pcq_power.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEBUG) -o pcq_testALL pcq.o pcq_power.o pcq_testALL.c 
clean:
	rm -f pcq_power mygps pcq_test pcq_test2 pcq_testALL *.o *.core
