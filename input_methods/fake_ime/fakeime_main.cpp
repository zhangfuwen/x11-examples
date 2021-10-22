#include <glib-2.0/glib.h>
#include <glib-object.h>
#include <glib/gstdio.h>
#include <ibus.h>
#include <stdio.h>

#define LOG_INFO(fmt, ...)                                                           \
    do                                                                               \
    {                                                                                \
        printf("%s:%d %s > " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        g_info("%s:%d %s > " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__);      \
    } while (0)

#define LOG_DEBUG LOG_INFO
#define LOG_WARN LOG_INFO
#define LOG_ERROR LOG_INFO

static void sigterm_cb(int sig)
{
    LOG_ERROR("sig term %d", sig);
    exit(-1);
}

static void ibus_disconnected_cb(IBusBus *bus, gpointer user_data)
{
    ibus_quit();
}

static IBusLookupTable *table = nullptr;
void engine_reset(IBusEngine * engine, IBusLookupTable * table) {
  ibus_lookup_table_clear(table);
  ibus_engine_hide_preedit_text(engine);
  ibus_engine_hide_auxiliary_text(engine);
  ibus_engine_hide_lookup_table(engine);
}

void engine_commit_text(IBusEngine * engine, IBusText * text) {
  ibus_engine_commit_text(engine, text);
  engine_reset(engine, table);
}

gboolean engine_process_key_event_cb(IBusEngine *engine,
                                     guint keyval,
                                     guint keycode,
                                     guint state)
{
    LOG_INFO("engine_process_key_event");
    ibus_engine_show_lookup_table(engine);
    ibus_engine_show_auxiliary_text(engine);

    if(state & IBUS_RELEASE_MASK) {
        return FALSE;
    }


    switch (keyval)
    {
    case IBUS_KEY_space: 
        engine_commit_text(engine, ibus_text_new_from_string("搜索语言"));
        break;
    
    default:
        ibus_lookup_table_append_candidate(table, ibus_text_new_from_string("hello"));
        ibus_lookup_table_append_candidate(table, ibus_text_new_from_string("hello"));
        // ibus_engine_update_preedit_text(engine, ibus_text_new_from_string("xx"), 2, TRUE);
        ibus_lookup_table_set_cursor_pos(table, 0);
        break;
    }

    //ibus_lookup_table_clear(table);
    //ibus_engine_hide_lookup_table(engine);
}



void engine_enable_cb(IBusEngine *engine)
{
    LOG_INFO("[IM:iBus]: IM enabled\n");
        // Setup Lookup table
    table = ibus_lookup_table_new(10, 0, TRUE, TRUE);
    LOG_INFO("table %p", table);
    g_object_ref_sink(table);

    ibus_lookup_table_set_orientation(table, IBUS_ORIENTATION_HORIZONTAL);
    ibus_engine_show_lookup_table(engine);
    ibus_engine_show_auxiliary_text(engine);
}

void engine_disable_cb(IBusEngine *engine)
{
    LOG_INFO("[IM:iBus]: IM disabled\n");
}

void engine_focus_out_cb(IBusEngine *engine)
{
    LOG_INFO("[IM:iBus]: IM Focus out\n");

    //   if(riti_context_ongoing_input_session(ctx)) {
    //     riti_context_finish_input_session(ctx);
    //     engine_reset();
    //   }
}

void engine_candidate_clicked_cb(IBusEngine *engine, guint index, guint button, guint state)
{
    LOG_INFO("[IM:iBus]: candidate clicked\n");
    ibus_engine_commit_text(engine, ibus_text_new_from_string("sss"));
    //   IBusText *text = ibus_lookup_table_get_candidate(table, index);
    //   riti_context_candidate_committed(ctx, index);
}

static gint id = 0;
static IBusEngine *engine = nullptr;
IBusBus *bus;
IBusEngine *create_engine_cb(IBusFactory *factory,
                             gchar *engine_name,
                             gpointer user_data)
{
    id += 1;
    gchar *path = g_strdup_printf("/org/freedesktop/IBus/Engine/%i", id);
    engine = ibus_engine_new(engine_name,
                             path,
                             ibus_bus_get_connection(bus));

    // Setup Lookup table
    LOG_INFO("[IM:iBus]: Creating IM Engine\n");
    LOG_INFO("[IM:iBus]: Creating IM Engine with name:%s and id:%d\n", engine_name, id);

    g_signal_connect(engine, "process-key-event", G_CALLBACK(engine_process_key_event_cb), NULL);
    g_signal_connect(engine, "enable", G_CALLBACK(engine_enable_cb), NULL);
    g_signal_connect(engine, "disable", G_CALLBACK(engine_disable_cb), NULL);
    g_signal_connect(engine, "focus-out", G_CALLBACK(engine_focus_out_cb), NULL);
    g_signal_connect(engine, "candidate-clicked", G_CALLBACK(engine_candidate_clicked_cb), NULL);

    return engine;
}

int main(gint argc, gchar **argv)
{
    signal(SIGTERM, sigterm_cb);
    signal(SIGINT, sigterm_cb);

    ibus_init();
    bus = ibus_bus_new();
    g_object_ref_sink(bus);

    LOG_DEBUG("bus %p", bus);

    if (!ibus_bus_is_connected(bus))
    {
        LOG_WARN("not connected to ibus");
        exit(0);
    }

    LOG_DEBUG("ibus bus connected");

    g_signal_connect(bus, "disconnected", G_CALLBACK(ibus_disconnected_cb), NULL);

    IBusFactory *factory = ibus_factory_new(ibus_bus_get_connection(bus));
    LOG_DEBUG("factory %p", factory);
    g_object_ref_sink(factory);

    g_signal_connect(factory, "create-engine", G_CALLBACK(create_engine_cb), NULL);

    ibus_factory_add_engine(factory, "LotIme", IBUS_TYPE_ENGINE);

    IBusComponent *component;

    if (0)
    {
        if (!ibus_bus_request_name(bus, "org.freedesktop.IBus.LotIme", 0))
        {
            LOG_ERROR("error requesting bus name");
            exit(1);
        }
        else
        {
            LOG_INFO("ibus_bus_request_name success");
        }
    }
    else
    {
        component = ibus_component_new("org.freedesktop.IBus.LotIme",
                                       "LOT input method",
                                       "1.1",
                                       "MIT",
                                       "zhangfuwen",
                                       "xxx",
                                       "/usr/bin/ibus-lotime --ibus",
                                       "lotime");
        LOG_DEBUG("component %p", component);
        ibus_component_add_engine(component,
                                  ibus_engine_desc_new("LotIme",
                                                       "input method for Linux on TNT",
                                                       "input method for Linux on TNT",
                                                       "zh_CN",
                                                       "MIT",
                                                       "zhangfuwen",
                                                       "/home/zhangfuwen/bin/sdk-tools-linux-3859397/platforms/android-29/data/res/mipmap-mdpi/sym_def_app_icon.png",
                                                       "default"));
        ibus_bus_register_component(bus, component);

        ibus_bus_set_global_engine_async(bus, "LotIme", -1, nullptr, nullptr, nullptr);
    }

    LOG_INFO("entering ibus main");
    ibus_main();
    LOG_INFO("exiting ibus main");

    g_object_unref(factory);
    g_object_unref(bus);
}
