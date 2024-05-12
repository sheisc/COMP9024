####################################################################################
# How to Use in CSE VLAB
# 
# 1. Test all testcases in tests
#
#   make
#   make compileAll
#   make runAll
#
# 2. Test one testcase
#
#   make 
#   make compileOne SCC_SRC_FILE=tests/Factorial.scc
#   make runOne SCC_SRC_FILE=tests/Factorial.scc
#
####################################################################################


PROJ_ROOT_PATH = $(shell pwd)

C_SRC_FILES = $(shell find ./src -name "*.c")

H_SRC_FILES = $(shell find ./src -name "*.h")

#ALL_SCC_SRC_FILES = $(shell find ./tests -name "*.scc")
#ALL_SCC_SRC_FILES = ./tests/Factorial.scc ./tests/Stack.scc ./tests/BinaryTree.scc ./tests/EnvVars.scc ./tests/MultipleArguments.scc ./tests/DoWhile.scc ./tests/Associativity.scc ./tests/SystemPython3.scc
ALL_SCC_SRC_FILES = ./tests/Factorial.scc ./tests/Stack.scc ./tests/BinaryTree.scc ./tests/EnvVars.scc ./tests/MultipleArguments.scc ./tests/Associativity.scc ./tests/SystemPython3.scc


SCC_SRC_FILE = tests/Factorial.scc

#OBJS        = $(C_SRC:.c=.o)
CC          = gcc -g

all:
	make scc

scc:  $(C_SRC_FILES) $(H_SRC_FILES)
	$(CC) -o scc -I $(PROJ_ROOT_PATH)/src $(C_SRC_FILES)

compileOne: scc
	./scc < ${SCC_SRC_FILE} > ${SCC_SRC_FILE}.s
	@echo "\n\n   x86_64 assembly code generated in ${SCC_SRC_FILE}.s\n\n"
	
runOne: scc
	./scc < ${SCC_SRC_FILE} > ${SCC_SRC_FILE}.s
	$(CC) -no-pie ${SCC_SRC_FILE}.s libs/SccHeap.c libs/SccLib.c -I ${PROJ_ROOT_PATH}/libs -o ${SCC_SRC_FILE}.exe
	${SCC_SRC_FILE}.exe


compileAll: scc
	@for testCase in ${ALL_SCC_SRC_FILES}; do \
		echo "\n\n...... $$testCase .... \n\n"; \
		./scc < $$testCase > $$testCase.s; \
		echo "   x86_64 assembly code generated in $$testCase.s\n"; \
	done

runAll: scc
	@for testCase in ${ALL_SCC_SRC_FILES}; do \
		echo "\n\n...... $$testCase .... \n\n"; \
		./scc < $$testCase > $$testCase.s; \
		$(CC) -no-pie $$testCase.s libs/SccHeap.c libs/SccLib.c -I ${PROJ_ROOT_PATH}/libs -o $$testCase.exe; \
		$$testCase.exe; \
	done
	
# https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Preprocessor-Options.html	
testHeap:
	$(CC) -D USE_OUR_MALLOC_FREE libs/TestSccHeap.c libs/SccHeap.c -I ${PROJ_ROOT_PATH}/libs -o TestSccHeap
	./TestSccHeap
	
clean:
	rm -f *.o *.scc.s scc TestSccHeap
	make -C tests
	


