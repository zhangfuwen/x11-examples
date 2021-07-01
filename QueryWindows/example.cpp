#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/Xcms.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define LOGI(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

Bool xerror = False;

Window get_focus_window(Display* d){
	Window w;
	int revert_to;
	printf("getting input focus window ... ");
	XGetInputFocus(d, &w, &revert_to); // see man
	if(xerror){
		printf("fail\n");
		exit(1);
	}else if(w == None){
		printf("no focus window\n");
		exit(1);
	}else{
		printf("success (window: %d)\n", (int)w);
	}

	return w;
}

// get the top window.
// a top window have the following specifications.
//  * the start window is contained the descendent windows.
//  * the parent window is the root window.
Window get_top_window(Display* d, Window start){
	Window w = start;
	Window parent = start;
	Window root = None;
	Window *children;
	unsigned int nchildren;
	Status s;

	printf("getting top window ... \n");
	while (parent != root) {
		w = parent;
		s = XQueryTree(d, w, &root, &parent, &children, &nchildren); // see man

		if (s)
			XFree(children);

		if(xerror){
			printf("fail\n");
			exit(1);
		}

		printf("  get parent (window: %d)\n", (int)w);
	}

	printf("success (window: %d)\n", (int)w);

	return w;
}

int main()
{
    // Open a display.
    Display *d = XOpenDisplay(0);
    if ( d )
    {
#if 1
        //Window w = XDefaultRootWindow(d);
        Window w = RootWindow(d, 0);
        LOGI("root window id:0x%x", w);
        char * name = nullptr;
        Status s = XFetchName(d, w, &name);
        if (s == 0) {
            LOGI("root window name :%s", name);
        }
#else
        char * name = nullptr;
		Window curWin = get_focus_window(d);
        Status s = XFetchName(d, curWin, &name);
        if (s == 0) {
            LOGI("current window name :%s, 0x%x", name, curWin);
        }
		Window w = get_top_window(d, w);
        s = XFetchName(d, w, &name);
        if (s == 0) {
            LOGI("root window name :%s", name);
        }
#endif
        Window rootWindow;
        Window parentWindow;
        Window *childrens;
        unsigned int numChildren;
        Status ret = XQueryTree(d, w, &rootWindow, &parentWindow, &childrens, &numChildren);
        if (ret == 1) {
            LOGI("root window id:0x%x", rootWindow);
            LOGI("parment window id:0x%x", parentWindow);
            LOGE("numChildren %d", numChildren);
            for(unsigned int i = 0; i< numChildren; i++) {
                char * name = nullptr;
                int ret = XFetchName(d, childrens[i], &name);
                if (ret != 1) {
                //    LOGE("failed to fetch name for window #%d", i);
                //    possible, some windows have no name
                }
                XWindowAttributes attrib;
                Status status = XGetWindowAttributes(d, childrens[i], &attrib);
                XClassHint classHint;
                XGetClassHint(d, childrens[i], &classHint);
                XTextProperty prop;
                XGetWMIconName(d, childrens[i], &prop);

                if (status == 1) {
                    if( attrib.x < 10 ) {
                        continue;
                    }
                    LOGI("x:%d,\t y:%d,\t w:%d,\t h:%d,\t %s \t %s \t %s"
                            "\t %s",
                            attrib.x, attrib.y, attrib.width, attrib.height, 
                            classHint.res_name, classHint.res_class, name,
                            prop.value
                            );
                } else {
                    LOGI("window #%d: %s", i, name);
                    LOGI("failed to get get window attributes");
                }

            }

        } else {
            LOGI("root window id:0x%x", rootWindow);
            LOGI("parment window id:0x%x", parentWindow);
            LOGE("numChildren %d", numChildren);
            LOGE("failed to query tree %d", ret);
        }

    }
    return 0;
}

