SHELL = bash -e -o pipefail -O globstar

NAME = cpusetcores
VERSION = 0.0.2
DEBUG ?= 0
CC ?= cc

CFLAGS = -c -std=gnu99 -Wall -Wextra -Wpedantic -MMD -MP
LFLAGS = -flto

SRC := $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJ := $(SRC:%.c=obj/%.o)
BIN := bin/$(NAME)

.PHONY:
compile: $(BIN)

$(BIN): $(OBJ)
	$(info [CC/LD] $@)
	@mkdir -p $(@D)
	@$(CC) -o $@ $^ $(LDFLAGS)

obj/%.o: %.c
	$(info [CC] $@)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rv obj/ bin/

# include the dependencies
-include $(DEP)
