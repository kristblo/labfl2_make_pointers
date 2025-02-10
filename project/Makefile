#List all source files to be compiled
SOURCE_FILES := \
	main.c \
	motordriver.c

SOURCE_DIR := src
INCLUDE_DIR := inc
BUILD_DIR := build

CC := gcc
CFLAGS := -Wall -Werror -Wpedantic #skrur paa alle advarsler; eleverer advarsler til feil; skrur paa enda flere feil (ISO C)
CFLAGS += -Wno-unused-variable -Wno-incompatible-pointer-types #skrur av kategorien "unused variable"
CFLAGS += $(addprefix -I , $(INCLUDE_DIR)) #forteller kompilatoren at inc skal inkluderes
CFLAGS += -O3 #skrur paa hoeyeste mulige nivaa av optimalisering

OBJECT_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main

#Calling 'make build' results in the creation of the 'build' directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

#Piping the BUILD_DIR ensures that it is executed IF necessary to make the main rule work
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Piping the BUILD_DIR rule here is not necessary, as it is covered by another rule
#on which this rule is also dependant
$(BUILD_DIR)/main: $(OBJECT_FILES)# | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/main


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
