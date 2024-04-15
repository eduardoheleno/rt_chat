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
