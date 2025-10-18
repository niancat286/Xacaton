#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "Types.h"
#include "Polygone.h"

#define LEN 30
#define POLYGONES 100000

void outputPolygon(Polygone p) {
    printf("Vertices: %u, Points: ", p.n);
    for (int i = 0; i < p.n; i++) {
        printf("(%.2f, %.2f) ", p.vertice[i].x, p.vertice[i].y);
    }
    printf("\n");
}

void input() {
    // Create a file with polygons in text format
    char filename[LEN];
    printf("Enter filename to write polygons: ");
    scanf("%s", filename);

    unsigned int n;
    printf("Enter number of polygons to write: ");
    scanf("%u", &n);

    assert(n > 0);

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: cannot create file %s\n", filename);
        return;
    }

    // Write polygon count in text format
    fprintf(fp, "%u", n);

    for (int i = 0; i < n; i++) {
        Polygone p;
        printf("\nPolygon %d:\n", i + 1);

        // Input polygon using our existing function
        if (!inputPolygone(NULL, &p)) {
            printf("Error: invalid polygon, skipping...\n");
            i--; // Retry this polygon
            continue;
        }

        // Write polygon in text format
        if (!writePolygone(fp, &p)) {
            printf("Error writing polygon %d\n", i + 1);
        }

        freePolygone(&p);
    }

    fclose(fp);
    printf("Successfully wrote %u polygons to %s\n", n, filename);
}

Polygone* readPolygones(FILE* fp) {
    // Read all polygons from a text file
    assert(fp);

    rewind(fp);
    unsigned int n;
    if (fscanf(fp, "%u", &n) != 1) {
        return NULL;
    }

    Polygone* polygones = (Polygone*)malloc((n + 1) * sizeof(Polygone));
    if (!polygones) return NULL;

    for (int i = 0; i < n; i++) {
        if (!readPolygoneFromText(fp, &polygones[i])) {
            // Mark end of array if error occurs
            polygones[i].n = 0;
            polygones[i].vertice = NULL;
            break;
        }
    }

    // Mark end of array
    polygones[n].n = 0;
    polygones[n].vertice = NULL;

    return polygones;
}

void output() {
    // Print all polygons from the file
    char filename[LEN];
    printf("Enter filename to read polygons from: ");
    scanf("%s", filename);

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: cannot open file %s\n", filename);
        return;
    }

    Polygone* polygones = readPolygones(fp);
    if (!polygones) {
        printf("Error: cannot read polygons from file\n");
        fclose(fp);
        return;
    }

    printf("\nPolygons in file %s:\n", filename);
    int i = 0;
    while (polygones[i].n != 0) {
        printf("Polygon %d: ", i + 1);
        outputPolygon(polygones[i]);
        i++;
    }

    // Free memory
    for (int j = 0; j < i; j++) {
        freePolygone(&polygones[j]);
    }
    free(polygones);

    fclose(fp);
}

// Additional helper function for text file operations
int readPolygoneFromText(FILE* fp, Polygone* p) {
    if (!fp || !p) return FALSE;

    if (fscanf(fp, "%u", &p->n) != 1) {
        return FALSE;
    }

    if (p->n < 3) return FALSE;

    p->vertice = (TPoint*)malloc(p->n * sizeof(TPoint));
    if (!p->vertice) return FALSE;

    for (int i = 0; i < p->n; i++) {
        if (fscanf(fp, "%f %f", &p->vertice[i].x, &p->vertice[i].y) != 2) {
            free(p->vertice);
            p->vertice = NULL;
            return FALSE;
        }
    }

    return TRUE;
}
