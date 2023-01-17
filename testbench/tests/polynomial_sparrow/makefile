CLANG_PATH=/Users/czarkos/projects/tools/llvm-sparrow/build/bin/
GCC_PATH=/opt/riscv/riscv64-unknown-elf

CC=$(CLANG_PATH)/clang
LD=/opt/riscv/bin/riscv64-unknown-elf-gcc

CFLAGS=--target=riscv32 -I$(GCC_PATH)/riscv64-unknown-elf/include -I$(GCC_PATH)/include
LDSCRIPT=../../link.ld
OBJCOPY=$(GCC_PATH)/bin/objcopy

all: polynomial_sparrow.exe ../../../program.hex

clean: 
	rm -f crt0.o printf.o polynomial_sparrow.o polynomial_sparrow.exe polynomial_sparrow.map ../../../program.hex

crt0.o: ../dhry/crt0.s
	$(CC) $(CFLAGS) -c ../dhry/crt0.s

printf.o: ../dhry/printf.c
	$(CC) $(CFLAGS) -c ../dhry/printf.c

polynomial_sparrow.o: polynomial_sparrow.c variables.h
	$(CC) $(CFLAGS) -c polynomial_sparrow.c

polynomial_sparrow.exe: crt0.o printf.o polynomial_sparrow.o
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=polynomial_sparrow.map -lgcc -T$(LDSCRIPT) -o polynomial_sparrow.exe crt0.o polynomial_sparrow.o printf.o -nostartfiles

../../../program.hex: polynomial_sparrow.exe
	$(OBJCOPY) -O verilog polynomial_sparrow.exe ../../../program.hex

hexes: variable4 4.hex 8.hex 16.hex 32.hex 64.hex 128.hex

hexes2: variable256 256.hex 512.hex 1024.hex 2048.hex 4096.hex 8192.hex

variable4:
	touch variables.h
	echo "#define N 4" > variables.h

variable256:
	touch variables.h
	echo "#define N 256" > variables.h

%.hex: crt0.o printf.o
	touch variables.h
	echo "#define N $*" > variables.h
	$(CC) $(CFLAGS) -c polynomial_sparrow.c
	$(LD) -mabi=ilp32 -march=rv32imc -Wl,-Map=polynomial_sparrow.map -lgcc -T$(LDSCRIPT) -o polynomial_sparrow.exe crt0.o polynomial_sparrow.o printf.o -nostartfiles
	$(OBJCOPY) -O verilog polynomial_sparrow.exe poly_sprw$@
	mkdir poly_sprw$*
	cp poly_sprw$@ poly_sprw$*/program.hex
	mv poly_sprw$@ poly_sprw$*
