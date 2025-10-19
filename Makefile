# all: clean RunTests.exe

# RunTests.exe: Polygone.o testPolygone.o main.o vectors.o linerays.o
# 	gcc -o RunTests.exe Polygone.o testPolygone.o main.o

# Polygone.o:
# 	gcc Polygone.c Predicat.c -c

# testPolygone.o:
# 	gcc testsPolygone.c Predicat.c -c

# main.o:
# 	gcc testsPolygone.c main.c -c  

# linerays.o:
# 	gcc linerays.c -c	

# vectors.o:
# 	gcc vectors.c -c

# clean:
# 	rm -f *.o RunTests.exe 


#for Windows
CC = gcc

CFLAGS = -g -Wall -Wextra
LDFLAGS =

TARGET = RunTests.exe


SRCS = main.c Polygone.c testsPolygone.c Predicat.c vectors.c linerays.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
ifeq ($(OS),Windows_NT)
	-del /F /Q *.o $(TARGET) > nul 2>&1
else
	-rm -f *.o $(TARGET)
endif