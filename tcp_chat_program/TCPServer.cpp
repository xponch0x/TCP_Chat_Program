#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>

/**
 * @author xponch0x
*/

class TCPServer{

private:
    int serverSocket;
    int port;
    static int messageCount;
    static std::mutex messageCountMutex;
    std::vector<std::thread> clientThreads;

    static void handleClient(int clientSocket, struct sockaddr_in clientAddr){
        char buffer[1024];

        while(true){
            memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

            if(bytes_received <= 0){
                break;
            }
            buffer[bytes_received] = '\0';
            std::string message(buffer);

            if(message == "exit" || message == "EXIT"){
                std::cout << "[DISCONNECTED]" << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
                break;
            }

            {
                std::lock_guard<std::mutex> lock(messageCountMutex);
                messageCount++;
                std::cout << "[MESSAGE RECEIVED] " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " - " << message << std::endl;

                std::string response = "TOTAL MESSAGES: " + std::to_string(messageCount) + "\n";
                send(clientSocket, response.c_str(), response.length(), 0);
            }
        }
        close(clientSocket);
    }

public:
    TCPServer(int portNum) : port(portNum), serverSocket(-1){}

    bool start(){
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(serverSocket < 0){
            std::cerr << "[ERROR] Socket Creation Failed" << std::endl;
            return false;
        }
    
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
            std::cerr << "[ERROR] Bind Failed" << std:: endl;
            return false;
        }

        if(listen(serverSocket, 10) < 0){
            std::cerr << "[ERROR] Listen Failed" << std::endl;
        }

        std::cout << "[TCP SERVER IS RUNNING]" << std::endl;
        return true;
    }

    void run(){
        while(true){
            struct sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);

            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
            if(clientSocket < 0){
                std::cerr << "[ERROR] Accept Failed" << std::endl;
                continue;
            }

            std::cout << "[NEW CONNECTION]" << inet_ntoa(clientAddr.sin_addr) << std::endl;

            clientThreads.emplace_back(handleClient, clientSocket, clientAddr);
            clientThreads.back().detach();
        }
    }

    ~TCPServer(){
        if(serverSocket != -1){
            close(serverSocket);
        }
    }
};

int TCPServer::messageCount = 0;
std::mutex TCPServer::messageCountMutex;

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <port> " << std::endl;
        return 1;
    }

    TCPServer server(std::stoi(argv[1]));

    if(!server.start()){
        return 1;
    }

    server.run();
    return 0;
}
