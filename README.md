## Protocol
The client and server communicate by sending data on `stdout`.

- 1 The client begins by sending a line containing two integers `width` and `height`, separated by a single space.
This will be the width and height of the board.

- 2 The client then sends two integers `x` and `y` in the same fashion.
This will be the starting position of the snake.
**Note**: Passing `x` or `y` as `-1` will set them to a random value in the range `[0, width)` and `[0, height)` respectively.

- 3 The server will now send a `0` to indicate that everything is setup and it is waiting for an action.

We now enter the main loop.

- 4 The client sends one of either `0`, `1`, `2`, or `3`, denoting the direction to move in (`0` = right, `1` = up, `2` = left, `3` = down).

- 5 The server responds with the following data:
    - **Length**: an integer representing the length of the snake, which is also the number of segments (including the head).
    - **Segments**: a list of segments as pairs `x` and `y`, separated by a single space.
    - **Food**: the position of the food pip as a pair `x` and `y`, separated by a single space.

- 6 Repeat steps 4 and 5.
