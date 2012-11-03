CC   := gcc
LD   := ld
ASM  := nasm
RM   := rm
GRUB := grub

AUXFILES := Makefile README

PROJDIRS := src include

CFILES   := $(shell find $(PROJDIRS) -type f -name '*.c')
ASMFILES := $(shell find $(PROJDIRS) -type f -name '*.s')

# Make a list of all of the object files we wish to generate.
OBJFILES := \
    $(patsubst %.c,%.o,$(CFILES)) \
    $(patsubst %.s,%.o,$(ASMFILES))

# For each source file, gcc will create a dependency file, which contains a
# Makefile dependency rule listing that source file's includes.
DEPFILES := \
    $(patsubst %.c,%.d,$(CFILES)) \
    $(patsubst %.s,%.d,$(ASMFILES))

# Just having `-Wall' simply won't cut it!
#WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
#    -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
#    -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
#    -Wuninitialized -Wconversion -Wstrict-prototypes
WARNINGS := -Wall

# gcc flags
CFLAGS := -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
    -I./include $(WARNINGS)

BOCHSFILES := $(GRUB)/stage1 $(GRUB)/stage2 $(GRUB)/pad kernel

# This is what gets executed when we just type `make'.
all: floppy.img

# Create a kernel executable.
floppy.img : kernel
	cat $(BOCHSFILES) > floppy.img

# Link all of the object modules together after they are assembled.
kernel : $(OBJFILES) link.ld
	$(LD) -T link.ld -o kernel $(OBJFILES)

# Assemble all assembly files.
%.o : %.s Makefile
	$(ASM) -f elf -o $@ $<

# Make dependencies listed in the DEPFILES part of our Makefile
-include $(DEPFILES)

# `-MMD' flag generates the dependency (%.d), which will hold rules making the
# generated file (%.o in this case) depend on the source file and any non-system
# header it includes.
%.o : %.c Makefile
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

# `-@' tells make to continue even on error and to be silent
clean :
	$(RM) $(OBJFILES) $(DEPFILES) kernel 2>/dev/null

# Create a phony target so if one of these targets are passed to make, make
# won't check to see if it is actually a file in our currect directory.
.PHONY: all clean dist check
