LIBUI := $(abspath libui-ng)

LIBUI_COMMON:=$(patsubst %.c,%.o,$(wildcard $(LIBUI)/common/*.c))
LIBUI_COMMON:=$(filter-out %OLD_table.o,$(LIBUI_COMMON))

CFLAGS=-I$(LIBUI) -g

all:
	echo "see Makefile"

ide/demo.h: ide/test.lua
	cd ide && xxd -i test.lua > demo.h
ide/test.c: ide/demo.h

ifeq ($(TARGET),w) # ++++++++++++++++++++++++++
LIBUI_COMMON += extras/favicon/win.o extras/scroll.o extras/win_init.o
include win.mk
endif # ------------------------------

ifeq ($(TARGET),l) # ++++++++++++++++++++++++++
LIBUI_COMMON += extras/favicon/linux.o extras/label.o

LIBUI_UNIX := $(patsubst %.c,%.o,$(wildcard $(LIBUI)/unix/*.c))
LIBUI_UNIX := $(filter-out %OLD_table.o,$(LIBUI_UNIX))
LIBUI_UNIX := $(filter-out %image.o,$(LIBUI_UNIX))
LIBUI_UNIX += extras/image/image.o

O_FILES := $(LIBUI_COMMON) $(LIBUI_UNIX)
O_FILES := $(O_FILES:.o=.$(TARGET).o)

CFLAGS += $(shell pkg-config --cflags gtk+-3.0) -fPIC
LDFLAGS += $(shell pkg-config --libs gtk+-3.0)

libui.so: $(O_FILES)
	$(CC) -shared $(O_FILES) $(LDFLAGS) -o libui.so

install: libui.so
	sudo rm -rf /usr/local/lib/x86_64-linux-gnu/libui.so
	sudo cp libui.so /usr/local/lib/x86_64-linux-gnu/libui.so
	sudo rm -rf /usr/lib/libui.so
	sudo cp libui.so /usr/lib/libui.so
	sudo cp $(LIBUI)/ui.h /usr/include/ui.h

endif # ------------------------------------

ifeq ($(TARGET),m) # ++++++++++++++++++++++++++
LIBUI_COMMON += extras/favicon/darwin.o

LIBUI_DARWIN := $(patsubst %.m,%.o,$(wildcard $(LIBUI)/darwin/*.m))
LIBUI_DARWIN := $(filter-out %OLD_table.o,$(LIBUI_DARWIN))

O_FILES := $(LIBUI_COMMON) $(LIBUI_DARWIN)
O_FILES := $(O_FILES:.o=.$(TARGET).o)

LDFLAGS := -framework Foundation -framework Appkit -framework CoreText -framework CoreGraphics -Wl,-undefined,dynamic_lookup

libui.dylib: $(O_FILES)
	$(CC) -shared $(O_FILES) $(LDFLAGS) -o libui.dylib

define COMPILE
$(2).out: $(1) libui.dylib
	$(CC) $(CFLAGS) $(1) -L. -lui -o $(2).out
endef

install: libui.dylib
	cp libui.dylib /usr/local/lib
	cp $(LIBUI)/ui.h /usr/local/include

endif # ---------------------

%.$(TARGET).o: %.m
	$(CC) -c $< $(CFLAGS) -o $@

%.$(TARGET).o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

%.$(TARGET).o: %.cpp
	$(CPP) -c $< $(CFLAGS) -o $@

clean:
	make TARGET=l clean_
	make TARGET=w clean_
	make TARGET=m clean_

clean_:
	$(RM) $(O_FILES) *.o *.exe *.out *.res *.so *.a build *.dylib example/*.out ide/*.o *.res
