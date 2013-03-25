NAME = libs
VER  = 0.2

FNAME   = ${NAME}-${VER}.so
OBJECTS = *.o
SOURCES = *.c

CCX   = arm-eabi-g++
CC    = arm-eabi-gcc
LD    = arm-eabi-ld
LC_M := LC_MESSAGES=C



CFLAGS = -I../include -W -msoft-float -mlittle-endian -mcpu=arm926ej-s -mtune=arm926ej-s -nostdlib -fshort-wchar \
		-fno-exceptions -fno-builtin -fdata-sections -ffunction-sections -std=gnu99
LDFLAGS = -L../libs -marmelf -z max-page-size=1 -nostdlib --defsym __ex=0 --gc-section -s
LIB_LDFLAGS = ${LDFLAGS} -shared -Bsymbolic -Bsymbolic-function -soname=${FNAME}
ELF_LDFLAGS = ${LDFLAGS} -pie 

LIBS = -lgcc -lcrt -lcrt_helper

CFLAGS += -D__NO_DIRECT_SWI -D__NO_LIBC
CFLAGS_INPUT =

.PHONY = clean

elf_elka: compile_elka link_elf
elf_nsg:  compile_nsg  link_elf
elf_sg:   compile_sg   link_elf

lib_elka: compile_elka link_lib
lib_nsg:  compile_nsg  link_lib
lib_sg:   compile_sg   ling_lib

compile_elka:
	${LC_M} ${CC} ${CFLAGS} -DELKA -DNEWSGOLD -c ${SOURCES}
compile_nsg:
	${LC_M} ${CC} ${CFLAGS} -DNEWSGOLD -c ${SOURCES}
compile_sg:
	${LC_M} ${CC} ${CFLAGS} -DSGOLD -c ${SOURCES}
	
link_elf:
	${LC_M} ${LD} -o ${FNAME} ${OBJECTS} ${ELF_LDFLAGS} ${LIBS}
link_lib:
	${LC_M} ${LD} -o ${FNAME} ${OBJECTS} ${LIB_LDFLAGS} ${LIBS}
clean:
	rm -f $(OBJECTS)
	find ./ -type f -executable -delete
	
