INPUT_FILES := $(shell find ./ -name "*.countme")
TARGETS := $(INPUT_FILES:.countme=.counted)

.PHONY: count
count: $(TARGETS)

%.counted: %.countme
	wc -w $< > $@