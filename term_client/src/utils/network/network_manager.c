#include "network_manager.h"

#include <stdbool.h>

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
}
