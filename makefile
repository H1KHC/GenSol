.PHONY: directories clean task_debug task_release

default: task_release

# Tasks:
task_debug: directories bin/gensold

task_release: directories bin/gensol

# Targets:
bin/gensol: .build/src/access.cpp.release.o \
 .build/src/debugOutput.cpp.release.o .build/src/error.cpp.release.o \
 .build/src/generate.cpp.release.o .build/src/help.cpp.release.o \
 .build/src/init.cpp.release.o .build/src/loadAndParse.cpp.release.o \
 .build/src/main.cpp.release.o .build/src/matchFiles.cpp.release.o \
 .build/src/merge.cpp.release.o .build/src/solution.cpp.release.o \
 .build/src/variables.cpp.release.o
	g++ $^ -lboost_filesystem -lboost_system -std=c++11 -o $@

bin/gensold: .build/src/access.cpp.debug.o .build/src/debugOutput.cpp.debug.o \
 .build/src/error.cpp.debug.o .build/src/generate.cpp.debug.o \
 .build/src/help.cpp.debug.o .build/src/init.cpp.debug.o \
 .build/src/loadAndParse.cpp.debug.o .build/src/main.cpp.debug.o \
 .build/src/matchFiles.cpp.debug.o .build/src/merge.cpp.debug.o \
 .build/src/solution.cpp.debug.o .build/src/variables.cpp.debug.o
	g++ $^ -lboost_filesystem -lboost_system -std=c++11 -o $@

# Sources:
.build/src/access.cpp.release.o: src/access.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/debugOutput.cpp.release.o: src/debugOutput.cpp include/trace.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/error.cpp.release.o: src/error.cpp include/error.h include/trace.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/generate.cpp.release.o: src/generate.cpp include/solution.h include/error.h \
 include/modules/task.h include/modules/basicModule.h include/error.h \
 include/trace.h include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/out.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/help.cpp.release.o: src/help.cpp
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/init.cpp.release.o: src/init.cpp include/modules/basicModule.h include/error.h \
 include/trace.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/loadAndParse.cpp.release.o: src/loadAndParse.cpp include/trace.h include/error.h \
 include/modules/task.h include/modules/basicModule.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/solution.h \
 include/error.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/main.cpp.release.o: src/main.cpp include/error.h include/solution.h include/error.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/matchFiles.cpp.release.o: src/matchFiles.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/merge.cpp.release.o: src/merge.cpp include/modules/compiler.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/linker.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/solution.cpp.release.o: src/solution.cpp include/trace.h include/solution.h \
 include/error.h include/error.h include/modules/task.h \
 include/modules/basicModule.h include/modules/config.h \
 include/modules/compiler.h include/modules/linker.h \
 include/modules/target.h include/out.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/variables.cpp.release.o: src/variables.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/out.h \
 include/error.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/access.cpp.debug.o: src/access.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/debugOutput.cpp.debug.o: src/debugOutput.cpp include/trace.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/error.cpp.debug.o: src/error.cpp include/error.h include/trace.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/generate.cpp.debug.o: src/generate.cpp include/solution.h include/error.h \
 include/modules/task.h include/modules/basicModule.h include/error.h \
 include/trace.h include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/out.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/help.cpp.debug.o: src/help.cpp
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/init.cpp.debug.o: src/init.cpp include/modules/basicModule.h include/error.h \
 include/trace.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/loadAndParse.cpp.debug.o: src/loadAndParse.cpp include/trace.h include/error.h \
 include/modules/task.h include/modules/basicModule.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/solution.h \
 include/error.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/main.cpp.debug.o: src/main.cpp include/error.h include/solution.h include/error.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/matchFiles.cpp.debug.o: src/matchFiles.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/merge.cpp.debug.o: src/merge.cpp include/modules/compiler.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/linker.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/solution.cpp.debug.o: src/solution.cpp include/trace.h include/solution.h \
 include/error.h include/error.h include/modules/task.h \
 include/modules/basicModule.h include/modules/config.h \
 include/modules/compiler.h include/modules/linker.h \
 include/modules/target.h include/out.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

.build/src/variables.cpp.debug.o: src/variables.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/out.h \
 include/error.h
	g++ $< -D_DEBUG -g -std=c++11 -c -o $@ -Iinclude/

# Others:
directories:
	mkdir -p .build/ .build/src bin/
clean:
	rm -rf .build/ .build/src bin/
