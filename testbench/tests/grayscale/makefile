CLANG_PATH=/Users/czarkos/projects/tools/llvm-sparrow/build/bin/
GCC_PATH=/opt/riscv/riscv64-unknown-elf

CC=$(CLANG_PATH)/clang
LD=/opt/riscv/bin/riscv64-unknown-elf-gcc

CFLAGS=--target=riscv32 -I$(GCC_PATH)/riscv64-unknown-elf/include -I$(GCC_PATH)/include
LDSCRIPT=../../link.ld
OBJCOPY=$(GCC_PATH)/bin/objcopy

all: grayscale.exe ../../../program.hex

clean: 
	rm -f crt0.o printf.o grayscale.o grayscale.exe grayscale.map ../../../program.hex

crt0.o: ../dhry/crt0.s
	$(CC) $(CFLAGS) -c ../dhry/crt0.s

printf.o: ../dhry/printf.c
	$(CC) $(CFLAGS) -c ../dhry/printf.c

grayscale.o: grayscale.c variables.h
	$(CC) $(CFLAGS) -c grayscale.c

grayscale.exe: crt0.o printf.o grayscale.o
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=grayscale.map -lgcc -T$(LDSCRIPT) -o grayscale.exe crt0.o grayscale.o printf.o -nostartfiles

../../../program.hex: grayscale.exe
	$(OBJCOPY) -O verilog grayscale.exe ../../../program.hex

hexes: variable4 4.hex 8.hex 16.hex 32.hex 64.hex 128.hex

variable4:
	touch variables.h
	echo "#define N 4" > variables.h

%.hex: crt0.o printf.o
	echo "#define N $*" > variables.h
	$(CC) $(CFLAGS) -c grayscale.c
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=grayscale.map -lgcc -T$(LDSCRIPT) -o grayscale.exe crt0.o grayscale.o printf.o -nostartfiles
	$(OBJCOPY) -O verilog grayscale.exe gray$@
	mkdir gray$*
	cp gray$@ gray$*/program.hex
	mv gray$@ gray$*
