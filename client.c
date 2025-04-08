#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    if (argc != 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }
    
    int file_fd = open(argv[1], O_RDONLY);
    if (file_fd < 0) {
        perror("Erro ao abrir arquivo");
        return 1;
    }
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro na criação do socket");
        close(file_fd);
        return 1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Endereço invalido");
        close(file_fd);
        close(sock);
        return 1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Conexão falhou");
        close(file_fd);
        close(sock);
        return 1;
    }
    
    printf("Conectou legal, enviando arquivo: %s\n", argv[1]);
    
    int bytes_read = read(file_fd, buffer, BUFFER_SIZE);
    if (bytes_read > 0) {
        send(sock, buffer, bytes_read, 0);
    }
    
    printf("Enviou legal\n");
    
    close(file_fd);
    close(sock);
    
    return 0;
}