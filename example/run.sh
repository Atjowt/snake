#!/bin/bash

usage() {
    echo "Usage: $0 [--game-width=] [--game-height=] [--screen-width=] [--screen-height=] [--seed=]"
    exit 1
}

game_width=""
game_height=""
screen_width=""
screen_height=""
seed=""

while [ "$#" -gt 0 ]; do
    case "$1" in
        --game-width=*)
            game_width="${1#*=}"
            shift 1
            ;;
        --game-height=*)
            game_height="${1#*=}"
            shift 1
            ;;
        --screen-width=*)
            screen_width="${1#*=}"
            shift 1
            ;;
        --screen-height=*)
            screen_height="${1#*=}"
            shift 1
            ;;
        --seed=*)
            seed="${1#*=}"
            shift 1
            ;;
        *)
            usage
            ;;
    esac
done

sh build.sh && ./snake $game_width $game_height $screen_width $screen_height $seed
