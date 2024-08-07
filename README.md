# Snake!
This is a simple header-only snake engine. Simply include [snake.h](include/snake.h) in your project to use it, and remember to define `SNAKE_C` before including in it in your source file. See example for tips on usage.

## Example

There is some example [source code](example) available inside the `example` directory. It is far from perfect but it serves as a starting point on how to use the engine.

### Build & Run
To run the example, I have provided a `run.sh` script. Just move into the `example` directory and execute the script, for example like `cd example && sh run.sh`. You can also provide the following optional arguments to it:

- `--game-width=X`
- `--game-height=X`
- `--screen-width=X`
- `--screen-height=X`
- `--seed=X`.

Unprovided or invalid arguments will simply retain their default value.

Alternatively, you can build the program with `make` and then run it manually. The arguments can still be provided, but only as pure values, in order. For example like `make && build/snake 9 9 13 13 57193`.

### How to Play
The program will wait for input. Type one of 'w' / 'a' / 's' / 'd' and hit enter to move. You can also enter a space to continue moving in the same direction.
