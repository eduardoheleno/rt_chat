#include <stdio.h>
#include <ncurses.h>

#include "./ui/login/login_ui.h"

typedef void (*functionp_t)(WINDOW *w);

struct Handlers {
    functionp_t ui_handler;
};

int main() {
    WINDOW *w = initscr();

    struct Handlers handlers;
    handlers.ui_handler = draw_text_inputs;

    // while (TRUE) {
    //
    // }

    handlers.ui_handler(w);
    wrefresh(w);

    getch();

    endwin();

    return 0;
}
