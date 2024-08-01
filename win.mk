LIBUI_WIN := $(patsubst %.cpp,%.o,$(wildcard $(LIBUI)/windows/*.cpp))
LIBUI_WIN := $(filter-out OLD_,$(LIBUI_WIN))

O_FILES := $(LIBUI_COMMON) $(LIBUI_WIN)
O_FILES := $(O_FILES:.o=.$(TARGET).o)

CPP := x86_64-w64-mingw32-c++
CC := x86_64-w64-mingw32-gcc

# Yes, lots of libs required
LIBS := -luser32 -lkernel32 -lgdi32 -lcomctl32 -luxtheme -lmsimg32 -lcomdlg32 -ld2d1 -ldwrite -lole32 -loleaut32 -loleacc
LIBS += -lstdc++ -lgcc -lpthread -lssp -lurlmon -luuid

CFLAGS += -I$(LIBUI)/windows -fvisibility=hidden -fdiagnostics-color=always -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wpedantic -std=c++11 -Wno-unused-parameter -Wno-switch -D_UI_STATIC -Dlibui_EXPORTS

libui_win64.a: $(O_FILES)
	x86_64-w64-mingw32-ar rsc libui_win64.a $(O_FILES)

libui_win64.dll: $(O_FILES)
	$(CPP) -shared $(O_FILES) $(LIBS) -o libui_win64.dll

install: libui_win64.a libui_win64.dll
	cp libui_win64.a /usr/x86_64-w64-mingw32/lib/libui.a
	cp libui_win64.dll /usr/x86_64-w64-mingw32/lib/libui_win64.dll
	cp $(LIBUI)/ui.h /usr/x86_64-w64-mingw32/include/

build: libui_win64.a
	-mkdir build
	cd $(LIBUI) && meson setup build && ninja -C build
	cp $(LIBUI)/build/meson-out/libui.so build/libui_linux64.so
	cp $(LIBUI)/ui.h build/ui.h
	cp libui_win64.a build/libui_win64.a

ide/ide.res: ide/a.rc
	x86_64-w64-mingw32-windres -I$(LIBUI)/windows ide/a.rc -O coff -o ide/ide.res

win.res: example/a.rc
	x86_64-w64-mingw32-windres -I$(LIBUI)/windows example/a.rc -O coff -o win.res

TESTER_FILES := $(addprefix $(LIBUI)/test/,drawtests.w.o images.w.o main.w.o menus.w.o page1.w.o page2.w.o page3.w.o page4.w.o page5.w.o page6.w.o page7.w.o page7a.w.o page7b.w.o page7c.w.o page11.w.o page12.w.o page13.w.o page14.w.o page15.w.o page16.w.o page17.w.o spaced.w.o)

tester.exe: $(TESTER_FILES) win.res libui_win64.a
	$(CC) -static $(TESTER_FILES) libui_win64.a win.res $(LIBS) -o tester.exe

