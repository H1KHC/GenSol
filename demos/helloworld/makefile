.PHONY: directories clean task_global

default:
	@echo please indicate a task!

# Tasks:
task_global: directories ./helloworld

# Targets:
./helloworld: .build/helloworld.cpp.global.o
	g++ $^  -o $@

# Sources:
.build/helloworld.cpp.global.o: helloworld.cpp
	g++ $<  -c -o $@ 

# Others:
directories:
	mkdir -p ./ .build/
clean:
	rm -rf ./ .build/
