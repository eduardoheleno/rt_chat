#include "input/chat_input.h"
#include "utils/cursor.h"
#include "utils/network_manager.h"

extern int g_sockfd;

int listen_user_chat_input(void *arg) {
    WINDOW *w = (WINDOW *) arg;

    for(;;) {
	ws_frame frame;
	char *user_message = user_input_listener(w, 0, getmaxy(w) - 1);
	char *key = generate_masking_key();

	frame.fin = 1;
	frame.mask = 1;
	frame.opcode = TEXT;
	frame.masking_key = key;
	frame.payload_length = (strlen(user_message) * sizeof(char));
	frame.payload_data = xor_encrypt(user_message, key);

	size_t frame_size = sizeof_frame(frame.payload_length);
	char *built_frame = build_ws_frame(&frame, frame_size);

	int total_bytes = frame_size, sent = 0, bytes = 0;
	do {
	    bytes = send(g_sockfd, built_frame+sent, total_bytes-sent, 0);
	    if (bytes < 0) perror("Error");
	    if (bytes == 0) break;

	    sent += bytes;
	} while(sent < total_bytes);

	free(key);
	free(user_message);
	free(built_frame);
    }

    return 0;
}
