#include <stdio.h>
#include <threads.h>
#include <ncurses.h>

#include "./ui/login/login_ui.h"
#include "./input/login/login_input.h"
#include "./utils/thread_helper.h"

typedef int (*functionp_t)(void *arg);

functionp_t ui_handler;
functionp_t input_handler;

cnd_t execution_code_cnd;
mtx_t execution_code_mtx;

int ui_thread_execution_code = 0;

int main() {
    WINDOW *w = initscr();

    ui_handler = draw_text_inputs;
    input_handler = login_listener;

    thrd_t ui_thread_handler;
    thrd_t input_thread_handler;

    cnd_init(&execution_code_cnd);
    mtx_init(&execution_code_mtx, mtx_plain);

    thrd_create(&ui_thread_handler, ui_handler, w);
    thrd_create(&input_thread_handler, input_handler, w);

    for(;;) {
	cnd_wait(&execution_code_cnd, &execution_code_mtx);

	switch(ui_thread_execution_code) {
	    case REFRESH_FRAME_CODE:
		wrefresh(w);
		break;
	    case EXEC_HANDLERS_CODE:
		// TODO: guarantee that the ui_thread_handler will be ended before create a new one
		thrd_create(&ui_thread_handler, ui_handler, w);
		thrd_create(&input_thread_handler, input_handler, w);
		break;
	}

	if (ui_thread_execution_code == EXIT_CODE) {
	    break;
	}

	ui_thread_execution_code = 0;
	mtx_unlock(&execution_code_mtx);
    }

    endwin();

    return 0;
}
