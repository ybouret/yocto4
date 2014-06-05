SRCS :=        \
bitmaplib.c    \
complexlib.c   \
domelib.c      \
hexlib.c       \
jpeg.c         \
offlib.c       \
opengllib.c    \
particlelib.c  \
paulslib.c     \
perlin.c       \
povraylib.c    \
seriallib.c    \
wrllib.c

OBJS := $(SRCS:.c=.o)

CC := gcc
CFLAGS := -Wall -I/opt/local/include

all: $(OBJS)
	@echo $(OBJS)

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o a.out a.exe

