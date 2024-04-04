#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MESSAGE_BUFFER_SIZE 1024
#define RESPONSE_BUFFER_SIZE 4096

#define HTTP_REQUEST_S "%s %s HTTP/1.1\r\nHost: %s\r\nContent-Type: %s\r\nAccept: */*\r\nContent-Length: %zu\r\n\r\n%s"

#define AUTH_USER_B "{\"nickName\": \"%s\", \"password\": \"%s\"}"
#define AUTH_USER_B_SIZE 32

#define ACCESS_TOKEN_INDEX "\"access_token\":"

#define SERVER_PORT 3000
#define HOST "localhost"

typedef enum {
    CONTINUATION,
    TEXT,
    BINARY,
    CLOSE,
    PING,
    PONG
} ws_opcode;

typedef struct {
    unsigned int fin;
    ws_opcode opcode;
    unsigned int mask;
    unsigned int payload_length;
    char *masking_key;
    char *payload_data;
} ws_frame;

typedef struct {
    char *type;
    char *host;
    char *path;
    char *content_type;
    char *content;
} Request;

char *build_request(Request *r);
int send_request(char *message, char *response_buf);
int connect_websocket();
char *extract_access_token(char *response);

char *build_ws_frame(ws_frame *frame);
char *generate_masking_key();
char *xor_encrypt(char *payload, char masking_key[4]);

#endif
