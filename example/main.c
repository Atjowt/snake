#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define SNAKE_C
#include "snake.h"

#define CANVAS_C
#include "canvas.h"

void draw_snake(const struct snake_game *game, struct canvas *canvas, size_t x0, size_t y0) {
    for (size_t i = 1; i < game->snake.length; i++) {
        struct snake_segment segment = game->snake.segments[i];
        canvas_pixel_set(canvas, x0 + segment.x, y0 + segment.y, '@');
    }
    struct snake_segment head = game->snake.segments[0];
    canvas_pixel_set(canvas, x0 + head.x, y0 + head.y, 'O');
}

void draw_food(const struct snake_game *game, struct canvas *canvas, size_t x0, size_t y0) {
    canvas_pixel_set(canvas, x0 + game->food.x, y0 + game->food.y, 'F');
}

void draw_background(const struct snake_game *game, struct canvas *canvas, size_t x0, size_t y0) {
    canvas_rect_fill(canvas, x0, y0, x0 + game->width - 1, y0 + game->height - 1, '.');
}

void draw_bounds(const struct snake_game *game, struct canvas *canvas, size_t x0, size_t y0) {
    if (x0 >= 1) {
        for (size_t y = 0; y < game->height; y++) {
            canvas_pixel_set(canvas, x0 - 1, y0 + y, '>');
        }
    }
    if (x0 + game->width < canvas->width) {
        for (size_t y = 0; y < game->height; y++) {
            canvas_pixel_set(canvas, x0 + game->width, y0 + y, '<');
        }
    }
    if (y0 >= 1) {
        for (size_t x = 0; x < game->width; x++) {
            canvas_pixel_set(canvas, x0 + x, y0 - 1, 'v');
        }
    }
    if (y0 + game->height < canvas->height) {
        for (size_t x = 0; x < game->width; x++) {
            canvas_pixel_set(canvas, x0 + x, y0 + game->height, '^');
        }
    }
}

void draw(const struct snake_game *game, struct canvas *canvas, size_t x0, size_t y0) {
    draw_background(game, canvas, x0, y0);
    draw_snake(game, canvas, x0, y0);
    draw_food(game, canvas, x0, y0);
    draw_bounds(game, canvas, x0, y0);
}

void display(const struct canvas *canvas) {
    system("clear");
    for (size_t y = 0; y < canvas->height; y++) {
        for (size_t x = 0; x < canvas->width; x++) {
            char c = canvas_pixel_get(canvas, x, y);
            putchar(c);
            putchar(' ');
        }
        putchar('\n');
    }
}

bool parse_direction(char c, enum snake_direction *direction) {
    switch (c) {
        case 'w': *direction = SNAKE_DIRECTION_UP;    break;
        case 'a': *direction = SNAKE_DIRECTION_LEFT;  break;
        case 's': *direction = SNAKE_DIRECTION_DOWN;  break;
        case 'd': *direction = SNAKE_DIRECTION_RIGHT; break;
        case ' ': break; // continue
        default: return false;
    }
    return true;
}

int main(int argc, char* argv[]) {

    size_t game_width = 9;
    if (argc > 1) {
        int value = atoi(argv[1]);
        if (value > 0) {
            game_width = value;
        } else {
            fprintf(stderr, "Bad game width, using default\n");
        }
    }

    size_t game_height = 9;
    if (argc > 2) {
        int value = atoi(argv[2]);
        if (value > 0) {
            game_height = value;
        } else {
            fprintf(stderr, "Bad game height, using default\n");
        }
    }

    size_t screen_width = game_width;
    if (argc > 3) {
        int value = atoi(argv[3]);
        if (value > 0 && (size_t)value >= game_width) {
            screen_width = value;
        } else {
            fprintf(stderr, "Bad screen width, using default\n");
        }
    }

    size_t screen_height = game_height;
    if (argc > 4) {
        int value = atoi(argv[4]);
        if (value > 0 && (size_t)value >= game_height) {
            screen_height = value;
        } else {
            fprintf(stderr, "Bad screen height, using default\n");
        }
    }

    unsigned int seed = time(NULL);
    if (argc > 5) {
        int value = atoi(argv[5]);
        if (value >= 0) {
            seed = value;
        } else {
            fprintf(stderr, "Bad seed, using default\n");
        }
    }
    
    srand(seed);

    struct canvas canvas;
    canvas_init(&canvas, screen_width, screen_height, malloc);

    struct snake_game game;
    snake_game_init(&game, game_width, game_height, malloc, rand);

    size_t x0 = (canvas.width - game.width) / 2; 
    size_t y0 = (canvas.height - game.height) / 2;

    bool quit = false;

    while (!quit) {

        canvas_fill(&canvas, '+');
        draw(&game, &canvas, x0, y0);
        display(&canvas);

        char c;
        do {
            c = getchar();
        } while(!parse_direction(c, &game.snake.direction));

        enum snake_result result = snake_game_update(&game);
        switch (result) {
            case SNAKE_RESULT_OK: {
                break;
            }
            case SNAKE_RESULT_ATE_FOOD: {
                printf("Nom nom\n");
                break;
            }
            case SNAKE_RESULT_HIT_SELF:
            case SNAKE_RESULT_HIT_WALL: {
                printf("YEOUCH!\n");
                quit = true;
                break;
            }
        }
    }

    snake_game_deinit(&game, free);
    canvas_deinit(&canvas, free);

    return 0;
}