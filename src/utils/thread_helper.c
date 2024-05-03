#include "utils/thread_helper.h"
#include <threads.h>

extern int g_ui_thread_execution_code;
extern cnd_t g_execution_code_cnd;
extern mtx_t g_execution_code_mtx;

void send_ui_signal(int code) {
    mtx_lock(&g_execution_code_mtx);
    g_ui_thread_execution_code = code;
    mtx_unlock(&g_execution_code_mtx);

    cnd_signal(&g_execution_code_cnd);
}
