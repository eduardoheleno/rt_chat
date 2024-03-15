#include "login_input.h"
#include "../../ui/login/login_ui.h"

int username_listener(WINDOW *w) {
    wmove(w, 0, sizeof(TYPE_YOUR_USERNAME_TEXT) - 1);
    while (1) {}
}

int login_listener(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    int username_listener_val = username_listener(w);

    return 0;
}
