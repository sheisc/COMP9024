####################################################################################
# 				How to Use
# 
# (1) Build the project
#   make
# (2) Run the executable
#   ./main
# (3) Clean the project
#   make clean
#
# If you want to know more about makefile, please refer to:
#
#    https://www.gnu.org/software/make/manual/html_node/Rule-Syntax.html
#
# You can reuse this Makefile in different assignments/projects in COMP9024.
# To be simple, please put your *.c and *.h in the src directory.
#
#                                                                 COMP9024 24T2
####################################################################################
PROJ_ROOT_PATH = $(shell pwd)

TARGET_EXE = main
BUILD_DIR = build

C_SRC_FILES = $(shell find ./src -name "*.c")
H_SRC_FILES = $(shell find ./src -name "*.h")

# src/*.c  --->  src/*.o ---> build/*.o
TMP_OBJ_FILES = $(C_SRC_FILES:.c=.o)
OBJ_FILES_IN_BUILD =$(subst src/,$(BUILD_DIR)/,$(TMP_OBJ_FILES))


CC= gcc
#CFLAGS = -g -no-pie --save-temps -I $(PROJ_ROOT_PATH)/src
#CFLAGS = -no-pie --save-temps -fno-asynchronous-unwind-tables -I $(PROJ_ROOT_PATH)/src 
CFLAGS = -g -Wall -I $(PROJ_ROOT_PATH)/src

# create a directory  
$(shell mkdir -p $(BUILD_DIR))

# the default target
all: 
	make $(TARGET_EXE)

# generate the target, which depends on the "build/*.o" files
$(TARGET_EXE):  $(OBJ_FILES_IN_BUILD) 	
	$(CC) $(CFLAGS) -o $(TARGET_EXE) $(OBJ_FILES_IN_BUILD)

# How to generate a "build/*.o" from a "src/*.c"
# To generate prerequisites automatically, please see COMP9024/C/HowToMake/Makefile.V2
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
$(BUILD_DIR)/%.o: src/%.c $(H_SRC_FILES)
	@mkdir -p $(shell dirname $@)
	${CC} ${CFLAGS} -c $< -o $@

view:
	find ./images -name "*.png" | sort | xargs feh -g 720x540 &	

# clean all the files generated	
clean:
	rm -rf $(TARGET_EXE) $(BUILD_DIR) 
	find . -name "*.o" | xargs rm -f
	find . -name "*.s" | xargs rm -f
	find . -name "*.i" | xargs rm -f
	find . -name "*.d" | xargs rm -f
	find . -name "*.bc" | xargs rm -f
