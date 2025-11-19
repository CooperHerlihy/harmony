#!/bin/bash

START_TIME=$(date +%s.%N)

SRC_DIR="."
BUILD_DIR="build"

STD="-std=gnu99"
WARNINGS="-Werror -Wall -Wextra -Wconversion -Wshadow -pedantic"
CONFIG="-g -O1 -fsanitize=undefined"

INCLUDES="\
    -Iinclude\
    -Ivendor/libX11/include\
    -Ivendor/Vulkan-Headers/include\
    -Ivendor/stb\
    "

LIBS="\
    -L${BUILD_DIR}\
    -lharmony\
    -lm\
    -lc\
    "

mkdir -p ${BUILD_DIR}

if [ ! -f ${BUILD_DIR}/stb.o ]; then
    echo stb.c
    gcc -c ${SRC_DIR}/src/stb.c \
        -o ${BUILD_DIR}/stb.o \
        ${STD} ${CONFIG} ${INCLUDES}
fi

echo harmony.c
gcc -c ${SRC_DIR}/src/harmony.c \
    -o ${BUILD_DIR}/harmony.o \
    ${STD} ${WARNINGS} ${CONFIG} ${INCLUDES}

echo harmony_linux.c
gcc -c ${SRC_DIR}/src/harmony_linux.c \
    -o ${BUILD_DIR}/harmony_linux.o \
    ${STD} ${WARNINGS} ${CONFIG} ${INCLUDES}

echo harmony_vulkan.c
gcc -c ${SRC_DIR}/src/harmony_vulkan.c \
    -o ${BUILD_DIR}/harmony_vulkan.o \
    ${STD} ${WARNINGS} ${CONFIG} ${INCLUDES}

echo libharmony.a
ar rcs ${BUILD_DIR}/libharmony.a \
    ${BUILD_DIR}/harmony.o \
    ${BUILD_DIR}/harmony_linux.o \
    ${BUILD_DIR}/harmony_vulkan.o \
    ${BUILD_DIR}/stb.o

echo harmony_embed
gcc ${SRC_DIR}/src/harmony_embed.c \
    -o ${BUILD_DIR}/harmony_embed \
    ${STD} ${WARNINGS} ${CONFIG} ${INCLUDES} ${LIBS}

END_TIME=$(date +%s.%N)
printf "Build complete: %.6f seconds\n" "$(echo "$END_TIME - $START_TIME" | bc)"

exit 0
