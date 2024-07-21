obj-m += test_1.o 

PWD := $(CURDIR) 

CC=gcc-12

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
