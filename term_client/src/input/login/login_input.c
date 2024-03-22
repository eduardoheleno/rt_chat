#include "login_input.h"

#include "../../ui/login/login_ui.h"
#include "../../utils/cursor/cursor.h"
#include "../../utils/network/network_manager.h"

char* username_listener(WINDOW *w) {
    int minx_pos = sizeof(TYPE_YOUR_USERNAME_TEXT) - 1;
    wmove(w, 0, minx_pos);
    wrefresh(w);

    return user_input_listener(w, minx_pos, 0, false);
}

char* password_listener(WINDOW *w) {
    int minx_pos = sizeof(TYPE_YOUR_PASSWORD_TEXT) - 1;
    wmove(w, 1, minx_pos);
    wrefresh(w);

    return user_input_listener(w, minx_pos, 1, true);
}

int login_listener(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    char *nickname_listener_v = username_listener(w);
    char *password_listener_v = password_listener(w);

    char *content = malloc(AUTH_USER_B_SIZE + strlen(nickname_listener_v) + strlen(password_listener_v));
    sprintf(content, AUTH_USER_B, nickname_listener_v, password_listener_v);

    return 0;
}
