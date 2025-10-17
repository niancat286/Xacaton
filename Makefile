all: clean RunTests.exe

RunTests.exe: Polygone.o testPolygone.o main.o vectors.o linerays.o
	gcc -o RunTests.exe Polygone.o testPolygone.o main.o

Polygone.o:
	gcc Polygone.c Predicat.c -c

testPolygone.o:
	gcc testsPolygone.c Predicat.c -c

main.o:
	gcc testsPolygone.c main.c -c  

linerays.o:
	gcc linerays.c -c	

vectors.o:
	gcc vectors.c -c

clean:
	rm -f *.o RunTests.exe 




