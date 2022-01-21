# Directories
SRC := ./src
OBJ := ./obj
BIN := ./bin

# Files
SERVER_SRC_FILES := $(wildcard $(SRC)/server/*.c)
CLIENT_SRC_FILES := $(wildcard $(SRC)/client/*.c)
SERVER_OBJ_FILES := $(patsubst $(SRC)/server/%.c, $(OBJ)/server/%.o, $(SERVER_SRC_FILES))
CLIENT_OBJ_FILES := $(patsubst $(SRC)/client/%.c, $(OBJ)/client/%.o, $(CLIENT_SRC_FILES))
SERVER := $(BIN)/server
CLIENT := $(BIN)/client

# Making Executables
all: SERVER CLIENT


SERVER: $(SERVER_OBJ_FILES)
	mkdir -p $(BIN)
	gcc -o $(SERVER) $^

CLIENT: $(CLIENT_OBJ_FILES)
	mkdir -p $(BIN)
	gcc -o $(CLIENT) $^

# Making Object Files
$(OBJ)/server/%.o: $(SRC)/server/%.c
	mkdir -p $(OBJ)/server
	gcc -o $@ -c $<

$(OBJ)/client/%.o: $(SRC)/client/%.c
	mkdir -p $(OBJ)/client
	gcc -o $@ -c $<

# Cleaning Files
clean:
	rm -rf $(OBJ);
	rm -rf $(BIN);

server:
	$(SERVER)

client:
	$(CLIENT)
