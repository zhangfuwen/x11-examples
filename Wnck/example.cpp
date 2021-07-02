#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/Xcms.h>
#include <cstddef>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <functional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define LOGI(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

struct LotWindow {
    LotWindow(const char * s, gulong id) : name(s), xid(id) {}
    std::string name;
    gulong xid;
};

struct LotGroup {
    LotGroup(const char *s):name(s) {};
    std::string name;
};
bool operator<(const LotGroup me, const LotGroup other ) {
    return me.name < other.name;
}

struct DTO {
    std::map<LotGroup,std::forward_list<LotWindow>> dock;
};

void to_json(json &j, const DTO& dto)
{
    for(auto [group, windows] : dto.dock)
    {
        j[group.name] = json::array({});

        for(const auto & w : windows) {
            j[group.name].emplace_back(json::array({std::to_string(w.xid), w.name}));
        }
    }
}

void from_json(json &j, DTO& dto) {
    for(auto [k, v] : j.items()) {
        auto & group = dto.dock[LotGroup(k.c_str())];
        for(const auto & [id, name ] : v.items()) {
            std::string n = name.get<std::string>();
            group.emplace_front(n.c_str(), std::atoi(id.c_str()));
        }
    }
}


class LotDock {
public:
    void getWindowList(WnckScreen * screen)
    {

        LOGI("getWindowList start");
        DTO dto;
        auto & dock = dto.dock;
        GList * winListNode = wnck_screen_get_windows(screen);
        while(winListNode != nullptr) {
                WnckWindow * window = (WnckWindow *)(winListNode->data);
                if(!WNCK_IS_WINDOW(window)) {
                    winListNode = winListNode->next;
                    continue;
                }
                const char * name = wnck_window_get_icon_name(window);
                gulong xid = wnck_window_get_xid(window);
                LOGI("window name %s", name);
                const char * groupName = wnck_window_get_class_group_name(window);
                dock[LotGroup(groupName)].emplace_front(name, xid);
                winListNode = winListNode->next;
        }
        json j = dto;
        std::string str = j.dump();
        LOGI("window string:%s", str.c_str());
        LOGI("getWindowList end");
    }

    static void on_window_opened(WnckScreen *screen, WnckWindow * window, gpointer data)
    {
        LotDock * dock = (LotDock *)data;
        LOGI("on_window_opened");
        if(window) {
            LOGI("%s opened", wnck_window_get_name(window));
        }
    }

    static void on_active_window_changed(WnckScreen *screen, WnckWindow *prevWindow, gpointer data)
    {
        LotDock * dock = (LotDock *)data;
        LOGI("on_active_window_changed");
        WnckWindow *active_window = wnck_screen_get_active_window(screen);
        if(prevWindow) {
            LOGI("curretWindowDeactivated:%s", wnck_window_get_name(prevWindow));
        }
        if (active_window) {
            LOGI("active windows:%s", 
                    wnck_window_get_name(active_window));
        } 
        dock->getWindowList(screen);
    }

    static void on_window_closed(WnckScreen *screen, WnckWindow * window, gpointer data)
    {
        LotDock * dock = (LotDock *)data;
        LOGI("on_window_closed");
        if (window) {
            LOGI("close window:%s", 
                    wnck_window_get_name(window));
        } 


    }

};


int main(int argc, char **argv)
{

    gdk_init(&argc, &argv);
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    WnckScreen * screen = wnck_screen_get_default();
    wnck_screen_force_update(screen);

    LotDock dock;

    g_signal_connect(screen, "window-opened" , 
            G_CALLBACK(LotDock::on_window_opened), &dock);

    g_signal_connect(screen, "active-window-changed" , 
            G_CALLBACK(LotDock::on_active_window_changed), &dock);
    g_signal_connect(screen, "on-window-closed" , 
            G_CALLBACK(LotDock::on_window_closed), &dock);

    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    return 0;
}

