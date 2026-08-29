CC = gcc
CFLAGS = -g -Wall -Wextra -Wsign-conversion -fsanitize=address,undefined -fdiagnostics-color=always

define collect
$(foreach dep,$($(2)_DEPS),$($(dep)_$(1)))
endef

USEFUL_MACROS_INCLD := UsefulMacros/include

DATATYPES_DEPS  = USEFUL_MACROS
DATATYPES_FILES = $(call collect,FILES,DATATYPES) Datatypes/src/dbuffer
DATATYPES_INCLD = $(call collect,INCLD,DATATYPES) Datatypes/include

ARGS_DEPS  = USEFUL_MACROS
ARGS_FILES = $(call collect,FILES,ARGS) Args/src/args
ARGS_INCLD = $(call collect,INCLD,ARGS) Args/include

SHA_DEPS  = USEFUL_MACROS
SHA_FILES = $(call collect,FILES,SHA) SHA/main SHA/src/sha2
SHA_INCLD = $(call collect,INCLD,SHA) SHA/include

BASE64_DEPS  = USEFUL_MACROS
BASE64_FILES = $(call collect,FILES,BASE64) Base64/main Base64/src/base64
BASE64_INCLD = $(call collect,INCLD,BASE64) Base64/include

ASCII_HELP_DEPS  = DATATYPES ARGS
ASCII_HELP_FILES = $(call collect,FILES,ASCII_HELP) ASCIIHelper/src/table ASCIIHelper/src/cols ASCIIHelper/src/asciihelp
ASCII_HELP_INCLD = $(call collect,INCLD,ASCII_HELP)

PROGRAMS := sha2 base64 asciihelp

OUT := bin

.PHONY: all clean $(PROGRAMS)

all: $(PROGRAMS)

clean:
	rm -rf bin

$(OUT):
	@mkdir -p $(OUT)

$(PROGRAMS): %: $(OUT)/%

$(OUT)/sha2: $(addsuffix .c,$(SHA_FILES)) | $(OUT)
	$(CC) $(CFLAGS) $^ -o $@ $(addprefix -I,$(SHA_INCLD))

$(OUT)/base64: $(addsuffix .c,$(BASE64_FILES)) | $(OUT)
	$(CC) $(CFLAGS) $^ -o $@ $(addprefix -I,$(BASE64_INCLD))

$(OUT)/asciihelp: $(addsuffix .c,$(ASCII_HELP_FILES)) | $(OUT)
	$(CC) $(CFLAGS) $^ -o $@ $(addprefix -I,$(ASCII_HELP_INCLD))