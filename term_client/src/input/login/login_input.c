#include <ncurses.h>
#include <threads.h>

#include "login_input.h"
#include "../../ui/login/login_ui.h"
#include "../../utils/cursor.h"

char* username_listener(WINDOW *w) {
    int minx_pos = sizeof(TYPE_YOUR_USERNAME_TEXT) - 1;
    wmove(w, 0, minx_pos);
    wrefresh(w);

    return user_input_listener(w, minx_pos, 0, false);
}

char* password_listener(WINDOW *w) {
    int minx_pos = sizeof(TYPE_YOUR_PASSWORD_TEXT) - 1;
    wmove(w, 1, minx_pos);
    wrefresh(w);

    return user_input_listener(w, minx_pos, 1, true);
}

int login_listener(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    char *username_listener_v = username_listener(w);
    char *password_listener_v = password_listener(w);

    return 0;
}
