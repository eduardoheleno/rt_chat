#ifndef CURSOR_H
#define CURSOR_H

#include <stdlib.h>
#include <ncurses.h>

#define KEY_ESC 27

typedef struct CursorData {
    char c;
    struct CursorData *n_node;
    struct CursorData *p_node;
} CursorData;

typedef struct {
    int x;
    CursorData *h_data;
} Cursor;

void add_data(CursorData **h_data, char c);
void remove_data(int index, CursorData **h_data);
void free_cursor_data(CursorData **h_data);
size_t data_size(CursorData *h_data);
char* concatenate_string(CursorData *h_data);
char* user_input_listener(WINDOW *w, int minx_pos, int y_pos);

#endif
