#include <stdio.h>
#include <threads.h>
#include <ncurses.h>

#include "./ui/login/login_ui.h"
#include "./input/login/login_input.h"
#include "./utils/thread_helper.h"

typedef int (*functionp_t)(void *arg);

functionp_t ui_handler;
functionp_t input_handler;

mtx_t ui_mtx;
mtx_t input_mtx;

int ui_thread_code_execution = 0;

// essa função observa o valor de "ui_thread_code_execution"
// a variável será alterada por uma outra thread que executará edições práticas na UI
// esse looping serve basicamente para checar se a thread que está fazendo alterações na UI
// precisa que a tela seja atualizada ou se a thread antiga da UI já foi finalizada e se
// deve iniciar uma nova
//
// possivelmente será necessário utilizar os signals ou mutexes para administrar essa variável
int ui_thread_handler(void *arg) {
    WINDOW *w = (WINDOW *) arg;
    thrd_t handler_thread;

    while(1) {
	switch(ui_thread_code_execution) {
	    case REFRESH_FRAME_CODE:
		wrefresh(w);
		break;
	    case EXEC_UI_HANDLER_CODE:
		thrd_create(&handler_thread, ui_handler, NULL);
		break;
	}
    }
    // mtx_lock(&ui_mtx);
    // ui_handler(w);
    //
    // mtx_unlock(&ui_mtx);
    return 0;
}

int main() {
    WINDOW *w = initscr();

    ui_handler = draw_text_inputs;
    // input_handler = login_listener;

    thrd_t ui_thread;

    thrd_create(&ui_thread, ui_thread_handler, w);
    // mtx_init(&mutex, mtx_plain);

    endwin();

    return 0;
}
