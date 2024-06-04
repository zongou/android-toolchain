#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define INDEX_HTML                                                             \
  "<html><body><h1>Welcome to My Server</h1><nav><a href='page1.html'>Page "   \
  "1</a> | <a href='page2.html'>Page 2</a></nav></body></html>"

// 新增函数，用于构建响应头
void build_response_header(char *header, size_t header_size,
                           long content_length) {
  snprintf(header, header_size,
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: %ld\r\n\r\n",
           content_length);
}

void send_html(int client_socket) {
  char header[BUFFER_SIZE] = {0};
  long content_length = sizeof(INDEX_HTML) - 1;

  build_response_header(header, sizeof(header), content_length);

  // 优化发送方式，确保一次发送整个响应
  send(client_socket, header, strlen(header), 0);
  send(client_socket, INDEX_HTML, content_length, 0);
}

int main() {
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket creation failed");
    return -1;
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("Setsockopt failed");
    return -1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    return -1;
  }

  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    return -1;
  }

  printf("Server is listening on port %d...\n", PORT);

  while (1) {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      perror("Accept failed");
      // 考虑到accept失败可能是暂时的网络问题，这里选择继续循环
      continue;
    }

    printf("Accepted a new connection from %s:%d\n",
           inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    send_html(new_socket);
    close(new_socket);
  }

  return 0;
}