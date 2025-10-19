#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "Types.h"

#define LEN 30
#define POLYGONES 100000

void outputPolygon(Polygone p) {
    printf("n: %u, ", p.n);
    for (int i = 0; i < p.n; i++) {
        printf("(%g, %g) ", p.vertice[i].x, p.vertice[i].y);
    }
    printf("\n");
}

void input() {
    // to form a file with polygones
    char filename[LEN];
    printf("Enter a name of the file to write polygons to : ");
    scanf("%s", filename);
    unsigned int n;
    printf("Enter the amount of polygons to write into the file : ");
    scanf("%u", &n);
    assert(n >= 0);
    FILE* fp = fopen(filename, "w");
    fwrite(&n, sizeof(unsigned int), 1, fp);
    for (int i = 0; i < n; i++) {
        unsigned int N;
        Polygone* p = (Polygone*) malloc(sizeof(Polygone)); 
        printf("(%d)th polygon. Enter N (the amount of vertices) = ", i+1);
        scanf("%d", &p->n);
        assert(p->n > 2);
        fwrite(&p->n, sizeof(unsigned int), 1, fp);
        p->vertice = (TPoint*) malloc(p->n * sizeof(TPoint));
        for (int j = 0; j < p->n; j++) {
            printf("Enter a vertice (x%i, y%i): ", j+1, j+1);
            scanf("%f %f", &p->vertice[i].x, &p->vertice[i].y);
            fwrite(&p->vertice[i].x, sizeof(float), 1, fp);
            fwrite(&p->vertice[i].y, sizeof(float), 1, fp);
        }
        free(p->vertice);
        free(p);
    }
    fclose(fp);
}

Polygone* readPolygones(FILE* fp) {
    // to read all polygones from a file
    assert(fp);
    unsigned int n;
    fread(&n, sizeof(unsigned int), 1, fp);
    Polygone* polygones = (Polygone*) malloc(n * sizeof(Polygone));
    Polygone p;
    for (int i = 0; i < n; i++) {
        fread(&p.n, sizeof(unsigned int), 1, fp);
        p.vertice = (TPoint*) malloc(p.n * sizeof(TPoint));
        for (int j = 0; j < p.n; j++) {
            fread(&p.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&p.vertice[j].y, sizeof(PTYPE), 1, fp);
        }
        polygones[i] = p;
    }
    return polygones;
}
    
Polygone* readPolygones_from_text(FILE* fp) {
    assert(fp);
    unsigned int n;
    fscanf(fp, "%u", &n);
    Polygone* polygones = (Polygone*) malloc(n * sizeof(Polygone));
    Polygone p;
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%u", &p.n);
        p.vertice = (TPoint*) malloc(p.n * sizeof(TPoint));
        for (int j = 0; j < p.n; j++) {
            fscanf(fp, "%f %f", &p.vertice[j].x, &p.vertice[j].y);
        }
        polygones[i] = p;
    }
    return polygones;
}

void output() {
    // to print put all polygones from the file
    char filename[LEN];
    printf("Enter a name of the file to read polygons from : ");
    scanf("%s", filename);
   
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
        printf("Error: File %s must have .bin or .txt extension.\n", filename);
        return;
    }

    FILE *fp = fopen(filename, is_binary ? "rb" : "r");
    if (!fp) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }

    unsigned int count = 0;
    if (is_binary) {
        fread(&count, sizeof(NTYPE), 1, fp_a);
    } else {
        fscanf(fp_a, "%u", &count);
    }

    Polygone *polygones = (is_binary ? readPolygones(fp) : readPolygones_from_text(fp));
    
    if (polygones) {
        for (unsigned int i = 0; i < count; i++) {
            outputPolygon(polygones[i]);
            free_polygone(&polygones[i]);
        }
        free(polygones);
    }
}
