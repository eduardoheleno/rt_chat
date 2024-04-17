#include "chat_input.h"

#include "../../utils/cursor/cursor.h"
#include "../../utils/network/network_manager.h"

extern int g_sockfd;

int listen_user_chat_input(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    // TODO: frame communication shows 2 different errors:
    // - UTF8Error
    // - Illegal fragmented control frame
    for(;;) {
	ws_frame frame;

	frame.fin = 1;
	frame.mask = 1;
	frame.opcode = TEXT;

	char *user_message = user_input_listener(w, 0, getmaxy(w) - 1);
	char *key = generate_masking_key();

	frame.masking_key = key;
	frame.payload_length = (strlen(user_message) * sizeof(char));
	frame.payload_data = xor_encrypt(user_message, key);

	char *built_frame = build_ws_frame(&frame);

	int total_bytes = strlen(built_frame), sent = 0, bytes = 0;
	do {
	    bytes = send(g_sockfd, built_frame+sent, total_bytes-sent, 0);
	    if (bytes < 0) perror("Error");
	    if (bytes == 0) break;

	    sent += bytes;
	} while(sent < total_bytes);

	free(key);
	free(user_message);
    }

    return 0;
}
