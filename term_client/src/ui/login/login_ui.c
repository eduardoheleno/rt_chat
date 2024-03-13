#include "login_ui.h"

void draw_text_inputs(WINDOW *w) {
    waddstr(w, TYPE_YOUR_USERNAME_TEXT);
    mvwaddstr(w, 1, 0, TYPE_YOUR_PASSWORD_TEXT);
}
