ifeq (,$(CROSS_COMPILE))
$(error missing CROSS_COMPILE for this toolchain)
endif

TARGET = cp_bench
CC = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip
CFLAGS = -marm -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7ve -Wall -std=gnu99 -Ofast

INCLUDE_PATHS = -I/opt/miyoomini-toolchain/arm-linux-gnueabihf/libc/usr/include \
                -Ineon-arm-library-miyoo/include
LIBRARY_PATH = /opt/miyoomini-toolchain/arm-linux-gnueabihf/libc/usr/lib
SUBMODULE_LIB_PATH = neon-arm-library-miyoo/lib

LDFLAGS = -L$(LIBRARY_PATH) -L$(SUBMODULE_LIB_PATH) -lneonarmmiyoo -lmi_sys

all: build_submodule $(TARGET)

build_submodule:
	$(MAKE) -C neon-arm-library-miyoo || (echo "Submodule library build failed!" && exit 1)

$(TARGET): main.o
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) main.o -o $(TARGET) $(LDFLAGS)
	$(STRIP) $(TARGET)

main.o: src/main.c
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) -c src/main.c -o main.o

clean:
	rm -f $(TARGET) main.o
	$(MAKE) -C neon-arm-library-miyoo clean

dist: all
	mkdir -p ./dist/bin ./dist/lib
	cp $(TARGET) ./dist/bin/
	cp $(SUBMODULE_LIB_PATH)/libneonarmmiyoo.so ./dist/lib/
