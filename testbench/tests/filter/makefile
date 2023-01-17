CLANG_PATH=/Users/czarkos/projects/tools/llvm-sparrow/build/bin/
GCC_PATH=/opt/riscv/riscv64-unknown-elf

CC=$(CLANG_PATH)/clang
LD=/opt/riscv/bin/riscv64-unknown-elf-gcc

CFLAGS=--target=riscv32 -I$(GCC_PATH)/riscv64-unknown-elf/include -I$(GCC_PATH)/include
LDSCRIPT=../../link.ld
OBJCOPY=$(GCC_PATH)/bin/objcopy

all: filter.exe ../../../program.hex

clean: 
	rm -f crt0.o printf.o filter.o filter.exe filter.map ../../../program.hex

crt0.o: ../dhry/crt0.s
	$(CC) $(CFLAGS) -c ../dhry/crt0.s

printf.o: ../dhry/printf.c
	$(CC) $(CFLAGS) -c ../dhry/printf.c

filter.o: filter.c variables.h
	$(CC) $(CFLAGS) -c filter.c

filter.exe: crt0.o printf.o filter.o
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=filter.map -lgcc -T$(LDSCRIPT) -o filter.exe crt0.o filter.o printf.o -nostartfiles

../../../program.hex: filter.exe
	$(OBJCOPY) -O verilog filter.exe ../../../program.hex

hexes: variable4 4.hex 8.hex 16.hex 32.hex 64.hex 128.hex

variable4:
	touch variables.h
	echo "#define N 4" > variables.h

%.hex: crt0.o printf.o
	echo "#define N $*" > variables.h
	$(CC) $(CFLAGS) -c filter.c
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=filter.map -lgcc -T$(LDSCRIPT) -o filter.exe crt0.o filter.o printf.o -nostartfiles
	$(OBJCOPY) -O verilog filter.exe filter$@
	mkdir filter$*
	cp filter$@ filter$*/program.hex
	mv filter$@ filter$*
