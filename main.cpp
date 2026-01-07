#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class MiniRedis {
private:
    std::unordered_map<std::string, std::string> data;
    int server_fd;
    int port;
    //https://redis.io/docs/latest/develop/reference/protocol-spec/1
    std::vector<std::string> parseCommand(const std::string& input) {
        std::vector<std::string> tokens;
        std::istringstream stream(input);
        std::string token;
        
        while (stream >> token) {
            if (token[0] == '$') {
                // Bulk string length, skip it
                continue;
            }
            if (token[0] == '*') {
                //Size of array, skip it
                continue;
            }
            tokens.push_back(token);
        }

        std::cout << "tokens:";
        for (const auto& t : tokens) {
            std::cout << " [" << t << "]";
        }
        std::cout << std::endl;

        return tokens;
    }

    std::string handleCommand(const std::vector<std::string>& tokens) {
        if (tokens.empty()) {
            return "-ERR empty command\r\n";
        }

        std::string cmd = tokens[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

        if (cmd == "PING") {
            return "+PONG\r\n";
        }
        else if (cmd == "SET") {
            if (tokens.size() < 3) {
                return "-ERR wrong number of arguments for 'set' command\r\n";
            }
            data[tokens[1]] = tokens[2];
            return "+OK\r\n";
        }
        else if (cmd == "GET") {
            if (tokens.size() < 2) {
                return "-ERR wrong number of arguments for 'get' command\r\n";
            }
            auto it = data.find(tokens[1]);
            if (it != data.end()) {
                return "$" + std::to_string(it->second.length()) + "\r\n" + it->second + "\r\n";
            }
            return "$-1\r\n";
        }
        else if (cmd == "DEL") {
            if (tokens.size() < 2) {
                return "-ERR wrong number of arguments for 'del' command\r\n";
            }
            int count = 0;
            for (size_t i = 1; i < tokens.size(); i++) {
                if (data.erase(tokens[i])) {
                    count++;
                }
            }
            return ":" + std::to_string(count) + "\r\n";
        }
        else if (cmd == "EXISTS") {
            if (tokens.size() < 2) {
                return "-ERR wrong number of arguments for 'exists' command\r\n";
            }
            int count = 0;
            for (size_t i = 1; i < tokens.size(); i++) {
                if (data.find(tokens[i]) != data.end()) {
                    count++;
                }
            }
            return ":" + std::to_string(count) + "\r\n";
        }
        else if (cmd == "KEYS") {
            std::string result = "*" + std::to_string(data.size()) + "\r\n";
            for (const auto& pair : data) {
                result += "$" + std::to_string(pair.first.length()) + "\r\n" + pair.first + "\r\n";
            }
            return result;
        }
        else if (cmd == "QUIT") {
            return "+OK\r\n";
        }
        else {
            return "-ERR unknown command '" + tokens[0] + "'\r\n";
        }
    }

public:
    MiniRedis(int p = 6379) : port(p), server_fd(-1) {}

    ~MiniRedis() {
        if (server_fd >= 0) {
            close(server_fd);
        }
    }

    bool start() {
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        // Set socket options to reuse address
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Failed to set socket options" << std::endl;
            close(server_fd);
            return false;
        }

        // Bind socket
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Failed to bind socket on port " << port << std::endl;
            close(server_fd);
            return false;
        }

        // Listen for connections
        if (listen(server_fd, 10) < 0) {
            std::cerr << "Failed to listen on socket" << std::endl;
            close(server_fd);
            return false;
        }

        std::cout << "MiniRedis server started on port " << port << std::endl;
        return true;
    }

    void run() {
        while (true) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            
            int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            if (client_fd < 0) {
                std::cerr << "Failed to accept connection" << std::endl;
                continue;
            }

            std::cout << "Client connected from " 
                      << inet_ntoa(client_addr.sin_addr) << std::endl;

            handleClient(client_fd);
            close(client_fd);
        }
    }

private:
    void handleClient(int client_fd) {
        char buffer[4096];
        
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_read <= 0) {
                std::cout << "Client disconnected" << std::endl;
                break;
            }

            std::string input(buffer, bytes_read);
            
            // Remove trailing newlines
            while (!input.empty() && (input.back() == '\n' || input.back() == '\r')) {
                input.pop_back();
            }

            if (input.empty()) {
                continue;
            }

            std::vector<std::string> tokens = parseCommand(input);
            
            if (!tokens.empty() && tokens[0] == "QUIT") {
                std::string response = handleCommand(tokens);
                send(client_fd, response.c_str(), response.length(), 0);
                break;
            }

            std::string response = handleCommand(tokens);
            send(client_fd, response.c_str(), response.length(), 0);
        }
    }
};

int main(int argc, char* argv[]) {
    int port = 6379;
    
    if (argc > 1) {
        port = std::atoi(argv[1]);
    }

    MiniRedis server(port);
    
    if (!server.start()) {
        return 1;
    }

    server.run();
    
    return 0;
}
