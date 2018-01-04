.PHONY: directories clean task_global

default: task_global

# Tasks:
task_global: directories ./helloworld

# Targets:
./helloworld: .build/helloworld.cpp.global.o
	g++ -o $@ $^ 

# Sources:
.build/helloworld.cpp.global.o: helloworld.cpp
	g++ -c -o $@ $<  

# Others:
directories:
	mkdir -p .build/

all: task_global

clean:
	rm -rf .build/ ./helloworld
