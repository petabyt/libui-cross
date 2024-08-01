NAME := ide
SRC := libuilua.c test.c

demo.h: test.lua
	xxd -i test.lua > demo.h
test.c: demo.h

ifeq ($(TARGET),w)

LIBS := -luser32 -lkernel32 -lgdi32 -lcomctl32 -luxtheme -lmsimg32 -lcomdlg32 -ld2d1 -ldwrite -lole32 -loleaut32 -loleacc
LIBS += -lstdc++ -lpthread -lssp -lurlmon -luuid

win.res: a.rc
	x86_64-w64-mingw32-windres -I../libui-ng/windows a.rc -O coff -o win.res

CC := x86_64-w64-mingw32-gcc
$(NAME).exe: $(SRC) win.res Makefile
	$(CC) $(SRC) ../libui_win64.a /usr/x86_64-w64-mingw32/lib/liblua.a $(LIBS) win.res -Wl,-subsystem,windows -s -o $(NAME).exe
endif #endif w

ifeq ($(TARGET),l)
$(NAME).out: $(SRC) Makefile
	$(CC) $(SRC) -lui -ldl $(shell pkg-config --libs --cflags lua-5.3) -o $(NAME).out

$(NAME).AppImage:
	linuxdeploy --appdir=AppDir --executable=ide.out -d ide.desktop -i libui.png
	appimagetool AppDir

endif # endif l

ifeq ($(TARGET),m)
$(error Doh)
endif #endif m

clean:
	rm -rf *.o *.out *.exe *.res
