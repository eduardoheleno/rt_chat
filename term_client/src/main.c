#include <threads.h>

#include "./ui/login/login_ui.h"
#include "./input/login/login_input.h"
#include "./utils/thread/thread_helper.h"

typedef int (*functionp_t)(void *arg);

functionp_t ui_handler;
functionp_t input_handler;

cnd_t g_execution_code_cnd;
mtx_t g_execution_code_mtx;

int g_ui_thread_execution_code = 0;

int main() {
    // char response[RESPONSE_BUFFER_SIZE];
    //
    // Request r;
    // r.type = "POST";
    // r.host = HOST;
    // r.path = "/user";
    // r.content_type = "application/json";
    //
    // char *request_content = "{\"nickName\": \"test5\", \"password\": \"salve\"}";
    // r.content = request_content;
    //
    // char *request = build_request(&r);
    //
    // send_request(request, response);
    //
    // printf("%s", response);

    WINDOW *w = initscr();
    noecho();
    cbreak();
    keypad(w, true);

    ui_handler = draw_text_inputs;
    input_handler = login_listener;

    thrd_t ui_thread_handler;
    thrd_t input_thread_handler;

    cnd_init(&g_execution_code_cnd);
    mtx_init(&g_execution_code_mtx, mtx_plain);

    thrd_create(&ui_thread_handler, ui_handler, w);
    thrd_create(&input_thread_handler, input_handler, w);

    for(;;) {
	cnd_wait(&g_execution_code_cnd, &g_execution_code_mtx);

	switch(g_ui_thread_execution_code) {
	    case EXEC_HANDLERS_CODE:
		wclear(w);
		// TODO: guarantee that the ui_thread_handler will be ended before create a new one
		thrd_create(&ui_thread_handler, ui_handler, w);
		thrd_create(&input_thread_handler, input_handler, w);
		break;
	}

	if (g_ui_thread_execution_code == EXIT_CODE) {
	    break;
	}

	g_ui_thread_execution_code = 0;
	mtx_unlock(&g_execution_code_mtx);
    }

    endwin();

    return 0;
}
