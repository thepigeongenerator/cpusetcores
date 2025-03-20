# project name = the workspace directory name
NAME := $(shell basename $(PWD))

# compiler settings
CC := clang
STD := c17
LANG = c
CFLAGS := -target x86_64-pc-linux-gnu -Wall -Wextra -Wpedantic -Wno-pointer-arith
LDFLAGS := -lc
DEBUG ?= 0

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -fsanitize=address,undefined -g -Og
PROF := dbg
else
REL_FLAGS += -O2
PROF := rel
endif

# dirs
DIR_BIN := bin
DIR_OBJ := obj
DIR_BUILD := $(DIR_BIN)/$(PROF)
DIR := $(DIR_BIN)/$(PROF) $(DIR_OBJ)/$(PROF)

# source files
SRC := $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)

# output locations
OBJ := $(patsubst src/%,$(DIR_OBJ)/$(PROF)/%,$(SRC:.c=.o))
DEP := $(OBJ:.o=.d)
TARGET := $(DIR_BUILD)/$(NAME)

define wr_colour
	@echo -e "\033[$(2)m$(1)\033[0m"
endef

compile: compile_commands.json $(DIR) $(TARGET)
clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ) compile_commands.json

# create the binary (linking step)
$(TARGET): $(OBJ)
	@$(call wr_colour,"CC: '$(CC)'",94)
	@$(call wr_colour,"CFLAGS: '$(CFLAGS)'",94)
	@$(call wr_colour,"LDFLAGS: '$(LDFLAGS)'",94)
	@$(call wr_colour,"linking to: '$@'",92)
	@$(CC) -o $(TARGET) $^ $(CFLAGS) $(LDFLAGS)
	@$(call wr_colour,"current profile: '$(PROF)'",93)

# create .o and .d files
$(DIR_OBJ)/$(PROF)/%.o: src/%.c
	@$(call wr_colour,"compiling $(notdir $@) from $(notdir $<)",92)
	@mkdir -p $(dir $@)
	@$(CC) -o $@ -MD -MP -c $< $(CFLAGS) -std=$(STD) -x $(LANG) -Wno-unused-command-line-argument

# create directories
$(DIR):
	mkdir -p $@

# update compile commands if the makefile has been updated (for linting)
ifeq ($(DEBUG),1)
compile_commands.json: makefile
	$(MAKE) clean
	@touch compile_commands.json
	bear -- make compile
else
compile_commands.json:
endif

# include the dependencies
-include $(DEP)
