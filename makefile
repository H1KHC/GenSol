.PHONY: directories clean TASK_debug TASK_release

# Tasks:
TASK_RELEASE: directories bin/GenSol

TASK_DEBUG: directories bin/GenSolD

# Targets:
bin/GenSol: .build/src/access.cpp.o .build/src/debugOutput.cpp.o \
 .build/src/error.cpp.o .build/src/generate.cpp.o .build/src/help.cpp.o \
 .build/src/init.cpp.o .build/src/loadAndParse.cpp.o .build/src/main.cpp.o \
 .build/src/matchFiles.cpp.o .build/src/merge.cpp.o .build/src/solution.cpp.o \
 .build/src/variables.cpp.o
	g++ $^ -lboost_filesystem -lboost_system -std=c++11 -o $@

bin/GenSolD: .build/src/access.cpp.o .build/src/debugOutput.cpp.o \
 .build/src/error.cpp.o .build/src/generate.cpp.o .build/src/help.cpp.o \
 .build/src/init.cpp.o .build/src/loadAndParse.cpp.o .build/src/main.cpp.o \
 .build/src/matchFiles.cpp.o .build/src/merge.cpp.o .build/src/solution.cpp.o \
 .build/src/variables.cpp.o
	g++ $^ -lboost_filesystem -lboost_system -std=c++11 -o $@

# Sources:
.build/src/access.cpp.o: src/access.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/debugOutput.cpp.o: src/debugOutput.cpp include/trace.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/error.cpp.o: src/error.cpp include/error.h include/trace.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/generate.cpp.o: src/generate.cpp include/solution.h include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/out.h \
 include/error.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/help.cpp.o: src/help.cpp
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/init.cpp.o: src/init.cpp include/modules/basicModule.h include/error.h \
 include/trace.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/loadAndParse.cpp.o: src/loadAndParse.cpp include/trace.h include/error.h \
 include/modules/task.h include/modules/basicModule.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/solution.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/main.cpp.o: src/main.cpp include/error.h include/solution.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/matchFiles.cpp.o: src/matchFiles.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/merge.cpp.o: src/merge.cpp include/modules/compiler.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/linker.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/solution.cpp.o: src/solution.cpp include/trace.h include/solution.h \
 include/error.h include/modules/task.h include/modules/basicModule.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/out.h \
 include/error.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

.build/src/variables.cpp.o: src/variables.cpp include/modules/task.h \
 include/modules/basicModule.h include/error.h include/trace.h \
 include/modules/config.h include/modules/compiler.h \
 include/modules/linker.h include/modules/target.h include/out.h \
 include/error.h
	g++ $< -O2 -std=c++11 -c -o $@ -Iinclude/

# Others:
directories:
	mkdir -p .build/ .build/src bin/
clean:
	rm -rf .build/ .build/src bin/
