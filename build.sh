#!/bin/bash

START_TIME=$(date +%s.%N)

SRC_DIR="."
BUILD_DIR="build"

STD="-std=c11"
WARNINGS="-Werror -Wall -Wextra -Wconversion -Wshadow -pedantic"
CONFIG="-g -O1 -fsanitize=undefined"

INCLUDES="\
    -Iinclude\
    -Ivendor/libX11/include\
    -Ivendor/Vulkan-Headers/include\
    "

LIBS="\
    -lm\
    -lc\
    "

mkdir -p ${BUILD_DIR}

echo harmony_embed
gcc ${SRC_DIR}/src/harmony_embed.c \
    -o ${BUILD_DIR}/harmony_embed \
    ${STD} ${WARNINGS} ${CONFIG} ${INCLUDES} ${LIBS}

echo harmony_test.c
gcc ${SRC_DIR}/src/harmony_test.c \
    -o ${BUILD_DIR}/harmony_test \
    ${STD} ${WARNINGS} ${CONFIG} ${INCLUDES} ${LIBS}

END_TIME=$(date +%s.%N)
printf "Build complete: %.6f seconds\n" "$(echo "$END_TIME - $START_TIME" | bc)"

exit 0
