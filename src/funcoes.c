#include <stdio.h>
#include <string.h>
#include "funcoes.h"
#include <winsock2.h>
#include <windows.h>

char *obter_tipo(char *caminho){

    // Tratar o caso em que o caminho não contém um ponto (.) indicando a ausência de extensão
    // Tratar arquivos com pontos no nome e não na extensão, como "arquivo.min.js
    char *p = strrchr(caminho, '.');
    
    if (p == NULL) {
        return "application/octet-stream"; 
    }
    // printf("Funcao obter_tipo\n");
    // printf("Caminho: %s\n", caminho);
    // printf("P: %s\n", p);

    if (p == NULL || strcmp(p, ".html") == 0 || strcmp(p, ".htm") == 0) 
        return "text/html";
    if (strcmp(p, ".css") == 0)     
        return "text/css";
    if (strcmp(p, ".js") == 0)      
        return "application/javascript";
    if (strcmp(p, ".png") == 0)     
        return "image/png";
    if (strcmp(p, ".jpg") == 0 || strcmp(p, ".jpeg") == 0) 
        return "image/jpeg";
    if (strcmp(p, ".gif") == 0)  
        return "image/gif";
    if (strcmp(p, ".webp") == 0)    
        return "image/webp";

    return "application/octet-stream";
}

void get_erro_menssage(int num, char msg[40]){
    switch (num){
            case 404: strcpy(msg, "Not Found"); 
        break;
            case 501: strcpy(msg, "Not Implemented"); 
        break;
            default: strcpy(msg, "Erro Desconhecido");
        break;
    }
}

void erro(SOCKET client_socket, int num){
    char f_path[255];
    char msg[40];
    
    get_erro_menssage(num, msg);
    printf("Carregando Pagina Erro...\n"); 
    
    snprintf(f_path, sizeof(f_path), "%s/erro%d.html", LOCATION, num);
    // printf("F_PATH:%s\n", f_path);
    FILE *file = fopen(f_path, "rb");   
    char cabecalho[256];
    
    snprintf(cabecalho, sizeof(cabecalho),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n\r\n", num, msg);
                        
    char buffer_leitura[1024];
    send(client_socket, cabecalho, strlen(cabecalho), 0);
    if(file == NULL){
        printf("Arquivo de erro nao encontrado: %s\n", f_path);
        strcpy(buffer_leitura, "<h1 style=\"text-align: center;\">Erro 404</h1>\0");
        send(client_socket, buffer_leitura, strlen(buffer_leitura), 0);
    }else {           
        int bytes_lidos;

        while ((bytes_lidos = fread(buffer_leitura, 1, sizeof(buffer_leitura), file)) > 0){
            send(client_socket, buffer_leitura, bytes_lidos, 0);
        }
        fclose(file);
    }
}

void carregar_pagina(SOCKET client_socket, char *arquivo, char *tipo){
    
    //printf("Tipo..:%s\n", tipo); 
    FILE *file = fopen(arquivo, "rb");   

    if (file == NULL) {
        erro(client_socket, 404); 
        return;
    } else {
        printf("Carregando Arquivo: %s\n", tipo); 
        // Sleep(1000);
        char cabecalho[256];
        snprintf(cabecalho, sizeof(cabecalho),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Connection: Keep-Alive\r\n\r\n", tipo);

        send(client_socket, cabecalho, strlen(cabecalho), 0);

        char buffer_leitura[1024];
        int bytes_lidos;

        while ((bytes_lidos = fread(buffer_leitura, 1, sizeof(buffer_leitura), file)) > 0){
            send(client_socket, buffer_leitura, bytes_lidos, 0);
        }

        fclose(file);
    }

}