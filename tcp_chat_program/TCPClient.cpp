#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>

/**
 * @author xponch0x
*/

class TCPClient{
private:
    int sock;
    std::string serverAddress;
    int port;

public:
    TCPClient(const std::string& address, int portNum) : serverAddress(address), port(portNum), sock(-1){}

    bool connect(){
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0){
            std::cerr << "[ERROR] Socket Creation Failed" << std::endl;
            return false;
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if(inet_pton(AF_INET, serverAddress.c_str(), &server_addr.sin_addr) <= 0){
            std::cout << "[ERROR] Invalid Address" << std::endl;
            return false;
        }

        if(::connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
            std::cerr << "[ERROR] Connection Failed" << std::endl;
            return false;
        }

        std::cout << "[CONNECTED TO TCP SERVER] @ " << serverAddress << ":" << port << std::endl;
        return true;
    }

    void run(){
        char buffer[1024];
        std::string message;

        while(true){
            std::cout << "ENTER MESSAGE [OR TYPE 'EXIT' TO DISCONNECT]: ";
            std::getline(std::cin, message);

            send(sock, message.c_str(), message.length(), 0);

            if(message == "exit" || message == "EXIT"){
                std::cout << "[DISCONNECTING FROM THE SERVER...]" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::cout << "[DISCONNECTED]" << std::endl;
                break;
            }

            memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if(bytes_received > 0) {
                buffer[bytes_received] = '\0';
                std::cout << "[FROM SERVER] " << buffer << std::endl;
            }
        }
    }

    ~TCPClient(){
        if(sock != -1){
            close(sock);
        }
    }
};

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <server_address> <port>" << std::endl;
        return 1;
    }

    TCPClient client(argv[1], std::stoi(argv[2]));

    if(!client.connect()){
        return 1;
    }

    client.run();
    return 0;
}