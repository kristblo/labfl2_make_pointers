#As a general note on the Makefile langauge: just like in bash,
#the syntax $(VARNAME) is replaced with a previous definition
#of the symbol VARNAME, i.e. represents variable usage.

#The list of source files is conveniently placed at the top of the Makefile.
#Notice the backslash indicating a linebreak, necessary unless we want the 
#list to be horisontal: SOURCE_FILES := main.c motordriver.c ... xyz.c
SOURCE_FILES := main.c \
	motordriver.c

#The three subdirectories are then listed for later use
SOURCE_DIR := src
INCLUDE_DIR := inc
BUILD_DIR := build

#Continue by specifying which compiler (CC) and which flags (CFLAGS) to use
CC := gcc
CFLAGS := -Wall -Werror
CFLAGS += -Wno-unused-variable -Wno-incompatible-pointer-types
CFLAGS += $(addprefix -I , $(INCLUDE_DIR)) #Header files only need to be included per directory


#Create a list of object files, one per c file.
#Notice that = is used instead of := to indicate
#that the symbol will be expanded at some later
#point in the script.
OBJECT_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)


#The 'rules' section of the Makefile starts here:
#with the creation of the default goal referencing a
#hitherto undefined rule called '$(BUILD_DIR)/main'
.DEFAULT_GOAL := $(BUILD_DIR)/main

#Calling 'make build' results in the creation of the 'build' directory.
#Other rules depend on it, and therefore it must be defined first
# -- not unlike functions in other languages. This rule has no dependencies,
#as we can see by the lack of items on the right hand side of the colon
$(BUILD_DIR):
	mkdir $(BUILD_DIR)


#We then create the .o files, listing the .c files in SOURCE_DIR as dependencies.
#Piping the BUILD_DIR rule ensures that it is executed if necessary to make the main rule work.
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
#This evaluates to 'gcc -Wall -I inc -c src/main.c -o build/main.o'
#and similar for all listed .c files
	$(CC) $(CFLAGS) -c $< -o $@


#Finally, we define the rule referenced by the default goal. It creates the
#program file 'main' from the previously created object files and stores it
#in the build directory.
#Piping the BUILD_DIR rule here is not necessary, as it is covered by another rule
#on which this rule is also dependant
$(BUILD_DIR)/main: $(OBJECT_FILES)# | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/main



#Phony rules, i.e. rules that do stuff rather than generate files,
#are defined near the bottom of the file.
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
