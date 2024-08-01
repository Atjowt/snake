/*
    Make sure to define SNAKE_C before including in source file!
    ...
    #define SNAKE_C
    #include "snake.h"
    ...
*/

#ifndef SNAKE_H
#define SNAKE_H

#include <stddef.h>

struct snake_game {
    int (*randfn)(void);
    size_t width, height;
    struct {
        size_t length;
        enum snake_direction {
            SNAKE_DIRECTION_RIGHT,
            SNAKE_DIRECTION_UP,
            SNAKE_DIRECTION_LEFT,
            SNAKE_DIRECTION_DOWN,
        } direction;
        struct snake_segment {
            size_t x, y;
        } *segments;
    } snake;
    struct snake_food {
        size_t x, y;
    } food;
};

enum snake_result {
    SNAKE_RESULT_OK,
    SNAKE_RESULT_ATE_FOOD,
    SNAKE_RESULT_HIT_SELF,
    SNAKE_RESULT_HIT_WALL,
};

void snake_game_init(struct snake_game *game, size_t width, size_t height, void *(*allocfn)(size_t), int (*randfn)(void));
enum snake_result snake_game_update(struct snake_game *game);
void snake_game_deinit(struct snake_game *game, void (*deallocfn)(void *));

#ifdef SNAKE_C

void snake_randomize_food(struct snake_game *game) {
    bool collision;
    do {
        game->food.x = game->randfn() % game->width;
        game->food.y = game->randfn() % game->height;
        collision = false;
        for (size_t i = 0; i < game->snake.length; i++) {
            struct snake_segment segment = game->snake.segments[i];
            if (game->food.x == segment.x && game->food.y == segment.y) {
                collision = true;
                break;
            }
        }
    } while (collision);
}

void snake_game_init(struct snake_game *game, size_t width, size_t height, void *(*allocfn)(size_t), int (*randfn)(void)) {
    game->width = width;
    game->height = height;
    game->snake.segments = (struct snake_segment*)allocfn(width * height * sizeof(struct snake_segment));
    game->snake.direction = SNAKE_DIRECTION_RIGHT;
    game->snake.length = 1U;
    struct snake_segment *head = &game->snake.segments[0];
    head->x = game->width / 2;
    head->y = game->height / 2;
    game->randfn = randfn;
    snake_randomize_food(game);
}

enum snake_result snake_game_update(struct snake_game *game) {

    enum snake_result result = SNAKE_RESULT_OK;

    struct snake_segment *head = &game->snake.segments[0];

    struct snake_segment new_head = *head;
    switch (game->snake.direction) {
        case SNAKE_DIRECTION_RIGHT: {
            if (head->x == game->width - 1) { result = SNAKE_RESULT_HIT_WALL; }
            else { new_head.x++; }
            break;
        }
        case SNAKE_DIRECTION_LEFT: {
            if (head->x == 0) { result = SNAKE_RESULT_HIT_WALL; }
            else { new_head.x--; }
            break;
        }
        case SNAKE_DIRECTION_UP: {
            if (head->y == 0) { result = SNAKE_RESULT_HIT_WALL; }
            else { new_head.y--; }
            break;
        }
        case SNAKE_DIRECTION_DOWN: {
            if (head->y == game->height - 1) { result = SNAKE_RESULT_HIT_WALL; }
            else { new_head.y++; }
            break;
        }
    }

    if (result == SNAKE_RESULT_HIT_WALL) return SNAKE_RESULT_HIT_WALL;

    for (size_t i = game->snake.length - 1; i >= 1; --i) {
        game->snake.segments[i] = game->snake.segments[i - 1];
    }

    *head = new_head;

    if (head->x == game->food.x && head->y == game->food.y) {
        game->snake.length++;
        game->snake.segments[game->snake.length - 1] = game->snake.segments[game->snake.length - 2];
        result = SNAKE_RESULT_ATE_FOOD;
        snake_randomize_food(game);
    }

    if (result == SNAKE_RESULT_ATE_FOOD) return SNAKE_RESULT_ATE_FOOD;

    for (size_t i = 1; i < game->snake.length; i++) {
        struct snake_segment segment = game->snake.segments[i];
        if (head->x == segment.x && head->y == segment.y) {
            result = SNAKE_RESULT_HIT_SELF;
            break;
        }
    }

    return result;
}

void snake_game_deinit(struct snake_game *game, void (*deallocfn)(void *)) {
    deallocfn(game->snake.segments);
    game->width = 0;
    game->height = 0;
}

#endif

#endif