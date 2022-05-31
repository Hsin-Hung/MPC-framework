.PHONY: all clean

export DEFINES =
export CFLAGS = $(DEFINES) -std=c99 -ggdb -O3 -Wall -I../src -fno-omit-frame-pointer
secrecy.ukl: CFLAGS = $(DEFINES) -std=c99 -ggdb -O3 -Wall -I../src -fno-omit-frame-pointer -mno-red-zone -mcmodel=kernel -fno-pic
secrecy-bypass: DEFINES = -DUKL -DUKL_BYPASS
secrecy-shortcut: DEFINES = -DUKL -DUKL_SHORTCUT
secrecy: DEFINES = -DUKL
export LDFLAGS = -lsodium -lm
export CC = gcc
export AR = ar

all:
	$(MAKE) -C src/ all
	$(MAKE) -C tests/ all
	$(MAKE) -C experiments/ all
	$(MAKE) -C examples/ all

clean:
	-rm -f secrecy.ukl
	$(MAKE) -C src/ clean
	$(MAKE) -C tests/ clean
	$(MAKE) -C experiments/ clean
	$(MAKE) -C examples/ clean

libsodium-1.0.18:
		curl https://download.libsodium.org/libsodium/releases/libsodium-1.0.18.tar.gz --output libsodium-1.0.18.tar.gz
		tar xf libsodium-1.0.18.tar.gz
		rm -f tar xf libsodium-1.0.18.tar.gz

libsodium.a: libsodium-1.0.18
		cd libsodium-1.0.18 && ./configure CFLAGS='-ggdb -O2 -mno-red-zone -mcmodel=kernel' \
			--enable-static --disable-shared
		$(MAKE) -C libsodium-1.0.18 $(PARALLEL)
		cp libsodium-1.0.18/src/libsodium/.libs/libsodium.a .

DIR := ${CURDIR}/..
GCC_LIB=$(DIR)/gcc-build/x86_64-pc-linux-gnu/libgcc/
LC_DIR=$(DIR)/glibc-build/
CRT_LIB=$(LC_DIR)csu/
C_LIB=$(LC_DIR)libc.a
PTHREAD_LIB=$(LC_DIR)nptl/libpthread.a
RT_LIB=$(LC_DIR)rt/librt.a
MATH_LIB=$(LC_DIR)math/libm.a
CRT_STARTS=$(CRT_LIB)crt1.o $(CRT_LIB)crti.o $(GCC_LIB)crtbeginT.o
CRT_ENDS=$(GCC_LIB)crtend.o $(CRT_LIB)crtn.o
SYS_LIBS=$(GCC_LIB)libgcc.a $(GCC_LIB)libgcc_eh.a

secrecy.ukl:
	$(MAKE) -C src/ all
	gcc -o exp_group_by.o -c $(CFLAGS) $(DEFINES) experiments/exp_group_by.c
	ld -r -o secrecy.ukl --allow-multiple-definition $(CRT_STARTS) \
		exp_group_by.o --start-group --whole-archive libsecrecy.a libsodium.a \
		$(RT_LIB) $(PTHREAD_LIB) $(MATH_LIB) $(C_LIB) --no-whole-archive \
		$(SYS_LIBS) --end-group $(CRT_ENDS)

secrecy-bypass: secrecy.ukl

secrecy-shortcut: secrecy.ukl

secrecy: secrecy.ukl
