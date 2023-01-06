CLANG_PATH=/Users/czarkos/projects/tools/llvm-sparrow/build/bin/
GCC_PATH=/opt/riscv/riscv64-unknown-elf

CC=$(CLANG_PATH)/clang
LD=/opt/riscv/bin/riscv64-unknown-elf-gcc

CFLAGS=--target=riscv32 -I$(GCC_PATH)/riscv64-unknown-elf/include -I$(GCC_PATH)/include
LDSCRIPT=../../link.ld
OBJCOPY=$(GCC_PATH)/bin/objcopy

all: polynomial.exe ../../../program.hex

clean: 
	rm -f crt0.o printf.o polynomial.o polynomial.exe polynomial.map ../../../program.hex

crt0.o: ../dhry/crt0.s
	$(CC) $(CFLAGS) -c ../dhry/crt0.s

printf.o: ../dhry/printf.c
	$(CC) $(CFLAGS) -c ../dhry/printf.c

polynomial.o: polynomial.c
	$(CC) $(CFLAGS) -c polynomial.c

polynomial.exe: crt0.o printf.o polynomial.o
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=polynomial.map -lgcc -T$(LDSCRIPT) -o polynomial.exe crt0.o polynomial.o printf.o -nostartfiles

../../../program.hex: polynomial.exe
	$(OBJCOPY) -O verilog polynomial.exe ../../../program.hex
