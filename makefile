CC = gcc
CFLAGS = -g -Wall -Wextra -Wsign-conversion -fsanitize=address,undefined -fdiagnostics-color=always

SHA_FILES := SHA/main SHA/src/sha2
SHA_INCLUDES := SHA/include UsefulMacros/include

BASE64_FILES := Base64/main Base64/src/base64
BASE64_INCLUDES := Base64/include UsefulMacros/include

PROGRAMS := sha2 base64

OUT := bin

.PHONY: all $(PROGRAMS)

all: $(PROGRAMS)

$(OUT):
	@mkdir -p $(OUT)

$(PROGRAMS): %: bin/%

$(OUT)/sha2: $(addsuffix .c,$(SHA_FILES)) | $(OUT)
	$(CC) $(CFLAGS) $^ -o $@ $(addprefix -I,$(SHA_INCLUDES))

$(OUT)/base64: $(addsuffix .c,$(BASE64_FILES)) | $(OUT)
	$(CC) $(CFLAGS) $^ -o $@ $(addprefix -I,$(BASE64_INCLUDES))