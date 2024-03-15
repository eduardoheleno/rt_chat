#include "login_ui.h"
#include "../../utils/thread_helper.h"
#include <threads.h>
#include <ncurses.h>

extern int ui_thread_execution_code;
extern cnd_t execution_code_cnd;
extern mtx_t execution_code_mtx;

void send_ui_signal() {
    mtx_lock(&execution_code_mtx);
    ui_thread_execution_code = REFRESH_FRAME_CODE;
    mtx_unlock(&execution_code_mtx);

    cnd_signal(&execution_code_cnd);
}

int draw_text_inputs(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    waddstr(w, TYPE_YOUR_USERNAME_TEXT);
    mvwaddstr(w, 1, 0, TYPE_YOUR_PASSWORD_TEXT);

    send_ui_signal();

    getch();

    return 0;
}


