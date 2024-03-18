#include <stdlib.h>
#include <string.h>

#include "cursor.h"

void add_data(CursorData **h_data, char c) {
    if (*h_data == NULL) {
	*h_data = malloc(sizeof(CursorData));

	(*h_data)->p_node = NULL;
	(*h_data)->p_node = NULL;
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

    while(index - 1 != counter) {
	counter++;
	h_data_buffer = h_data_buffer->n_node;
    }
    
    if (h_data_buffer->p_node == NULL && h_data_buffer->n_node == NULL) {
	free(*h_data);
	*h_data = NULL;

	return;
    }

    if (h_data_buffer->p_node == NULL) {
	h_data_buffer = h_data_buffer->n_node;
	free(h_data_buffer);

	return;
    }

    h_data_buffer->p_node->n_node = h_data_buffer->n_node;
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
