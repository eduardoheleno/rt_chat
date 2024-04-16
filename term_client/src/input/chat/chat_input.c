#include "chat_input.h"

#include "../../utils/cursor/cursor.h"
#include "../../utils/network/network_manager.h"

extern int g_sockfd;

int listen_user_chat_input(void *arg) {
    WINDOW *w = (WINDOW *) arg;
    ws_frame frame;

    frame.fin = 1;
    frame.mask = 1;
    frame.opcode = TEXT;

    for(;;) {
	char *user_message = user_input_listener(w, 0, getmaxy(w) - 1);
	char *key = generate_masking_key();

	frame.masking_key = key;
	frame.payload_length = (strlen(user_message) * sizeof(char));
	frame.payload_data = xor_encrypt(user_message, key);

	char *built_frame = build_ws_frame(&frame);

	send(g_sockfd, built_frame, strlen(built_frame), 0);

	free(key);
	free(user_message);
    }

    return 0;
}
