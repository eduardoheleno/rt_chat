#include "login_input.h"
#include "../../ui/login/login_ui.h"

void login_listener(WINDOW *w) {
    int username_listener_val = username_listener(w);
}

int username_listener(WINDOW *w) {
    wmove(w, 0, sizeof(TYPE_YOUR_USERNAME_TEXT) - 1);
    wrefresh(w);
    while (1) {}
}
