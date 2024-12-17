TCP Client/Server Program
========================

A simple TCP client/server program in C++. The server can handle multiple client connections and keeps track of messages received.

Compile and Run:
1. Build the programs:

   make all

2. Start the server:

   ./server <port>

   Example: ./server 443


3. Start a client:

   ./client <server_address> <port>
   
   Example: ./client 127.0.0.1 443


Basic Usage:
- Type messages in the client to send to server
- Server will respond with total message count
- Type 'EXIT' to disconnect client
- Use Ctrl+C to stop the server

Requirements:
- C++ compiler
- Make
- Linux/Unix system

@author 

xponch0x