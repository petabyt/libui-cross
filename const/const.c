#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "json.h"
#include "const.h"

typedef struct uiScroll uiScroll;
_UI_EXTERN uiScroll *uiNewScroll();
_UI_EXTERN void uiScrollSetChild(uiScroll *scroll, uiControl *ctl);

void uiLabelSetTextSize(uiLabel *l, float size) {
	
}

static uiScroll *get_articles(const char *constitution) {
    struct Parse p;
    uiLabel *lbl;

    uiScroll *scroll = uiNewScroll();

    uiBox *box = uiNewVerticalBox();
    uiBoxSetPadded(box, 1);

    uiScrollSetChild(scroll, uiControl(box));

    lbl = uiNewLabel("We the people");
    uiLabelSetTextSize(lbl, 25.0);
    uiBoxAppend(box, uiControl(lbl), 0);

    json_get(&p, constitution, "['preamble']");
    char *buf = json_fixup_string(&p);
    uiBoxAppend(box, uiControl(uiNewLabel(buf + 14)), 0);
    free(buf);

    int lengths[] = {10, 4, 3, 4, 1, 1};
    for (int s = 0; s < 6; s++) {
        for (int i = 0; i < lengths[s]; i++) {
            char req[64];
            snprintf(req, sizeof(req), "['articles'][%d][%d]", s, i);

            json_get(&p, constitution, req);

            buf = json_fixup_string(&p);
            uiBoxAppend(box, uiControl(uiNewLabel(buf)), 0);
            free(buf);
        }

        uiBoxAppend(box, uiControl(uiNewVerticalSeparator()), 0);
    }

    lbl = uiNewLabel("Signatories");
    uiLabelSetTextSize(lbl, 20.0);
    uiBoxAppend(box, uiControl(lbl), 0);

    json_get(&p, constitution, "['signatories']");
    buf = json_fixup_string(&p);
    uiBoxAppend(box, uiControl(uiNewLabel(buf)), 0);
    free(buf);

    return scroll;
}

static uiScroll *get_amendments(const char *constitution) {
    struct Parse p;
    uiScroll *scroll = uiNewScroll();

    uiBox *box = uiNewVerticalBox();
    uiBoxSetPadded(box, 1);

    uiScrollSetChild(scroll, uiControl(box));

    for (int i = 0; i < 27; i++) {
        char req[64];
        snprintf(req, sizeof(req), "Amendment #%d", i + 1);
        uiLabel *lbl = uiNewLabel(req);
        uiLabelSetTextSize(lbl, 20.0);
        uiBoxAppend(box, uiControl(lbl), 0);

        snprintf(req, sizeof(req), "['amendments'][%d]", i);

        json_get(&p, constitution, req);

        char *buf = json_fixup_string(&p);
        uiBoxAppend(box, uiControl(uiNewLabel(buf)), 0);
        free(buf);

        uiBoxAppend(box, uiControl(uiNewVerticalSeparator()), 0);
    }

    return scroll;
}

static uiTab *get_app(const char *constitution) {
    uiLabel *lbl;
    uiBox *box;

    uiTab *tab = uiNewTab();

    uiTabAppend(tab, "Articles", uiControl(get_articles(constitution)));
    uiTabAppend(tab, "Amendments", uiControl(get_amendments(constitution)));

    uiBox *about = uiNewVerticalBox();
    uiBoxSetPadded(about, 1);
    uiBoxAppend(about, uiControl(uiNewLabel("Created with LibUI")), 0);
    uiBoxAppend(about, uiControl(uiNewLabel("Compiled for all platforms, this APK is 59kb."
                                            " Download size for single arch is 54kb.")), 0);
    uiBoxAppend(about, uiControl(uiNewLabel("All the UI you see here was created in C, and the exact"
                                            " same code can be run on Windows, Linux, and MacOS.")), 0);
    uiBoxAppend(about, uiControl(uiNewButton("That's neat")), 0);

    uiTabAppend(tab, "About", uiControl(about));

    return tab;
}

int on_closing(uiWindow *w, void *data)
{
	// TODO: close down threads
	uiQuit();
	return 1;
}

int onClosing(uiWindow *w, void *data)
{
	uiQuit();
	return 1;
}

int main(void)
{
	uiInitOptions o;
	uiWindow *w;
	uiScroll *s;
	uiBox *box;

	puts("Hello, WOrld");

	memset(&o, 0, sizeof (uiInitOptions));
	if (uiInit(&o) != NULL) {
		puts("ui err");
		return 1;
	}

	w = uiNewWindow("Scrolling", 320, 240, 0);
	s = uiNewScroll();
	box = uiNewVerticalBox();
	uiScrollSetChild(s, uiControl(box));
	uiBoxSetPadded(box, 1);

	for (int i = 0; i < 32; i++) {
		uiBoxAppend(box, uiControl(uiNewButton("Button")), 0);
	}

	uiWindowOnClosing(w, onClosing, NULL);
	uiWindowSetChild(w, uiControl(s));
	uiControlShow(uiControl(w));
	uiMain();
	return 0;
}

int main2() {
	uiInitOptions o;
	uiWindow *w;

	// libui sets this thread as "single-thread apartment"
	memset(&o, 0, sizeof(uiInitOptions));
	if (uiInit(&o) != NULL) {
		puts("uiInit(&o) != NULL, abort");
		abort();
	}

	w = uiNewWindow("Hello, World", 800, 500, 0);
	uiWindowSetMargined(w, 1);

	uiTab *app = get_app((char *)const_txt);

	uiWindowSetChild(w, uiControl(app));

	uiWindowOnClosing(w, on_closing, NULL);
	uiControlShow(uiControl(w));
	uiMain();
	
	return 0;
}
