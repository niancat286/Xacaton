TARGET = RunTests

CC = gcc
CFLAGS = -Wall -Wextra -g

OBJS = Polygone.o testsPolygone.o main.o vectors.o linerays.o Predicat.o

.PHONY: all clean

all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

Polygone.o: Polygone.c
	$(CC) $(CFLAGS) -c Polygone.c

testsPolygone.o: testsPolygone.c
	$(CC) $(CFLAGS) -c testsPolygone.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

linerays.o: linerays.c
	$(CC) $(CFLAGS) -c linerays.c

vectors.o: vectors.c
	$(CC) $(CFLAGS) -c vectors.c

Predicat.o: Predicat.c
	$(CC) $(CFLAGS) -c Predicat.c

clean:
	rm -f *.o $(TARGET)





# Для вінди друзякі собі
#CC = gcc

#CFLAGS = -g -Wall -Wextra
#LDFLAGS =

#TARGET = RunTests.exe


#SRCS = main.c Polygone.c testsPolygone.c Predicat.c vectors.c linerays.c

#OBJS = $(SRCS:.c=.o)

#all: $(TARGET)

#$(TARGET): $(OBJS)
	#$(CC) $(LDFLAGS) -o $@ $^

#%.o: %.c
	#$(CC) $(CFLAGS) -c $< -o $@


#clean:
#ifeq ($(OS),Windows_NT)
#	-del /F /Q *.o $(TARGET) > nul 2>&1
#else
#	-rm -f *.o $(TARGET)
#endif