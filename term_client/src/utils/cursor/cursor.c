#include "cursor.h"

#include <stdlib.h>
#include <string.h>

void add_data(CursorData **h_data, char c) {
    if (*h_data == NULL) {
	*h_data = malloc(sizeof(CursorData));

	(*h_data)->p_node = NULL;
	(*h_data)->n_node = NULL;
	(*h_data)->c = c;

	return;
    }

    CursorData *h_data_buffer = *h_data;
    while(h_data_buffer->n_node != NULL) {
	h_data_buffer = h_data_buffer->n_node;
    }

    CursorData *new_data = malloc(sizeof(CursorData));

    new_data->p_node = h_data_buffer;
    new_data->n_node = NULL;
    new_data->c = c;

    h_data_buffer->n_node = new_data;
}

void remove_data(int index, CursorData **h_data) {
    CursorData *h_data_buffer = *h_data;
    int counter = 0;

    while(counter <= index - 1) {
	counter++;
	h_data_buffer = h_data_buffer->n_node;
    }

    if (h_data_buffer->p_node == NULL && h_data_buffer->n_node == NULL) {
	*h_data = NULL;

	return;
    }

    if (h_data_buffer->n_node == NULL) {
	h_data_buffer->p_node->n_node = NULL;
	free(h_data_buffer);

	return;
    }

    if (h_data_buffer->p_node == NULL) {
	(*h_data)->n_node->p_node = NULL;
	(*h_data) = (*h_data)->n_node;

	return;
    }

    h_data_buffer->p_node->n_node = h_data_buffer->n_node;
    h_data_buffer->n_node->p_node = h_data_buffer->p_node;

    free(h_data_buffer);
}

size_t data_size(CursorData *h_data) {
    CursorData *h_data_buffer = h_data;
    size_t d_size = 0;

    while(h_data_buffer != NULL) {
	d_size++;
	h_data_buffer = h_data_buffer->n_node;
    }

    return d_size;
}

char* concatenate_string(CursorData *h_data) {
    size_t s_size = data_size(h_data);
    char *c_string = malloc(sizeof(s_size) + 1);
    *c_string = '\0';

    CursorData *h_data_buffer = h_data;
    while(h_data_buffer != NULL) {
	strcat(c_string, &h_data_buffer->c);
	h_data_buffer = h_data_buffer->n_node;
    }

    return c_string;
}

char* user_input_listener(WINDOW *w, int minx_pos, int y_pos, bool is_password) {
    Cursor c; c.x = 0; c.h_data = NULL;
    int ui_pos_tracker = minx_pos;

    for(;;) {
	int pressed_char = wgetch(w);

	if (pressed_char == '\n') {
	    return concatenate_string(c.h_data);
	}

	if (pressed_char == KEY_BACKSPACE) {
	    if (data_size(c.h_data) > 0 && c.x > 0) {
		mvwdelch(w, y_pos, --ui_pos_tracker);
		remove_data(--c.x, &c.h_data);
	    }

	    continue;
	}

	if (pressed_char == KEY_LEFT) {
	    if (c.x > 0) {
		c.x--;

		wmove(w, y_pos, --ui_pos_tracker);
		wrefresh(w);
	    }

	    continue;
	}

	if (pressed_char == KEY_RIGHT) {
	    if (c.x < data_size(c.h_data)) {
		c.x++;

		wmove(w, y_pos, ++ui_pos_tracker);
		wrefresh(w);
	    }

	    continue;
	}

	if (is_password) {
	    mvwaddch(w, y_pos, ui_pos_tracker++, '*');
	} else {
	    mvwaddch(w, y_pos, ui_pos_tracker++, pressed_char);
	}
	c.x++;

	add_data(&c.h_data, pressed_char);

	wrefresh(w);
    }
}
