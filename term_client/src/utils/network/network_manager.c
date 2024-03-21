#include "network_manager.h"

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

    return 0;
}
