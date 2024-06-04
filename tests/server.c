#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Create a simple JSON response string
char *get_json_response() {
  return "{\"message\": \"Hello from C Server\", \"status\": \"success\"}";
}

int main() {
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};
  char *response;

  // Create a socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket creation failed");
    return -1;
  }

  // Set socket options for port reuse
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("Setsockopt failed");
    return -1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind the address and port to the socket
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    return -1;
  }

  // Start listening, with a maximum of 5 pending connections
  if (listen(server_fd, 5) < 0) {
    perror("Listen failed");
    return -1;
  }

  fprintf(stderr, "Listening at %d\n", PORT);

  while (1) {
    // Accept a new connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      perror("Accept failed");
      return -1;
    }

    // Read the client request; simplify for demonstration, parse HTTP requests
    // in real applications
    valread = read(new_socket, buffer, BUFFER_SIZE);
    printf("Request: %s\n", buffer);

    // Construct the HTTP response header and JSON body
    response = malloc(
        strlen("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n") +
        strlen(get_json_response()) + 1);
    strcpy(response,
           "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
    strcat(response, get_json_response());

    // Send the response to the client
    send(new_socket, response, strlen(response), 0);
    printf("Response sent\n");

    free(response);    // Free dynamically allocated memory
    close(new_socket); // Close the connection
  }

  return 0;
}