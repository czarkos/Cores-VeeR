CLANG_PATH=/Users/czarkos/projects/tools/llvm-sparrow/build/bin/
GCC_PATH=/opt/riscv/riscv64-unknown-elf

CC=$(CLANG_PATH)/clang
LD=/opt/riscv/bin/riscv64-unknown-elf-gcc

CFLAGS=--target=riscv32 -I$(GCC_PATH)/riscv64-unknown-elf/include -I$(GCC_PATH)/include
LDSCRIPT=../../link.ld
OBJCOPY=$(GCC_PATH)/bin/objcopy

all: filter_sparrow.exe ../../../program.hex

clean: 
	rm -f crt0.o printf.o filter_sparrow.o filter_sparrow.exe filter_sparrow.map ../../../program.hex

crt0.o: ../dhry/crt0.s
	$(CC) $(CFLAGS) -c ../dhry/crt0.s

printf.o: ../dhry/printf.c
	$(CC) $(CFLAGS) -c ../dhry/printf.c

filter_sparrow.o: filter_sparrow.c
	$(CC) $(CFLAGS) -c filter_sparrow.c

filter_sparrow.exe: crt0.o printf.o filter_sparrow.o
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=filter_sparrow.map -lgcc -T$(LDSCRIPT) -o filter_sparrow.exe crt0.o filter_sparrow.o printf.o -nostartfiles

../../../program.hex: filter_sparrow.exe
	$(OBJCOPY) -O verilog filter_sparrow.exe ../../../program.hex
