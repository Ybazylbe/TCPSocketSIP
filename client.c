#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 49153
#define MAX_BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_SIZE];
    FILE *received_image;

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

    // Open a file to save received image
    received_image = fopen("received_image.jpg", "wb");
    if (received_image == NULL)
    {
        perror("Failed to create image file");
        exit(EXIT_FAILURE);
    }

    // Open sending image
    FILE *image = fopen("./images/42.jpg", "rb");
    if (image == NULL)
    {
        perror("Failed to open image file");
        exit(EXIT_FAILURE);
    }

    int bytes_read;
    while ((bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, image)) > 0)
    {
        send(sockfd, buffer, bytes_read, 0);
        printf("Sent packet with %d bytes to server.\n", bytes_read);
        
        // Receive echoed data from server
        int bytes_received = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        printf("Received packet with %d bytes from server.\n", bytes_received);
        
        fwrite(buffer, 1, bytes_received, received_image);
    }

    printf("Image sent successfully.\n");

    // Closing the socket and files
    fclose(image);
    fclose(received_image);
    close(sockfd);

    return 0;
}
