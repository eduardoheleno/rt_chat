#include "login_ui.h"
#include <ncurses.h>

int draw_text_inputs(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    waddstr(w, TYPE_YOUR_USERNAME_TEXT);
    mvwaddstr(w, 1, 0, TYPE_YOUR_PASSWORD_TEXT);

    return 0;
}
