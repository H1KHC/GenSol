.PHONY: directories clean task_debug task_release

default: task_release

# Tasks:
task_debug: directories bin/gensold

task_release: directories bin/gensol

# Targets:
bin/gensol: .build/src/access.cpp.release.o .build/src/debugOutput.cpp.release.o .build/src/generate.cpp.release.o .build/src/help.cpp.release.o .build/src/init.cpp.release.o .build/src/loadAndParse.cpp.release.o .build/src/main.cpp.release.o .build/src/matchFiles.cpp.release.o .build/src/merge.cpp.release.o .build/src/solution.cpp.release.o .build/src/variables.cpp.release.o
	g++ -o $@ $^ -lboost_filesystem-mt -lboost_program_options-mt -lboost_system-mt -static-libgcc -std=c++11

bin/gensold: .build/src/access.cpp.debug.o .build/src/debugOutput.cpp.debug.o .build/src/generate.cpp.debug.o .build/src/help.cpp.debug.o .build/src/init.cpp.debug.o .build/src/loadAndParse.cpp.debug.o .build/src/main.cpp.debug.o .build/src/matchFiles.cpp.debug.o .build/src/merge.cpp.debug.o .build/src/solution.cpp.debug.o .build/src/variables.cpp.debug.o
	g++ -o $@ $^ -lboost_filesystem-mt -lboost_program_options-mt -lboost_system-mt -static-libgcc -std=c++11

# Sources:
.build/src/access.cpp.release.o: src/access.cpp include/modules/task.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/debugOutput.cpp.release.o: src/debugOutput.cpp include/trace.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/generate.cpp.release.o: src/generate.cpp include/solution.h include/error.h include/modules/task.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h include/out.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/help.cpp.release.o: src/help.cpp
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/init.cpp.release.o: src/init.cpp include/modules/basicModule.h include/error.h include/trace.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/loadAndParse.cpp.release.o: src/loadAndParse.cpp include/trace.h include/error.h include/modules/task.h include/modules/basicModule.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h include/solution.h include/error.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/main.cpp.release.o: src/main.cpp include/trace.h include/error.h include/solution.h include/error.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/matchFiles.cpp.release.o: src/matchFiles.cpp include/modules/task.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/merge.cpp.release.o: src/merge.cpp include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/solution.cpp.release.o: src/solution.cpp include/trace.h include/solution.h include/error.h include/error.h include/modules/task.h include/modules/basicModule.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h include/out.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/variables.cpp.release.o: src/variables.cpp include/modules/task.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h include/out.h include/error.h
	g++ -c -o $@ $< -O2 -std=c++11 -Iinclude/

.build/src/access.cpp.debug.o: src/access.cpp include/modules/task.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/debugOutput.cpp.debug.o: src/debugOutput.cpp include/trace.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/generate.cpp.debug.o: src/generate.cpp include/solution.h include/error.h include/modules/task.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h include/out.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/help.cpp.debug.o: src/help.cpp
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/init.cpp.debug.o: src/init.cpp include/modules/basicModule.h include/error.h include/trace.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/loadAndParse.cpp.debug.o: src/loadAndParse.cpp include/trace.h include/error.h include/modules/task.h include/modules/basicModule.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h include/solution.h include/error.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/main.cpp.debug.o: src/main.cpp include/trace.h include/error.h include/solution.h include/error.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/matchFiles.cpp.debug.o: src/matchFiles.cpp include/modules/task.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/merge.cpp.debug.o: src/merge.cpp include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/solution.cpp.debug.o: src/solution.cpp include/trace.h include/solution.h include/error.h include/error.h include/modules/task.h include/modules/basicModule.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h include/out.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

.build/src/variables.cpp.debug.o: src/variables.cpp include/modules/task.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/compiler.h include/modules/linker.h include/modules/target.h include/out.h include/error.h
	g++ -c -o $@ $< -D_DEBUG -g -std=c++11 -Iinclude/

# Others:
directories:
	mkdir -p .build/ .build/src bin/

all: task_debug task_release

clean:
	rm -rf .build/ .build/src bin/ bin/gensol bin/gensold
