#include <stdio.h>
#include <stdlib.h>

#include "Types.h"
#include "Predicat.h"
#include "testsPolygone.h"

float createPoly(int n, char* fname){

	srand(time(NULL));

	FILE* f = fopen(fname, "wb");

	int p1 = 3;

	float s = 0;

	for(int i=0;i<n;i++){
		Polygone p;
		p.n = 3;
		p.vertice= calloc(3, sizeof(TPoint));
		p.vertice[0].x =  rand() / (float)RAND_MAX - 0.5;
		p.vertice[0].y =  rand() / (float)RAND_MAX - 0.5;

		p.vertice[1].x = p.vertice[0].x + (i%4)+1;
		p.vertice[1].y = p.vertice[0].y;

		p.vertice[2].x = p.vertice[0].x;
		p.vertice[2].y =  p.vertice[0].y + (i/4)%4+1;

		writePolygones(f,p,1);
		s += ((i%4)+1) * ((i/4)%4+1);
	}

	return s;
}


float createPoly2(int n, char* fname) {
    srand(time(NULL));

    FILE* f = fopen(fname, "wb");
    if (!f) {
        perror("");
        return -1;
    }

    int p1 = 3;
    int p2 = 30;

    float s = 0;

    for (int i = 0; i < n; i++) {
        Polygone p;
        p.n = p1 + rand() % (p2 - p1 + 1);  

        p.vertice = calloc(p.n, sizeof(TPoint));
        if (!p.vertice) {
            perror("");
            fclose(f);
            return -1;
        }

        p.vertice[0].x = rand() / (float)RAND_MAX - 0.5;
        p.vertice[0].y = rand() / (float)RAND_MAX - 0.5;

        for (int j = 1; j < p.n; j++) {
            float v1 = rand() / (float)RAND_MAX;
            float v2 = rand() / (float)RAND_MAX;

            int sign1 = (i > n / 2) ? -1 : 1;
            int sign2 = 2 * ((i / (n / 4) % 2) - 1);

            p.vertice[j].x = p.vertice[j - 1].x + sign1 * v1;
            p.vertice[j].y = p.vertice[j - 1].y + sign2 * v2;
        }

        writePolygones(f, p, 1);
        s += ((i % 4) + 1) * ((i / 4) % 4 + 1);

        free(p.vertice); 
    }

    fclose(f); 
    return s;
}
