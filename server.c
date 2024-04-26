#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 49153
#define MAX_BUFFER_SIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE];

    // Creating a TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setting socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding a socket to an address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Waiting for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Accepting an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
	{
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Receiving data (image) from the client and sending back
        int bytes_received;
        while ((bytes_received = recv(new_socket, buffer, MAX_BUFFER_SIZE, 0)) > 0)
	{
            send(new_socket, buffer, bytes_received, 0);
            printf("Sent packet with %d bytes to client. Received packet with %d bytes from client.\n", bytes_received, bytes_received);
        }

        if (bytes_received < 0)
	{
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        printf("Image received and echoed successfully.\n");

        // Closing client socket
        close(new_socket);
    }

    close(server_fd);

    return 0;
}
