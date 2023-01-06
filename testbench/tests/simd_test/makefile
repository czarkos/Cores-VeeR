CLANG_PATH=/Users/czarkos/projects/tools/llvm-sparrow/build/bin/
GCC_PATH=/opt/riscv/riscv64-unknown-elf

CC=$(CLANG_PATH)/clang
LD=/opt/riscv/bin/riscv64-unknown-elf-gcc

CFLAGS=--target=riscv32 -I$(GCC_PATH)/riscv64-unknown-elf/include -I$(GCC_PATH)/include
LDSCRIPT=../../link.ld
OBJCOPY=$(GCC_PATH)/bin/objcopy

all: simd_test.exe ../../../program.hex

clean: 
	rm -f crt0.o printf.o simd_test.o simd_test.exe simd_test.map ../../../program.hex

crt0.o: ../dhry/crt0.s
	$(CC) $(CFLAGS) -c ../dhry/crt0.s

printf.o: ../dhry/printf.c
	$(CC) $(CFLAGS) -c ../dhry/printf.c

simd_test.o: simd_test.c
	$(CC) $(CFLAGS) -c simd_test.c

simd_test.exe: crt0.o printf.o simd_test.o
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=simd_test.map -lgcc -T$(LDSCRIPT) -o simd_test.exe crt0.o simd_test.o printf.o -nostartfiles

../../../program.hex: simd_test.exe
	$(OBJCOPY) -O verilog simd_test.exe ../../../program.hex
