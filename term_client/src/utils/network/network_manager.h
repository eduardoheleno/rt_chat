#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    char *username;
    char *message;
} ws_server_message;

typedef struct {
    char *type;
    char *host;
    char *path;
    char *content_type;
    char *content;
} Request;

char *build_request(Request *r);
int send_request(char *message, char *response_buf);
int connect_websocket(char *username);
char *extract_access_token(char *response);
ws_server_message extract_message(char *frame_buffer);
char *build_ws_frame(ws_frame *frame, size_t frame_size);
size_t sizeof_frame(int payload_length);
char *generate_masking_key();
char *xor_encrypt(char *payload, char masking_key[5]);

#endif
