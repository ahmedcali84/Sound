#!/bin/bash

CFLAGS=$(sdl2-config --cflags --libs)
OUTPUT="sound"
SOURCE="sound.cpp"
g++ $SOURCE -o $OUTPUT $CFLAGS
