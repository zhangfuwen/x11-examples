#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/Xcms.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <functional>

#define LOGI(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

class LotDock {
public:
    void getWindowList()
    {
        wnck_screen_force_update(screen);
        GList * windows = wnck_screen_get_windows(screen);
        g_list_foreach(windows, [](gpointer w, gpointer data) {
                WnckWindow * window = (WnckWindow *)w;
                const char * name = wnck_window_get_icon_name(window);
                LOGI("window name %s", name);

        }, 0);

    }
    static void on_window_opened(WnckScreen *screen, WnckWindow * window, gpointer data)
    {
        LOGI("on_window_opened");
        if(window) {
            LOGI("%s opened", wnck_window_get_name(window));
        }
    }

    static void on_active_window_changed(WnckScreen *screen, WnckWindow *prevWindow, gpointer data)
    {
        LOGI("on_active_window_changed");
        WnckWindow *active_window = wnck_screen_get_active_window(screen);
        if(prevWindow) {
            LOGI("curretWindowDeactivated:%s", wnck_window_get_name(prevWindow));
        }
        if (active_window) {
            LOGI("active windows:%s", 
                    wnck_window_get_name(active_window));
        } 
    }

    static void on_window_closed(WnckScreen *screen, WnckWindow * window, gpointer data)
    {
        LOGI("on_window_closed");
        if (window) {
            LOGI("close window:%s", 
                    wnck_window_get_name(window));
        } 


    }

    WnckScreen * screen = wnck_screen_get_default();
};


int main(int argc, char **argv)
{

    gdk_init(&argc, &argv);
//    dock.getWindowList();
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    LotDock dock;

    g_signal_connect(dock.screen, "window-opened" , 
            G_CALLBACK(LotDock::on_window_opened), NULL);

    g_signal_connect(dock.screen, "active-window-changed" , 
            G_CALLBACK(LotDock::on_active_window_changed), NULL);
    g_signal_connect(dock.screen, "on-window-closed" , 
            G_CALLBACK(LotDock::on_window_closed), NULL);

    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    return 0;
}

