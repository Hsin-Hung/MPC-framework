.PHONY: all clean

export DEFINES = -DURING_TCP
export CFLAGS = $(DEFINES) -std=c99 -ggdb -g -O3 -Wall -I../src -fno-omit-frame-pointer
secrecy.ukl: CFLAGS = $(DEFINES) -std=c99 -ggdb -O3 -Wall -I../src -fno-omit-frame-pointer -mno-red-zone -mcmodel=kernel -fno-pic
secrecy-bypass: DEFINES = -DUKL -DUKL_HEADLESS -DUKL_BYPASS
secrecy-shortcut: DEFINES = -DUKL -DUKL_HEADLESS -DUKL_SHORTCUT
secrecy-user: DEFINES = -DUKL_HEADLESS 
secrecy: DEFINES = -DUKL
export LDFLAGS = -lsodium -lm -luring
export CC = gcc
export AR = ar

all:
	$(MAKE) -C src/ all
	$(MAKE) -C tests/ all
	$(MAKE) -C experiments/ all
	$(MAKE) -C examples/ all

clean:
	-rm -f secrecy.ukl UKL.a undefined_sys_hack.o
	$(MAKE) -C src/ clean
	$(MAKE) -C tests/ clean
	$(MAKE) -C experiments/ clean
	$(MAKE) -C examples/ clean

libsodium-1.0.18:
		curl https://download.libsodium.org/libsodium/releases/libsodium-1.0.18.tar.gz --output libsodium-1.0.18.tar.gz
		tar xf libsodium-1.0.18.tar.gz
		rm -f tar xf libsodium-1.0.18.tar.gz

libsodium.a: libsodium-1.0.18
		cd libsodium-1.0.18 && ./configure CFLAGS='-ggdb -O2 -mno-red-zone -mcmodel=kernel -fno-pic' \
			--enable-static --disable-shared
		$(MAKE) -C libsodium-1.0.18 $(PARALLEL)
		cp libsodium-1.0.18/src/libsodium/.libs/libsodium.a .

DIR := ${CURDIR}/..
GCC_LIB=$(DIR)/libgcc-build/x86_64-pc-linux-gnu/libgcc/
LC_DIR=$(DIR)/glibc-build/
CRT_LIB=$(LC_DIR)csu/
C_LIB=$(LC_DIR)libc.a
PTHREAD_LIB=$(LC_DIR)nptl/libpthread.a
RT_LIB=$(LC_DIR)rt/librt.a
MATH_LIB=$(LC_DIR)math/libm.a
CRT_STARTS=$(CRT_LIB)crt1.o $(CRT_LIB)crti.o $(GCC_LIB)crtbeginT.o
CRT_ENDS=$(GCC_LIB)crtend.o $(CRT_LIB)crtn.o
SYS_LIBS=$(GCC_LIB)libgcc.a $(GCC_LIB)libgcc_eh.a

undefined_sys_hack.o: ../undefined_sys_hack.c
	gcc -o $@ -c -ggdb -O2 -fno-omit-frame-pointer -mno-red-zone -mcmodel=kernel -fno-pic $^

secrecy.ukl: libsodium.a undefined_sys_hack.o
	$(MAKE) -C src/ all
	gcc -o exp_group_by.o -c $(CFLAGS) $(DEFINES) experiments/exp_group_by.c
	ld -r -o secrecy.ukl --allow-multiple-definition $(CRT_STARTS) \
		exp_group_by.o --start-group --whole-archive src/libsecrecy.a libsodium.a \
		$(RT_LIB) $(PTHREAD_LIB) $(MATH_LIB) $(C_LIB) --no-whole-archive \
		$(SYS_LIBS) --end-group $(CRT_ENDS)
	ar cr UKL.a secrecy.ukl undefined_sys_hack.o
	objcopy --prefix-symbols=ukl_ UKL.a
	objcopy --redefine-syms=../redef_sym_names UKL.a
	cp UKL.a ../

secrecy-bypass: secrecy.ukl

secrecy-shortcut: secrecy.ukl

secrecy-user: all

secrecy: secrecy.ukl
