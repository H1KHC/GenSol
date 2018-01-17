.PHONY: all clean directories distclean task_debug task_release

default: task_release

all: task_debug task_release

# Tasks:
task_debug: directories bin/gensold
	@echo "Task debug has been finished!"

task_release: directories bin/gensol
	@echo "Task release has been finished!"

# Targets:
bin/gensol: .build/src/access.cpp.gensol.o .build/src/debugOutput.cpp.gensol.o .build/src/generate.cpp.gensol.o .build/src/globalVariables.cpp.gensol.o .build/src/help.cpp.gensol.o .build/src/init.cpp.gensol.o .build/src/loadAndParse.cpp.gensol.o .build/src/main.cpp.gensol.o .build/src/matchFiles.cpp.gensol.o .build/src/merge.cpp.gensol.o .build/src/solution.cpp.gensol.o
	@echo "[  91% ] Linking target gensol... "
	@g++ -o $@ $^ -std=c++11 -static-libgcc -lboost_program_options-mt -lboost_system-mt -lboost_filesystem-mt
	@echo "[ 100% ] Target gensol has been built!"

bin/gensold: .build/src/access.cpp.gensold.o .build/src/debugOutput.cpp.gensold.o .build/src/generate.cpp.gensold.o .build/src/globalVariables.cpp.gensold.o .build/src/help.cpp.gensold.o .build/src/init.cpp.gensold.o .build/src/loadAndParse.cpp.gensold.o .build/src/main.cpp.gensold.o .build/src/matchFiles.cpp.gensold.o .build/src/merge.cpp.gensold.o .build/src/solution.cpp.gensold.o
	@echo "[  91% ] Linking target gensold... "
	@g++ -o $@ $^ -std=c++11 -static-libgcc -lboost_program_options-mt -lboost_system-mt -lboost_filesystem-mt
	@echo "[ 100% ] Target gensold has been built!"

# Sources:
.build/src/access.cpp.gensol.o: src/access.cpp include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h include/modules/target.h
	@echo "Generating sources for target gensol..."
	@echo "[   0% ] Compiling .build/src/access.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/debugOutput.cpp.gensol.o: src/debugOutput.cpp include/trace.h
	@echo "[   8% ] Compiling .build/src/debugOutput.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/generate.cpp.gensol.o: src/generate.cpp include/solution.h include/error.h include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h include/modules/target.h include/out.h
	@echo "[  16% ] Compiling .build/src/generate.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/globalVariables.cpp.gensol.o: src/globalVariables.cpp include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h include/modules/target.h include/out.h include/error.h
	@echo "[  25% ] Compiling .build/src/globalVariables.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/help.cpp.gensol.o: src/help.cpp
	@echo "[  33% ] Compiling .build/src/help.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/init.cpp.gensol.o: src/init.cpp include/modules/basicModule.h include/error.h
	@echo "[  41% ] Compiling .build/src/init.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/loadAndParse.cpp.gensol.o: src/loadAndParse.cpp include/trace.h include/error.h include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/modules/config.h include/modules/linker.h include/modules/target.h include/solution.h include/error.h include/variables.h
	@echo "[  50% ] Compiling .build/src/loadAndParse.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/main.cpp.gensol.o: src/main.cpp include/trace.h include/error.h include/solution.h include/error.h
	@echo "[  58% ] Compiling .build/src/main.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/matchFiles.cpp.gensol.o: src/matchFiles.cpp include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h include/modules/target.h
	@echo "[  66% ] Compiling .build/src/matchFiles.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/merge.cpp.gensol.o: src/merge.cpp include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h
	@echo "[  75% ] Compiling .build/src/merge.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/solution.cpp.gensol.o: src/solution.cpp include/trace.h include/solution.h include/error.h include/error.h include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/modules/config.h include/modules/linker.h include/modules/target.h include/out.h
	@echo "[  83% ] Compiling .build/src/solution.cpp.gensol.o..."
	@g++ -c -o $@ $< -O2 -std=c++11 -m32 -Iinclude/

.build/src/access.cpp.gensold.o: src/access.cpp include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h include/modules/target.h
	@echo "Generating sources for target gensold..."
	@echo "[   0% ] Compiling .build/src/access.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/debugOutput.cpp.gensold.o: src/debugOutput.cpp include/trace.h
	@echo "[   8% ] Compiling .build/src/debugOutput.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/generate.cpp.gensold.o: src/generate.cpp include/solution.h include/error.h include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h include/modules/target.h include/out.h
	@echo "[  16% ] Compiling .build/src/generate.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/globalVariables.cpp.gensold.o: src/globalVariables.cpp include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h include/modules/target.h include/out.h include/error.h
	@echo "[  25% ] Compiling .build/src/globalVariables.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/help.cpp.gensold.o: src/help.cpp
	@echo "[  33% ] Compiling .build/src/help.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/init.cpp.gensold.o: src/init.cpp include/modules/basicModule.h include/error.h
	@echo "[  41% ] Compiling .build/src/init.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/loadAndParse.cpp.gensold.o: src/loadAndParse.cpp include/trace.h include/error.h include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/modules/config.h include/modules/linker.h include/modules/target.h include/solution.h include/error.h include/variables.h
	@echo "[  50% ] Compiling .build/src/loadAndParse.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/main.cpp.gensold.o: src/main.cpp include/trace.h include/error.h include/solution.h include/error.h
	@echo "[  58% ] Compiling .build/src/main.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/matchFiles.cpp.gensold.o: src/matchFiles.cpp include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h include/modules/target.h
	@echo "[  66% ] Compiling .build/src/matchFiles.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/merge.cpp.gensold.o: src/merge.cpp include/modules/compiler.h include/modules/basicModule.h include/error.h include/trace.h include/modules/config.h include/modules/linker.h
	@echo "[  75% ] Compiling .build/src/merge.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

.build/src/solution.cpp.gensold.o: src/solution.cpp include/trace.h include/solution.h include/error.h include/error.h include/modules/task.h include/modules/compiler.h include/modules/basicModule.h include/modules/config.h include/modules/linker.h include/modules/target.h include/out.h
	@echo "[  83% ] Compiling .build/src/solution.cpp.gensold.o..."
	@g++ -c -o $@ $< -g -D_DEBUG -std=c++11 -m32 -Iinclude/

# Others:
directories:
	@mkdir -p .build/ .build/src bin/
	@echo "Directories has been created!"

distclean:
	@rm -rf .build/
	@echo "Cleaned!"

clean:
	@rm -rf .build/ .build/src bin/ bin/gensol bin/gensold
	@echo "Cleaned!"

.PHONY: install_gensol install_gensold

install_gensol:
	@echo "Installing gensol..."
	@cp "bin/gensol" "/usr/bin/"
	@echo "Done!"

install_gensold:
	@echo "Installing gensold..."
	@cp "bin/gensold" "/usr/bin/"
	@echo "Done!"
