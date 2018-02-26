#STAGING_INCLUDE=$ROOTFS/../staging/usr/include
#PKG_CONFIG_PATH=$(ROOTFS)/usr/lib/arm-linux-gnueabihf/pkgconfig
CC=$(CROSS_COMPILE)gcc
CFLAGS=-g -c -Wall -DLINUX
LDFLAGS= -L$(ROOTFS)/usr/lib -lm -L$(ROOTFS)/usr/lib/arm-linux-gnueabihf/ -lpthread
CFLAGS+=-I$(PWD) -I/usr/include/
		
SOURCES=simple_read.c sysfs_helper.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mpu_reader

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	@echo $(LDFLAGS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o mpu_reader

install:
	cp $(EXECUTABLE) $(ROOTFS)/usr/bin
