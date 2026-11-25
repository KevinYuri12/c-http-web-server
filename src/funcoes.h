#ifndef FUNCOES_H
#define FUNCOES_H
// #define LOCATION "www"
#define LOCATION "www"
#include <winsock2.h>

char *obter_tipo(char *caminho);
void get_erro_menssage(int num, char msg[40]);
void erro(SOCKET client_socket, int num);
void carregar_pagina(SOCKET client_socket, char *arquivo, char *tipo);

#endif