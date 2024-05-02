#include "ui/chat_ui.h"
#include "utils/network_manager.h"

int cursor = 0;
ws_server_message *messages;
int messages_count = 0;

extern int g_sockfd;

int draw_chat_ui(WINDOW *w, int *cursor) {
    int max_visible_messages = 20;
    int limit;
    if (max_visible_messages + *cursor > messages_count) limit = messages_count; else limit = max_visible_messages + *cursor;

    wclear(w);

    int row_counter = 0;
    for (int i = *cursor; i < limit; ++i) {
	mvwprintw(w, row_counter, 0, "%s: %s", messages[i].username, messages[i].message);
	row_counter++;
    }

    wrefresh(w);

    return 0;
}

int chat_listen_network(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    while (TRUE) {
	char frame_buffer[1024];
	recv(g_sockfd, frame_buffer, sizeof(frame_buffer), 0);

	ws_server_message server_message = extract_message(frame_buffer);
	messages = realloc(messages, ++messages_count*sizeof(ws_server_message));
	messages[messages_count - 1] = server_message;

	draw_chat_ui(w, &cursor);
    }

    return 0;
}
