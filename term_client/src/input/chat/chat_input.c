#include "chat_input.h"

#include "../../utils/cursor/cursor.h"

int listen_user_chat_input(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    for(;;) {
	char *user_text = user_input_listener(w, 0, 0);
	printw("%s", user_text);

	wclear(w);

	free(user_text);
    }

    return 0;
}
