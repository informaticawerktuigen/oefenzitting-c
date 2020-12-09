#Folders
SRC_DIR := src
BUILD_DIR := build
LIB_DIR := lib
TEST_DIR := test

#Compiler settings
CC := gcc
COMPILE_FLAGS := -Wall -Werror -std=c17 -pedantic
LINK_FLAGS := 

#Main project
SOURCE_FILES := $(shell find $(SRC_DIR)/ -name "*.c")
OBJECT_FILES := $(SOURCE_FILES:.c=.o)
EXECUTABLE := main_program

#Library
LIB_SOURCE_FILES := $(shell find $(LIB_DIR)/ -name "*.c")
LIB_OBJECT_FILES := $(LIB_SOURCE_FILES:.c=.o)
LIBRARY := liblinkedlist.a

#Test suite
TEST_SOURCE_FILES := $(shell find $(TEST_DIR)/ -name "*.c")
TEST_OBJECT_FILES := $(TEST_SOURCE_FILES:.c=.o)
TEST_EXECUTABLE := test_suite

#Build rules
.PHONY: all
all: $(TEST_EXECUTABLE) $(EXECUTABLE)

$(LIBRARY): $(LIB_OBJECT_FILES)
	ar rcs $@ $(LIB_OBJECT_FILES)

$(TEST_EXECUTABLE): $(TEST_OBJECT_FILES) $(LIBRARY)
	$(CC) $^ -o $@ $(LINK_FLAGS)
	./$@

$(EXECUTABLE): $(OBJECT_FILES) $(LIBRARY)
	$(CC) $^ -o $@ $(LINK_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(COMPILE_FLAGS)

.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJECT_FILES) $(LIBRARY) $(LIB_OBJECT_FILES) $(TEST_EXECUTABLE) $(TEST_OBJECT_FILES)