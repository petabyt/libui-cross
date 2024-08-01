#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ui.h>
#include <lualib.h>
#include <lauxlib.h>
#include <lua.h>

#include "demo.h"

// Libui playground
// TODO: Live open - open file, open the file in another editor (remove the textarea) and reload from that file.

extern int luaopen_libuilua(lua_State *L);
static uiWindow *mainwin;
static uiMultilineEntry *editor;

static void openClicked(uiMenuItem *item, uiWindow *w, void *data)
{
	char *filename;

	filename = uiOpenFile(mainwin);
	if (filename == NULL) {
		uiMsgBoxError(mainwin, "No file selected", "Don't be alarmed!");
		return;
	}
	uiMsgBox(mainwin, "File selected", filename);
	uiFreeText(filename);
}

static void saveClicked(uiMenuItem *item, uiWindow *w, void *data)
{
	char *filename;

	filename = uiSaveFile(mainwin);
	if (filename == NULL) {
		uiMsgBoxError(mainwin, "No file selected", "Don't be alarmed!");
		return;
	}
	uiMsgBox(mainwin, "File selected (don't worry, it's still there)", filename);
	uiFreeText(filename);
}

static int onClosing(uiWindow *w, void *data)
{
	uiQuit();
	return 1;
}

static int onShouldQuit(void *data)
{
	uiWindow *mainwin = uiWindow(data);
	uiControlDestroy(uiControl(mainwin));
	return 1;
}

static void run_script_clicked(uiMenuItem *item, uiWindow *w, void *data) {
	char *text = uiMultilineEntryText(editor);

	lua_State *L = luaL_newstate();
	luaL_requiref(L, "ui", luaopen_libuilua, 1);
	luaopen_base(L);

	if (luaL_dostring(L, text)) {
		uiMsgBox(w, "Lua Error", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

#include "favicon.h"

void uiWindowSetIcon(uiWindow *w, const void *data, size_t length);

int main() {

#ifdef _WIN32
	// Redirect stdout
	AttachConsole(-1);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	uiInitOptions o = {0};
	const char *err;

	err = uiInit(&o);
	if (err != NULL) {
		fprintf(stderr, "Error initializing libui-ng: %s\n", err);
		uiFreeInitError(err);
		return 1;
	}

	uiMenu *menu;
	uiMenuItem *item;
	uiBox *box;
	uiBox *hbox;

	menu = uiNewMenu("File");
	item = uiMenuAppendItem(menu, "Open");
	uiMenuItemOnClicked(item, openClicked, NULL);
	item = uiMenuAppendItem(menu, "Save");
	uiMenuItemOnClicked(item, saveClicked, NULL);
	item = uiMenuAppendPreferencesItem(menu);
	item = uiMenuAppendQuitItem(menu);

	menu = uiNewMenu("Script");
	item = uiMenuAppendItem(menu, "Run");
	uiMenuItemOnClicked(item, run_script_clicked, NULL);

	menu = uiNewMenu("Help");
	item = uiMenuAppendItem(menu, "Help");
	item = uiMenuAppendAboutItem(menu);

	mainwin = uiNewWindow("LibUI Demo", 800, 480, 1);

	uiWindowSetIcon(mainwin, libui_ico, libui_ico_len);

	uiWindowSetMargined(mainwin, 0);
	uiWindowOnClosing(mainwin, onClosing, NULL);
	uiOnShouldQuit(onShouldQuit, mainwin);

	hbox = uiNewHorizontalBox();
	uiBoxSetPadded(hbox, 1);

	char *demo_file = malloc(test_lua_len + 1);
	memcpy(demo_file, test_lua, test_lua_len);
	demo_file[test_lua_len] = '\0';

	editor = uiNewNonWrappingMultilineEntry();
	uiBoxAppend(hbox, uiControl(editor), 1);
	uiMultilineEntrySetText(editor, demo_file);

	uiWindowSetChild(mainwin, uiControl(hbox));
	uiControlShow(uiControl(mainwin));

	uiMain();
	uiUninit();

	return 0;
}
