CC = gcc
GCC_INCLUDE = $(shell $(CC) -m32 -print-file-name=include)
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

# rm_video.asm и rm_video_blob.asm собираются отдельными явными правилами —
# исключаем их из общего elf32-конвейера
ASRCS = $(filter-out lib/vid/rm_video.asm lib/vid/rm_video_blob.asm, $(wildcard lib/*/*.asm))
AOBJS = $(patsubst %.asm,$(BUILD)/%.o,$(ASRCS))

all: $(BUILD)/os-image.bin

$(BUILD)/boot.bin: boot.asm | $(BUILD)
	nasm -f bin boot.asm -o $@

$(BUILD)/rm_video.bin: lib/vid/rm_video.asm | $(BUILD)
	nasm -f bin lib/vid/rm_video.asm -o $@

$(BUILD)/lib/vid/rm_video_blob.o: lib/vid/rm_video_blob.asm $(BUILD)/rm_video.bin | $(BUILD)
	@mkdir -p $(dir $@)
	nasm -f elf32 -I$(BUILD)/ lib/vid/rm_video_blob.asm -o $@

$(BUILD)/kernel_entry.o: kernel_entry.asm | $(BUILD)
	nasm -f elf32 kernel_entry.asm -o $@

$(BUILD)/%.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.asm | $(BUILD)
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/kernel.bin: $(BUILD)/kernel_entry.o $(COBJS) $(AOBJS) $(BUILD)/lib/vid/rm_video_blob.o linker.ld
	$(LD) $(LDFLAGS) -o $@ --oformat binary $(BUILD)/kernel_entry.o $(COBJS) $(AOBJS) $(BUILD)/lib/vid/rm_video_blob.o
	truncate -s 16384 $@

$(BUILD)/os-image.bin: $(BUILD)/boot.bin $(BUILD)/kernel.bin
	cat $(BUILD)/boot.bin $(BUILD)/kernel.bin > $@

run: $(BUILD)/os-image.bin
	qemu-system-i386 -drive format=raw,file=$(BUILD)/os-image.bin -display sdl -machine pcspk-audiodev=snd0 -audiodev pa,id=snd0

clean:
	rm -rf $(BUILD)

-include $(CDEPS)
.PHONY: all run clean
