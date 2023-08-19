#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;
const int PORT = 1234;

int main() {
  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd == -1) {
    std::cerr << "Error creating socket." << std::endl;
    return 1;
  }
  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr))
      == -1) {
    std::cerr << "Error binding socket." << std::endl;
    return 1;
  }

  char buffer[BUFFER_SIZE];
  struct sockaddr_in client_addr = {};
  socklen_t client_addr_len = sizeof(client_addr);

  while (true) {
    ssize_t bytes_received = recvfrom(socket_fd,
                                      buffer,
                                      sizeof(buffer),
                                      0,
                                      (struct sockaddr*) &client_addr,
                                      &client_addr_len);
    if (bytes_received == -1) {
      std::cerr << "Error receiving data." << std::endl;
      continue;
    }
    std::string received_data(buffer, bytes_received);
    std::cout << "Received data: " << received_data << std::endl;
  }

  // Close the socket
  close(socket_fd);

  return 0;
}