CLANG_PATH=/Users/czarkos/projects/tools/llvm-sparrow/build/bin/
GCC_PATH=/opt/riscv/riscv64-unknown-elf

CC=$(CLANG_PATH)/clang
LD=/opt/riscv/bin/riscv64-unknown-elf-gcc

CFLAGS=--target=riscv32 -I$(GCC_PATH)/riscv64-unknown-elf/include -I$(GCC_PATH)/include
LDSCRIPT=../../link.ld
OBJCOPY=$(GCC_PATH)/bin/objcopy

all: mat_mul_sparrow.exe ../../../program.hex

clean: 
	rm -f crt0.o printf.o mat_mul_sparrow.o mat_mul_sparrow.exe mat_mul_sparrow.map ../../../program.hex

crt0.o: ../dhry/crt0.s
	$(CC) $(CFLAGS) -c ../dhry/crt0.s

printf.o: ../dhry/printf.c
	$(CC) $(CFLAGS) -c ../dhry/printf.c

mat_mul_sparrow.o: mat_mul_sparrow.c variables.h
	$(CC) $(CFLAGS) -c mat_mul_sparrow.c

mat_mul_sparrow.exe: crt0.o printf.o mat_mul_sparrow.o
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=mat_mul_sparrow.map -lgcc -T$(LDSCRIPT) -o mat_mul_sparrow.exe crt0.o mat_mul_sparrow.o printf.o -nostartfiles

../../../program.hex: mat_mul_sparrow.exe
	$(OBJCOPY) -O verilog mat_mul_sparrow.exe ../../../program.hex

hexes: variable4 4.hex 8.hex 16.hex 32.hex 64.hex 128.hex

variable4:
	touch variables.h
	echo "#define N 4" > variables.h

%.hex: crt0.o printf.o
	echo "#define N $*" > variables.h
	$(CC) $(CFLAGS) -c mat_mul_sparrow.c
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=mat_mul_sparrow.map -lgcc -T$(LDSCRIPT) -o mat_mul_sparrow.exe crt0.o mat_mul_sparrow.o printf.o -nostartfiles
	$(OBJCOPY) -O verilog mat_mul_sparrow.exe mat_mul_sprw$@
	mkdir mat_mul_sparrow$*
	cp mat_mul_sprw$@ mat_mul_sparrow$*/program.hex
	mv mat_mul_sprw$@ mat_mul_sparrow$*
