#include <threads.h>
#include <ncurses.h>

#include "./ui/login/login_ui.h"
#include "./input/login/login_input.h"
#include "./utils/thread/thread_helper.h"
#include "./utils/network/network_manager.h"

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef int (*functionp_t)(void *arg);

functionp_t ui_handler;
functionp_t input_handler;

cnd_t g_execution_code_cnd;
mtx_t g_execution_code_mtx;

int g_ui_thread_execution_code = 0;

int main() {
    char response[RESPONSE_BUFFER_SIZE];

    Request r;
    r.type = "POST";
    r.host = HOST;
    r.path = "/user";
    r.content_type = "application/json";

    char *request_content = "{\"nickName\": \"test5\", \"password\": \"salve\"}";
    r.content = request_content;

    char *request = build_request(&r);

    send_request(request, response);

    printf("%s", response);

 //    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
 //    server = gethostbyname(host);
	//
 //    memset(&serv_addr, 0, sizeof(serv_addr));
 //    serv_addr.sin_family = AF_INET;
 //    serv_addr.sin_port = htons(portno);
	//
 //    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
	//
 //    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
	// printf("merda");
 //    }
	//
 //    int total = strlen(request), sent = 0, bytes;
 //    do {
	// bytes = write(sockfd, request+sent, total-sent);
	// if (bytes < 0) perror("merda");
	// if (bytes == 0) break;
	//
	// sent += bytes;
 //    } while(sent < total);
	//
 //    char response[4096];
 //    memset(response, 0, sizeof(response));
 //    total = sizeof(response) - 1;
 //    int received = 0;
	//
 //    do {
	// bytes = read(sockfd, response+received, total-received);
	// if (bytes < 0) perror("merda");
	// if (bytes == 0) break;
	//
	// received += bytes;
 //    } while(received < total);
	//
 //    close(sockfd);
	//
 //    printf("Request %s\n", request);
 //    printf("Response %s\n", response);
    // int total = strlen(request);
    // int sent = 0;
    //

 //    WINDOW *w = initscr();
 //    noecho();
 //    cbreak();
 //    keypad(w, true);
	//
 //    ui_handler = draw_text_inputs;
 //    input_handler = login_listener;
	//
 //    thrd_t ui_thread_handler;
 //    thrd_t input_thread_handler;
	//
 //    cnd_init(&g_execution_code_cnd);
 //    mtx_init(&g_execution_code_mtx, mtx_plain);
	//
 //    thrd_create(&ui_thread_handler, ui_handler, w);
 //    thrd_create(&input_thread_handler, input_handler, w);
	//
 //    for(;;) {
	// cnd_wait(&g_execution_code_cnd, &g_execution_code_mtx);
	//
	// switch(g_ui_thread_execution_code) {
	//     case EXEC_HANDLERS_CODE:
	// 	wclear(w);
	// 	// TODO: guarantee that the ui_thread_handler will be ended before create a new one
	// 	thrd_create(&ui_thread_handler, ui_handler, w);
	// 	thrd_create(&input_thread_handler, input_handler, w);
	// 	break;
	// }
	//
	// if (g_ui_thread_execution_code == EXIT_CODE) {
	//     break;
	// }
	//
	// g_ui_thread_execution_code = 0;
	// mtx_unlock(&g_execution_code_mtx);
 //    }
	//
 //    endwin();

    return 0;
}
