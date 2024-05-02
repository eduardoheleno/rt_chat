#include "ui/login_ui.h"
#include "ui/chat_ui.h"
#include "input/chat_input.h"
#include "input/login_input.h"
#include "utils/thread_helper.h"

#include <threads.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef int (*functionp_t)(void *arg);

cnd_t g_execution_code_cnd;
mtx_t g_execution_code_mtx;

int g_ui_thread_execution_code = 0;
int g_sockfd;

int main() {
    WINDOW *w = initscr();
    noecho();
    cbreak();
    keypad(w, true);

    thrd_t ui_thread_handler;
    thrd_t input_thread_handler;

    cnd_init(&g_execution_code_cnd);
    mtx_init(&g_execution_code_mtx, mtx_plain);

    thrd_create(&ui_thread_handler, draw_text_inputs, w);
    thrd_create(&input_thread_handler, login_listener, w);

    for(;;) {
	cnd_wait(&g_execution_code_cnd, &g_execution_code_mtx);

	functionp_t ui_handler;
	functionp_t input_handler;

	switch (g_ui_thread_execution_code) {
	    case SHOW_CHAT_SCREEN_CODE:
		ui_handler = chat_listen_network;
		input_handler = listen_user_chat_input;
		break;
	}

	wclear(w);
	thrd_create(&ui_thread_handler, ui_handler, w);
	thrd_create(&input_thread_handler, input_handler, w);

	if (g_ui_thread_execution_code == EXIT_CODE) {
	    break;
	}

	g_ui_thread_execution_code = 0;
	mtx_unlock(&g_execution_code_mtx);
    }

    endwin();

    return 0;
}
