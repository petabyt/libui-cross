# libui-dev
A portable UI library written in C, which uses the native UI of the operating system it runs on.  
Like wxWidgets, but in C. And much more bare-bones.

This is a fork of libui-ng with some extras:
- [x] uiScroll
- [x] uiWindowSetIcon
- [x] DPI scaling (4k)
- [ ] uiImage as a widget
- [ ] Tooltips

```
# Compile for MinGW from Linux
make TARGET=w libui_win64.a install

# Compile for linux from linux
make TARGET=l libui.so install

# Compile for Mac from Darling/Mac
make TARGET=m libui.dylib
```

- [x] X86_64 Linux (.so, .AppImage)
- [x] X86_64 Windows (.a, .exe)
- [ ] X86_64 MacOS (.dylib, .app)
- [ ] ARM64 Linux
- [ ] ARM64 MacOS
