.PHONY: all clean

export DEFINES = -DURING_TCP
export CFLAGS = $(DEFINES) -std=c99 -ggdb -g -O3 -Wall -I../src -fno-omit-frame-pointer
secrecy.o: CFLAGS = $(DEFINES) -std=c99 -ggdb -O3 -Wall -I../src -fno-omit-frame-pointer -mno-red-zone -mcmodel=kernel -fno-pic
secrecy-bypass: DEFINES = -DUKL -DUKL_HEADLESS -DUKL_BYPASS
secrecy-shortcut: DEFINES = -DUKL -DUKL_HEADLESS -DUKL_SHORTCUT
secrecy-user: DEFINES = -DUKL_HEADLESS 
secrecy: DEFINES = -DUKL -DUKL_HEADLESS
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

lib: experiments/exp_group_by.c
	gcc -o exp_group_by.o -c $(CFLAGS) $(DEFINES) experiments/exp_group_by.c
	$(MAKE) -C src/ all

secrecy-bypass: lib

secrecy-shortcut: lib

secrecy-user: all

secrecy: lib
