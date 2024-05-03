#ifndef CHAT_UI_H
#define CHAT_UI_H

#include <ncurses.h>

void draw_chat_ui(WINDOW *w);
int chat_listen_network(void *arg);

#endif
