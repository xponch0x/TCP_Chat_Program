CXX = clang++
CXXFLAGS = -std=c++17 -Wall -pthread
TARGETS = server client

all: $(TARGETS)

server: TCPServer.cpp
	$(CXX) $(CXXFLAGS) -o server TCPServer.cpp

client: TCPClient.cpp
	$(CXX) $(CXXFLAGS) -o client TCPClient.cpp

clean:
	rm -f $(TARGETS)