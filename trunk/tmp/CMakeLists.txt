CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libs)

FIND_PACKAGE(JPEG REQUIRED)
FIND_PACKAGE(PNG  REQUIRED)
INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR})

ADD_LIBRARY(demo STATIC 
bitmaplib.c
bitmaplib.h
complexlib.c
complexlib.h
domelib.c
domelib.h
hexlib.c
hexlib.h
jpeg.c
jpeg.h
offlib.c
offlib.h
opengllib.c
opengllib.h
particlelib.c
particlelib.h
paulslib.c
paulslib.h
perlin.c
perlin.h
povraylib.c
povraylib.h
seriallib.c
seriallib.h
simplex.h
wrllib.c
wrllib.h
libpng-short-example.c
)

