#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include "funcoes.h"
#include <windows.h>
// Constantes
#define PORT 8080
#define MESSAGESIZE 1024


void senddata(void *arg) {
    SOCKET client_socket = *(SOCKET *)arg;
    free(arg);
    char mensagem[MESSAGESIZE + 1];
    int len = recv(client_socket, mensagem, MESSAGESIZE, 0);
        
    if(len > 0){
        // Resposta ao cliente 
        mensagem[len]='\0';
        char metodo[10];
        char caminho[255];
        char f_path[sizeof(LOCATION) + sizeof(caminho)];

        sscanf(mensagem, "%9s %254s", metodo, caminho);
        // printf("Metodo.: %s\n", metodo);
        // printf("Caminho: %s\n", caminho);
        printf("\nRequisicao (%zu bytes): \n\n%s\n", strlen(mensagem), mensagem);
      
        // strcat(f_path, obter_pasta(tipo));
        // strcat(f_path, caminho);

        if(strcmp(caminho, "/")==0) {
            snprintf(f_path, sizeof(f_path), "%s/index.html", LOCATION);
        }else{               
            snprintf(f_path, sizeof(f_path), "%s%s", LOCATION, caminho);
        }

        char *tipo = obter_tipo(f_path);    
        // printf("Tipo: %s\n", tipo);
        // printf("File Path: %s\n\n", f_path);

        if (strcmp(metodo, "GET")==0)
            carregar_pagina(client_socket, f_path, tipo);
        else{
            erro(client_socket, 501);
        }
    }

    closesocket(client_socket);
    _endthread();
}


int main(){
    WSADATA wsa;
    SOCKET s, new_socket;
    // Estrutura que armazena enderecos IPv4 e protocolos 
    struct sockaddr_in server , client;
    int c;

    // Funcao que inicializa a variavel wsa
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){
        printf("Falha no WSAStartup. Erro: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Inicializa o descritor do socket
    // IPv4 (AF_INET)
    // Protocolo TCP (SOCK_STREAM)
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        printf("Nao foi possivel criar o socket. Erro: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Preparacao da struct sockaddr_in
    server.sin_family = AF_INET;
    // O servidor ira aceitar conexoes vindas de qualquer placa de rede do computador (INADDR_ANY)
    server.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    // Inverte a PORT 8080 para ficar no formato padrao
    server.sin_port = htons(PORT);

    // Constroi o soket -> solicira uso da porta ao SO
    if(bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR){
        printf("Erro no bind (Porta %d possivelmente em uso). Erro: %d\n", PORT, WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    printf("\nAguardando Conexao\n");
    // Comeca a escura na porta
    listen(s, 3);
    // Caso chegue uma conexao, a funcao accept ira verificar se ira aceita-la ou nao 

    while (1) {
        c = sizeof(struct sockaddr_in);
        new_socket = accept(s, (struct sockaddr *)&client, &c);

        if (new_socket != INVALID_SOCKET) {
            printf("\nConexao aceita\n");
            printf("\nDados do cliente - IP: %s -  Porta: %d\n",inet_ntoa(client.sin_addr),htons(client.sin_port));
            // Aloca memória dinâmica para que o socket desta conexão seja único
            SOCKET *sock_ptr = malloc(sizeof(SOCKET));
            if (sock_ptr != NULL) {
                *sock_ptr = new_socket;
                // Cria uma nova thread para CADA cliente que conectar
                _beginthread(senddata, 0, sock_ptr);
            } else {
                closesocket(new_socket);
            }
        }
    }

    closesocket(new_socket);
    WSACleanup();
    exit(EXIT_FAILURE);

    return 1;
}