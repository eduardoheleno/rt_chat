#include "login_input.h"

#include "../../ui/login/login_ui.h"
#include "../../utils/cursor/cursor.h"
#include "../../utils/thread/thread_helper.h"
#include "../../utils/network/network_manager.h"

extern int g_sockfd;

char* username_listener(WINDOW *w) {
    int minx_pos = sizeof(TYPE_YOUR_USERNAME_TEXT) - 1;
    wmove(w, 0, minx_pos);
    wrefresh(w);

    return user_input_listener(w, minx_pos, 0);
}

int login_listener(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    char *username_listener_v = username_listener(w);
    g_sockfd = connect_websocket(username_listener_v);

    send_ui_signal(SHOW_CHAT_SCREEN_CODE);

    return 0;
}
