#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Creating a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert server IP address to binary format and set server address
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Establishing a connection to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Opening an image to send
    FILE *image = fopen("./image location/image.jpg", "rb");
    if (image == NULL)
    {
        perror("Failed to open image file");
        exit(EXIT_FAILURE);
    }

    // Reading an image and sending it to the server
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, image)) > 0)
    {
        send(sockfd, buffer, bytes_read, 0);
    }
    fclose(image);
    printf("Image sent successfully.\n");

    // Closing a socket
    close(sockfd);

    return 0;
}

