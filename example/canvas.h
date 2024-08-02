#ifndef CANVAS_H
#define CANVAS_H

#include <stddef.h>
#include <stdio.h>

struct canvas {
    size_t width, height;
    char *pixels;
};

void canvas_init(struct canvas *canvas, size_t width, size_t height, void *(*allocfn)(size_t));
char canvas_pixel_get(const struct canvas *canvas, size_t x, size_t y);
void canvas_pixel_set(struct canvas *canvas, size_t x, size_t y, char c);
void canvas_rect_fill(struct canvas *canvas, size_t x0, size_t y0, size_t x1, size_t y1, char c);
void canvas_rect_box(struct canvas *canvas, size_t x0, size_t y0, size_t x1, size_t y1, char c);
void canvas_fill(struct canvas *canvas, char c);
void canvas_deinit(struct canvas *canvas, void (*deallocfn)(void *));

#ifdef CANVAS_C

void canvas_init(struct canvas *canvas, size_t width, size_t height, void *(*allocfn)(size_t)) {
    *canvas = (struct canvas) {
        .width = width, .height = height,
        .pixels = allocfn(width * height * sizeof(char)),
    };
}

char canvas_pixel_get(const struct canvas *canvas, size_t x, size_t y) {
    size_t i = (x % canvas->width) + (y % canvas->height) * canvas->width;
    return canvas->pixels[i];
}

void canvas_pixel_set(struct canvas *canvas, size_t x, size_t y, char c) {
    size_t i = (x % canvas->width) + (y % canvas->height) * canvas->width;
    canvas->pixels[i] = c;
}

void canvas_rect_fill(struct canvas *canvas, size_t x0, size_t y0, size_t x1, size_t y1, char c) {
    for (size_t y = y0; y <= y1; y++) {
        for (size_t x = x0; x <= x1; x++) {
            canvas_pixel_set(canvas, x, y, c);
        }
    }
}

void canvas_rect_box(struct canvas *canvas, size_t x0, size_t y0, size_t x1, size_t y1, char c) {
    for (size_t x = x0; x <= x1; x++) {
        canvas_pixel_set(canvas, x, 0, c);
        canvas_pixel_set(canvas, x, canvas->height - 1, c);
    }
    for (size_t y = y0; y <= y1; y++) {
        canvas_pixel_set(canvas, 0, y, c);
        canvas_pixel_set(canvas, canvas->width - 1, y, c);
    }
}

void canvas_fill(struct canvas *canvas, char c) {
    canvas_rect_fill(canvas, 0, 0, canvas->width - 1, canvas->height - 1, c);
}

void canvas_deinit(struct canvas *canvas, void (*deallocfn)(void *)) {
    deallocfn(canvas->pixels);
}

#endif

#endif