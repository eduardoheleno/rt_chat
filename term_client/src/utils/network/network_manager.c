#include "network_manager.h"
#include <ncurses.h>

#include <stdbool.h>
#include <time.h>

int connect_websocket(char *username) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;

    server = gethostbyname("localhost");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);

    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
	perror("The connection between the client and the WebSocket server could not be established");
    }

    char *handshake_message = "GET / HTTP/1.1\r\n"
	"username: %s\r\n"
	"Host: localhost:8081\r\n"
	"Upgrade: websocket\r\n"
	"Connection: Upgrade\r\n"
	"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
	"Sec-WebSocket-Version: 13\r\n"
	"\r\n";

    size_t handshake_size = (strlen(handshake_message) + strlen(username) * sizeof(char));
    char ws_handshake[handshake_size];
    sprintf(ws_handshake, handshake_message, username);

    send(sockfd, ws_handshake, strlen(ws_handshake), 0);

    // TODO: check if the request was upgraded to websocket connection
    char buffer[1024];
    recv(sockfd, buffer, sizeof(buffer), 0);

    return sockfd;
}

size_t sizeof_frame(int payload_length) {
    // 6 is the base size (in bytes) of a frame with a payload_length less or equal to 125
    return 6 + payload_length;
}

unsigned int opcode_val(ws_opcode opcode) {
    switch (opcode) {
	case CONTINUATION:
	    return 0x00;
	case TEXT:
	    return 0x01;
	case BINARY:
	    return 0x02;
	case CLOSE:
	    return 0x08;
	case PING:
	    return 0x09;
	case PONG:
	    return 0x0A;
	default: perror("undefined opcode");
    }
}

char *xor_encrypt(char *payload, char masking_key[5]) {
    char *masked_payload = malloc((strlen(payload) * sizeof(char)) + 1);
    size_t payload_size = strlen(payload);

    int mk_counter = 0;
    for (int i = 0; i < payload_size; ++i) {
	if (mk_counter >= strlen(masking_key)) {
	    mk_counter = 0;
	}

	masked_payload[i] = payload[i] ^ masking_key[mk_counter];
	mk_counter++;
    }
    masked_payload[payload_size] = '\0';

    return masked_payload;
}

char *build_ws_frame(ws_frame *frame, size_t frame_size) {
    char *built_frame = malloc(frame_size + 1);
    unsigned int opcode = opcode_val(frame->opcode);

    memset(built_frame, 0, frame_size);

    int masking_key_index = 2;
    int payload_data_index = 6;

    built_frame[0] |= (frame->fin << 7);
    built_frame[0] |= (0 << 6);
    built_frame[0] |= (0 << 5);
    built_frame[0] |= (0 << 4);
    built_frame[0] |= (opcode << 0);

    built_frame[1] |= (frame->mask << 7);
    built_frame[1] |= (frame->payload_length << 0);

    for (int i = masking_key_index; i <= 5; ++i) {
	built_frame[i] |= frame->masking_key[i - masking_key_index];
    }

    for (int i = payload_data_index; i < frame_size; ++i) {
	built_frame[i] |= frame->payload_data[i - payload_data_index];
    }
    built_frame[frame_size] = '\0';

    return built_frame;
}

char *generate_masking_key() {
    // 4-bytes - 32-bits + 1 '\0'
    char *key = malloc(5);

    srand(time(NULL));
   
    // range of ASCII characters
    int min_number = 33;
    int max_number = 126;

    for (int i = 0; i < 4; ++i) {
	key[i] = rand() % (max_number + 1 - min_number) + min_number;
    }
    key[4] = '\0';

    return key;
}
