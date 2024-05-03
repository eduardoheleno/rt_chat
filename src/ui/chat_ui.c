#include "ui/chat_ui.h"
#include "utils/network_manager.h"
#include <ncurses.h>

#define MIN(a, b) (a < b) ? a : b

extern ws_server_message *messages;
extern int scroll_pos;
extern int messages_count;

// temporary
int max_visible_messages = 10;
// int max_visible_messages = getmaxy(w) - 1;

extern int g_sockfd;

void draw_chat_ui(WINDOW *w) {
    int limit = MIN(max_visible_messages + scroll_pos, messages_count);
    wclear(w);

    int row_counter = 0;
    for (int i = scroll_pos; i < limit; ++i) {
	mvwprintw(w, row_counter, 0, "%s: %s", messages[i].username, messages[i].message);
	row_counter++;
    }

    wrefresh(w);
}

int chat_listen_network(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    while (TRUE) {
	char frame_buffer[1024];
	recv(g_sockfd, frame_buffer, sizeof(frame_buffer), 0);

	ws_server_message server_message = extract_message(frame_buffer);
	messages = realloc(messages, ++messages_count*sizeof(ws_server_message));
	messages[messages_count - 1] = server_message;

	int limit = MIN(max_visible_messages + scroll_pos, messages_count);
	if (messages_count > limit) {
	    scroll_pos = messages_count - max_visible_messages;
	}

	draw_chat_ui(w);
    }

    return 0;
}
