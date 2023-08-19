#include <arpa/inet.h>
#include <iostream>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "csv_parser.h"
#include "matching_engine.h"
#include "order_book.h"

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

    if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) ==
        -1) {
        std::cerr << "Error binding socket." << std::endl;
        return 1;
    }

    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr = {};
    socklen_t client_addr_len = sizeof(client_addr);
    kraken::CsvParser csv_parser;

    std::unordered_map<std::string, std::shared_ptr<kraken::OrderBook> > order_books;
    auto matching_engine_ptr = std::make_unique<kraken::MatchingEngine>(order_books);

    while (true) {
        ssize_t bytes_received =
                recvfrom(socket_fd, buffer, sizeof(buffer), 0,
                         (struct sockaddr *) &client_addr, &client_addr_len);
        if (bytes_received == -1) {
            std::cerr << "Error receiving data." << std::endl;
            continue;
        }
        std::string received_data(buffer, bytes_received);
        auto order =
                std::unique_ptr<kraken::Order>(csv_parser.Parse(received_data));
        auto execution_results = order->Execute(matching_engine_ptr);
        std::cout << execution_results;
    }

    // Close the socket
    close(socket_fd);

    return 0;
}