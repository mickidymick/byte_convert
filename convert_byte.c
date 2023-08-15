#include <yed/plugin.h>
#include <inttypes.h>
#include <yed/gui.h>

#define DO_LOG
#define DBG__XSTR(x) #x
#define DBG_XSTR(x) DBG__XSTR(x)
#ifdef DO_LOG
#define DBG(...)                                           \
do {                                                       \
    LOG_FN_ENTER();                                        \
    yed_log(__FILE__ ":" XSTR(__LINE__) ": " __VA_ARGS__); \
    LOG_EXIT();                                            \
} while (0)
#else
#define DBG(...) ;
#endif

static void        unload(yed_plugin *self);

static yed_event_handler h_key;
static yed_event_handler h_mouse;

/* Event Handlers */
static void _gui_key_handler(yed_event *event);
static void _gui_mouse_handler(yed_event *event);


int yed_plugin_boot(yed_plugin *self) {
    yed_plugin_request_mouse_reporting(self);

    YED_PLUG_VERSION_CHECK();
    Self = self;

    h_key.kind = EVENT_KEY_PRESSED;
    h_key.fn   = _gui_key_handler;
    yed_plugin_add_event_handler(self, h_key);

    h_mouse.kind = EVENT_KEY_PRESSED;
    h_mouse.fn   = _gui_mouse_handler;
    yed_plugin_add_event_handler(self, h_mouse);

    yed_plugin_set_command(self, "convert-byte", convert_byte);

    list_items = array_make(char *);
    yed_gui_init_list_menu(&list_menu, list_items);
    list_menu.base.is_up = 0;
    converted_items = array_make(char *);

    yed_plugin_set_unload_fn(self, unload);


    return 0;
}

static void _gui_key_handler(yed_event *event) {
    int ret = 0;
    ret = yed_gui_key_pressed(event, &list_menu);
    if (ret) {
        run();
    }

    if (!list_menu.base.is_up) {
        yed_delete_event_handler(h_mouse);
    }
}

static void _gui_mouse_handler(yed_event *event) {
    yed_gui_mouse_pressed(event, &list_menu);

    if (!list_menu.base.is_up) {
        yed_delete_event_handler(h_mouse);
    }
}
