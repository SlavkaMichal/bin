CPPFLAGS=-g -pthread -I/sw/include/root -std=c++14
LDFLAGS=-g
#LDLIBS=-L/sw/lib/root -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint \
       -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lz -L/sw/lib -lfreetype -lz \
       -Wl,-framework,CoreServices -Wl,-framework,ApplicationServices -pthread -Wl,-rpath,/sw/lib/root \
       -lm -ldl

all: build clean

build: main.o chromosome.o evolution.o
	g++ $(LDFLAGS) -o cgp main.o  chromosome.o evolution.o #$(LDLIBS)

main.o: src/main.cpp src/chromosome.h src/evolution.h src/cgp.h
	g++ $(CPPFLAGS) -c src/main.cpp

evolution.o: chromosome.o src/evolution.cpp src/evolution.h src/chromosome.h src/cgp.h
	g++ $(CPPFLAGS) -c src/evolution.cpp

chromosome.o: src/chromosome.cpp src/chromosome.h src/cgp.h
	g++ $(CPPFLAGS) -c src/chromosome.cpp

clean:
		rm *.o

cleanlog:
		rm log_*.*
