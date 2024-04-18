## Build and Run
1. Clone the repo using `git clone https://github.com/Atjowt/snake.git`.
2. Build the project by running `make` inside the project directory. You might need to create the directories `snake-server/bin` and `snake-client/bin` first.
3. Run `sh start.sh [PORT]` to start the client with the server running in the background on port [PORT] **OR** run `snake-server/bin/snake-server [PORT]` and `snake-client/bin/snake-client` manually as separate instances.

## Protocol
The client and server communicate by sending data over TCP/IP.

1. The client begins by sending two integers `width` and `height`. This will be the width and height of the board.

2. The client then sends two integers `x` and `y`. This will be the starting position of the snake.
**Note**: Passing `x` or `y` as `-1` will set them to a random value in the range `[0, width)` and `[0, height)` respectively.

The main loop consists of the following:

3. The server sends the following data:
    - **length**: an integer representing the length of the snake (the number of segments, including the head). __If this is `0`, the snake is dead.__
    - **segments**: a list of segments `x`, `y`. The first segment is the head.
    - **food**: the position of the food pip `x`, `y`.

4. The client sends one of either `0`, `1`, `2`, or `3`, denoting the direction to move in (`0` = right, `1` = up, `2` = left, `3` = down).

