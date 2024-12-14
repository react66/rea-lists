DIFCMD= diff --strip-trailing-cr 
CC= g++ -std=c++17 -Wno-deprecated 

all: lists.o tst2.skl tst3.skl tst4.skl tst5.skl plyiotst plyiotst.skl  \
     install rmr1

lists.o: lists.h lists.cc clasnum.h
	$(CC) -g -c lists.cc

tst2.skl: tst2.cc lists.o
	$(CC) lists.o -g -o tst2 tst2.cc 
	chmod +x run_tst2
	run_tst2
	$(DIFCMD) tst2.r1 tst2.gold

tst3.skl: tst3.cc tst3.gold lists.o
	$(CC) lists.o -g -o tst3 tst3.cc 
	chmod +x run_tst3
	run_tst3
	$(DIFCMD) tst3.r1 tst3.gold

tst4.skl: tst4.cc tst4.gold lists.o
	$(CC) lists.o -g -o tst4 tst4.cc 
	chmod +x run_tst4
	run_tst4
	$(DIFCMD) tst4.r1 tst4.gold

tst5.skl: tst5.cc lists.o
	$(CC) lists.o -g -o tst5 tst5.cc 
	chmod +x run_tst5
	run_tst5
	$(DIFCMD) tst5.r1 tst5.gold

plyiotst: plyiotst.cc lists.o plyiotst.gold
# $(CC) lists.o -g -static -o plyiotst plyiotst.cc 
	$(CC) lists.o -g -o plyiotst plyiotst.cc 
	chmod +x plyiotst

plyiotst.skl: plyiotst
	plyiotst > x1
	$(DIFCMD) plyiotst.gold x1
	
clean: 
	rm  -f *.o
	rm  -f tst2
	rm  -f tst3
	rm  -f tst4
	rm  -f tst5
	rm  -f plyiotst
	rm  -f x1
	rm  -f ~/lib/lists.o 
	rm  -f ~/include/lists.h
	rm  -f ~/include/clasnum.h

rmr1:
	rm  -f *.r1
	rm  -f x1

install: ~/lib/lists.o ~/include/lists.h

~/lib/lists.o: lists.o
	cp lists.o ~/lib

~/include/lists.h: lists.h clasnum.h
	cp clasnum.h ~/include
	cp lists.h ~/include
