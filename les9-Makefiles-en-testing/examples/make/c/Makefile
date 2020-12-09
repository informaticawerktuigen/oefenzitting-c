SOURCE_FILES := $(shell find ./ -name "*.c")
OBJECT_FILES := $(SOURCE_FILES:.c=.o)
COMPILE_FLAGS := -Wall -Werror -std=c17 -pedantic
LINK_FLAGS := -lm
EXECUTABLE := my_executable

$(EXECUTABLE): $(OBJECT_FILES)
	gcc $^ -o $@ $(LINK_FLAGS)

%.o: %.c
	gcc -c $< $(COMPILE_FLAGS)

.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJECT_FILES)