CC = gcc
GCC_INCLUDE = $(shell $(CC) -m32 -print-file-name=include)

# автоматически добавляем -I на каждую подпапку lib/*, чтобы
# #include "bare.h" / #include "malloc.h" резолвились без полного пути
LIBDIRS = $(wildcard lib/*/)
INCLUDES = $(addprefix -I,$(LIBDIRS))

CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostdinc \
         -mno-sse -mno-sse2 -mno-mmx -mno-80387 -mgeneral-regs-only \
         -isystem $(GCC_INCLUDE) $(INCLUDES) -I. -Wall -Wextra -MMD -MP -c
LD = ld
LDFLAGS = -m elf_i386 -T linker.ld

BUILD = build

CSRCS = $(wildcard *.c) $(wildcard lib/*/*.c) $(wildcard cmd/*/*.c)
COBJS = $(patsubst %.c,$(BUILD)/%.o,$(CSRCS))
CDEPS = $(COBJS:.o=.d)

# дополнительные .asm внутри lib/*/ (interrupts.asm и подобные) —
# отдельно от boot.asm/kernel_entry.asm, у которых своя фиксированная роль
ASRCS = $(wildcard lib/*/*.asm)
AOBJS = $(patsubst %.asm,$(BUILD)/%.o,$(ASRCS))

all: $(BUILD)/os-image.bin

$(BUILD)/boot.bin: boot.asm | $(BUILD)
	nasm -f bin boot.asm -o $@

$(BUILD)/kernel_entry.o: kernel_entry.asm | $(BUILD)
	nasm -f elf32 kernel_entry.asm -o $@

# %.o зеркалит путь исходника внутри build/, поэтому
# build/lib/bare/bare.o и build/lib/malloc/malloc.o не конфликтуют
$(BUILD)/%.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.asm | $(BUILD)
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

# kernel_entry.o обязан идти первым — это _start, он должен оказаться
# по адресу 0x1000, куда прыгает boot.asm
$(BUILD)/kernel.bin: $(BUILD)/kernel_entry.o $(COBJS) $(AOBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ --oformat binary $(BUILD)/kernel_entry.o $(COBJS) $(AOBJS)
	truncate -s 16384 $@

$(BUILD)/os-image.bin: $(BUILD)/boot.bin $(BUILD)/kernel.bin
	cat $(BUILD)/boot.bin $(BUILD)/kernel.bin > $@

run: $(BUILD)/os-image.bin
	qemu-system-i386 -drive format=raw,file=$(BUILD)/os-image.bin -display sdl -machine pcspk-audiodev=snd0 -audiodev pa,id=snd0

clean:
	rm -rf $(BUILD)

-include $(CDEPS)

.PHONY: all run clean
