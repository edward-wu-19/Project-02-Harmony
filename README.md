# Harmony

### Mohammad Khan and Edward Wu
#### Period 4

## Proposal
### Statement
We want to create a group chat service that uses the design of applications like Slack or Discord as an example. Some of the features we are aiming for include 
- Memory and retaining past messages in the current session
- Displaying messages and typing simultaneously
- Multiple channels
- Usernames and UIs
- Commands
- Colors

### Description
There will be two main programs. The server program will serve as a hub for client programs to connect to and will be run from a third party hosting service like Github Virtual Machine, or a similar service. This will allow the client programs to always connect to a host with a fixed IP address. A client program should be run locally and connect automatically to the server. Once it is connected, they can choose their username. Messages from the client will be sent to the server and distributed to the other clients of the same channel. The message history will be displayed via terminal or a windowing library like GTK (not the top priority). Clients can also enter special tags to run commands such as change the color of their name, move between channels, and exit the program entirely.

### Design
Topics Used:
- Allocating Memory
    - Memory storage and display
- Processes and Forking
    - Server will be a forking server where the parent file accepts connections and a single child will send data between clients
- Signals
    - Basic signal processing like Control+C and other terminating signals
- Pipes and Sockets
    - Pipes to establish connections between parent and child servers
    - Sockets to establish connections between client and server

### Breakdown
Mohammad will handle
- connection process between server and client
- forking process in the server

Edward will handle
- utilities like signal processing and commands
- memory management of messages

### Data Structures and Algorithms
- Linked list to store clients and descriptions
- Modified queue to store messages
- Arrays and String parsing to handle commands and messages

### Timeline
1. 1/13: Write a client that can talk to itself through message queues
2. 1/14: Write a server that can talk to a single client and send data between them
3. 1/17: Create a forking server that can talk to two clients and send information without blocking
4. 1/18: Implement utilies like commands, colors, signals, etc.
5. 1/21: Host the server on a virtual machine and test with about eight students to ensure it works and improve
6. 1/21: Implement multiple channels
7. 1/21: Use GTK to make windowing

Our priorities are to complete up to and including step 5 by the project deadline, and steps 6 and 7 would be extra features if we have time.

## Submission
### Getting Started
- Clone the repository
```
git clone git@github.com:edward-wu-19/Project-02-Harmony.git
```
- Change directories
```
cd Project-02-Harmony
```
- Compile using `make`
- Initiate server using `make server`
- Start client using `make client`

### Required Libraries
None.

### Features
- A custom harmony_message struct that records the message, user, channel sent, and time sent
- A message is sent by the server when a new client joins or a client leaves

### Next Steps
- Separate channels
- Colored text and names to identify server alerts and messages
- Make usernames unique
- Have IDs continue (Currently, if a client closes the connection, their ID is reused)

### Bugs
- Closing the server while a client is selecting a name will not close their select name screen

### Limitations
- The maximum username length is 32
- The maximum queue size is 128 messages
- The maximum buffer size is 2048 characters

### Commands
- `--exit` or `--quit` : Takes in either zero (defaults to 0) or one numeric argument and exits the shell with that number as its exit value
- `--help` : Prints a list of all possible commands
- `--rename` : Allows a client to rename itself, makes a server alert
