KDIR=/lib/modules/`uname -r`/build
PWD=$(shell pwd)
VV=1

obj-m := mmap_example.o
module_name-objs := mmap_example.o

all:
	$(MAKE) -C ${KDIR} M=${PWD} V=${VV} modules
clean:
	$(MAKE) -C ${KDIR} M=${PWD} clean
