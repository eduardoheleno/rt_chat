#include <threads.h>

#include "./ui/login/login_ui.h"
#include "./input/login/login_input.h"
#include "./utils/thread/thread_helper.h"
#include "utils/network/network_manager.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

typedef int (*functionp_t)(void *arg);

functionp_t ui_handler;
functionp_t input_handler;

cnd_t g_execution_code_cnd;
mtx_t g_execution_code_mtx;

int g_ui_thread_execution_code = 0;

int main() {
    char *key = generate_masking_key();
    printf("%s", key);

    char *payload = "payload";
    char masked_payload[strlen(payload)*sizeof(char)];
    
    size_t payload_s = strlen(payload) * (sizeof(char) * 8);

    char *masking_key = "1234";

    int mk_counter = 0;
    for (int i = 0; i < strlen(payload); ++i) {
	if (mk_counter >= strlen(masking_key)) {
	    mk_counter = 0;
	}

	masked_payload[i] = payload[i] ^ masking_key[mk_counter];
	mk_counter++;
    }

    for (int i = 0; i < strlen(payload)*sizeof(char); ++i) {
	printf("%c", masked_payload[i]);
    }

    ws_frame frame;
    frame.fin = 1;
    frame.opcode = TEXT;
    frame.mask = 1;
    frame.payload_length = payload_s;
    frame.masking_key = masking_key;

    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;

    server = gethostbyname("localhost");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0 ,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);

    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char *test = "GET / HTTP/1.1\r\n"
	"user_id: 123\r\n"
	"Host: localhost:8081\r\n"
	"Upgrade: websocket\r\n"
	"Connection: Upgrade\r\n"
	"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
	"Sec-WebSocket-Version: 13\r\n"
	"\r\n";

    send(sockfd, test, strlen(test), 0);

    char buffer[1024];
    recv(sockfd, buffer, sizeof(buffer), 0);

    printf("%s", buffer);
    for(;;){}

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
