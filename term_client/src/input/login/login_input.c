#include <ncurses.h>
#include <threads.h>

#include "login_input.h"
#include "../../ui/login/login_ui.h"
#include "../../utils/cursor.h"

char* username_listener(WINDOW *w) {
    Cursor c; c.x = 0; c.h_data = NULL;

    int minx_pos = sizeof(TYPE_YOUR_USERNAME_TEXT) - 1;
    int ui_pos_tracker = minx_pos;

    wmove(w, 0, minx_pos);

    for(;;) {
	int pressed_char = wgetch(w);

	if (pressed_char == '\n') {
	    char *c_string = concatenate_string(c.h_data);
	    mvwprintw(w, 10, 0, "%s", c_string);
	    wrefresh(w);
	    // return concatenate_string(c.h_data);
	}

	if (pressed_char == KEY_BACKSPACE) {
	    if (data_size(c.h_data) > 0) {
		mvwdelch(w, 0, --ui_pos_tracker);
		remove_data(c.x, &c.h_data);

		c.x--;
	    }

	    mvwprintw(w, 11, 0, "%i", data_size(c.h_data));
	    wrefresh(w);

	    continue;
	}

	if (pressed_char == KEY_LEFT) {
	    if (c.x > 0) {
		c.x--;

		wmove(w, 0, --ui_pos_tracker);
		wrefresh(w);
	    }

	    continue;
	}

	if (pressed_char == KEY_RIGHT) {
	    if (c.x < data_size(c.h_data)) {
		c.x++;

		wmove(w, 0, ++ui_pos_tracker);
		wrefresh(w);
	    }

	    continue;
	}

	mvwaddch(w, 0, ui_pos_tracker++, pressed_char);
	c.x++;

	add_data(&c.h_data, pressed_char);

	wrefresh(w);
    }
}

int login_listener(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    char* username_listener_v = username_listener(w);

    return 0;
}
