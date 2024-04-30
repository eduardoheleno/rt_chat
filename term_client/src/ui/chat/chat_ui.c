#include "chat_ui.h"

#include "../../utils/network/network_manager.h"

extern int g_sockfd;

int draw_chat_ui(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    while (TRUE) {
	char frame_buffer[1024];
	recv(g_sockfd, frame_buffer, sizeof(frame_buffer), 0);

	ws_server_message server_message = extract_message(frame_buffer);

	clear();

	printw("%s:%s", server_message.username, server_message.message);

	refresh();

	free(server_message.username);
	free(server_message.message);
    }

    return 0;
}
