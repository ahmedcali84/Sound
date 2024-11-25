#!/bin/bash

CFLAGS=$(sdl2-config --cflags --libs)
OUTPUT="sound"
SOURCE="sound.cpp"
g++ $SOURCE -o $OUTPUT $CFLAGS

if [ $? -eq 0 ]; then
    echo "[Success]: Compiled $SOURCE Successfully."
    echo "RUN ./$OUTPUT -d to get Sound details."
    echo "RUN ./$OUTPUT -v time(in milliseconds) to Get Sine Wave Visualization"
else
    echo "[ERROR]: Compilation Failed."
fi
