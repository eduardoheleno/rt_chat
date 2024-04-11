#include "network_manager.h"

#include <stdbool.h>
#include <time.h>

char* build_request(Request *r) {
    char *message = malloc(sizeof(char) * MESSAGE_BUFFER_SIZE);
    size_t content_length = strlen(r->content);

    sprintf(
	message,
	HTTP_REQUEST_S,
	r->type,
	r->path,
	r->host,
	r->content_type,
	content_length,
	r->content
    );

    return message;
}

int send_request(char *message, char *response_buf) {
    // TODO: execute this in a different thread
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *server;
    struct sockaddr_in serv_addr;

    server = gethostbyname(HOST);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
	perror("The connection between the client and the HTTP server could not be established.");
    }

    int total_bytes = strlen(message), sent = 0, received = 0, bytes = 0;
    do {
	bytes = write(sockfd, message+sent, total_bytes-sent);
	if (bytes < 0) perror("Error at send bytes to the HTTP server.");
	if (bytes == 0) break;

	sent += bytes;
    } while(sent < total_bytes);

    memset(response_buf, 0, sizeof(char) * RESPONSE_BUFFER_SIZE);
    total_bytes = sizeof(char) * RESPONSE_BUFFER_SIZE;

    do {
	bytes = read(sockfd, response_buf+received, total_bytes-received);
	if (bytes < 0) perror("Error at receive bytes from the HTTP server.");
	if (bytes == 0) break;

	received += bytes;
    } while(received < total_bytes);

    close(sockfd);
    free(message);

    return 0;
}

int connect_websocket() {
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

    char *ws_handshake = "GET / HTTP/1.1\r\n"
	"user_id: 123\r\n"
	"Host: localhost:8081\r\n"
	"Upgrade: websocket\r\n"
	"Connection: Upgrade\r\n"
	"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
	"Sec-WebSocket-Version: 13\r\n"
	"\r\n";

    send(sockfd, ws_handshake, strlen(ws_handshake), 0);

    char buffer[1024];
    recv(sockfd, buffer, sizeof(buffer), 0);

    return sockfd;
}

char* extract_access_token(char *response) {
    // TODO: make this a generic JSON extractor
    size_t s_size = strlen(ACCESS_TOKEN_INDEX);
    char s_buffer[s_size], *s_cp = response;
    int s_index = 0;
    bool is_index_found = false;

    do {
	memcpy(s_buffer, s_cp++, s_size);
	if (strcmp(s_buffer, ACCESS_TOKEN_INDEX) == 0) {
	    is_index_found = true;
	    s_index += s_size;

	    break;
	}

	s_index++;
    } while(s_index + s_size < strlen(response));

    if (is_index_found) {
	int start_index, end_index;

	for(;;) {
	    if (response[s_index++] == '"') {
		start_index = s_index;
	    }

	    if (response[s_index] == '"') {
		end_index = s_index;
		break;
	    }
	}

	size_t token_size = end_index - start_index;
	char *access_token = malloc(token_size + 1);

	memcpy(access_token, &response[start_index], token_size);
	access_token[token_size] = '\0';

	return access_token;
    }

    perror("'access_token' wasn't found.'");
    return NULL;
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

char *xor_encrypt(char *payload, char masking_key[4]) {
    char *masked_payload = malloc(strlen(payload) * sizeof(char));
    size_t payload_bit_s = strlen(payload) * sizeof(char) * 8;

    int mk_counter = 0;
    for (int i = 0; i < strlen(payload); ++i) {
	if (mk_counter >= strlen(masking_key)) {
	    mk_counter = 0;
	}

	masked_payload[i] = payload[i] ^ masking_key[mk_counter];
	mk_counter++;
    }

    return masked_payload;
}

void set_bit(char *byte, int position, int value) {
    if (value) {
	*byte |= (1 << position);
    } else {
	*byte &= ~(1 << position);
    }
}

void byte_frame_to_big_endian(char *frame, size_t frame_size) {
    for (int j = 0; j < frame_size; ++j) {
	// 4 because its already swapping the leftover bytes
	for (int i = 0; i < 4; ++i) {
	    int bit = (frame[j] >> i) & 1;
	    int mirror_bit = (frame[j] >> (7 - i)) & 1;

	    set_bit(&frame[j], 7 - i, bit);
	    set_bit(&frame[j], i, mirror_bit);
	}
    }
}

char *build_ws_frame(ws_frame *frame) {
    size_t frame_size = sizeof_frame(frame->payload_length);
    char *built_frame = malloc(frame_size);
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

    for (int i = payload_data_index; i <= frame_size; ++i) {
	built_frame[i] |= frame->payload_data[i - payload_data_index];
    }

    return built_frame;
}

char *generate_masking_key() {
    // 32-bit
    char *key = malloc(4);

    srand(time(NULL));
    
    int min_number = 33;
    int max_number = 126;

    for (int i = 0; i < 4; ++i) {
	key[i] = rand() % (max_number + 1 - min_number) + min_number;
    }

    return key;
}
