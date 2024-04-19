#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 49153
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_SIZE];
    FILE *image;

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

    // Open an image file to send
    image = fopen("/nfs/homes/ybazylbe/GIT/TCPSocket/echo/42.jpg", "rb");
    if (image == NULL)
    {
        perror("Failed to open image file");
        exit(EXIT_FAILURE);
    }

    // Sending the image to the server
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, image)) > 0)
    {
        send(sockfd, buffer, bytes_read, 0);
    }

    // Receiving echoed image from the server
    image = freopen("received_imageCl.jpg", "wb", image);
    if (image == NULL)
    {
        perror("Failed to create image file");
        exit(EXIT_FAILURE);
    }

    int bytes_received;
    while ((bytes_received = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, 1, bytes_received, image);
    }

    printf("Image received successfully.\n");

    // Closing the socket and file
    fclose(image);
    close(sockfd);

    return 0;
}
