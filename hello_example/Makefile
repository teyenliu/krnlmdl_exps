KDIR=/lib/modules/`uname -r`/build
PWD=$(shell pwd)
VV=1

obj-m := hello.o
module_name-objs := hello.o

all:
	$(MAKE) -C ${KDIR} M=${PWD} V=${VV} modules
clean:
	$(MAKE) -C ${KDIR} M=${PWD} clean
