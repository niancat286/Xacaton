#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "Polygone.h"
#include "file_forming.h"
#include "vectors.h"

#define LEN 30

// Helper function for polygon validation
int isValidPolygon(const Polygone* p) {
    if (p->n < 3) return FALSE;

    // Check for duplicate points
    for (int i = 0; i < p->n; i++) {
        for (int j = i + 1; j < p->n; j++) {
            if (isEqual(p->vertice[i].x, p->vertice[j].x) &&
                isEqual(p->vertice[i].y, p->vertice[j].y)) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

int inputPolygone(FILE* fp, Polygone* p) {
    NTYPE n;

    if (fp) {
        if (fscanf(fp, "%u", &n) != 1) {
            return FALSE;
        }
    }
    else {
        printf("Enter number of polygon vertices: ");
        if (scanf("%u", &n) != 1) {
            return FALSE;
        }
    }

    if (n < 3) {
        printf("Error: number of vertices must be >= 3\n");
        return FALSE;
    }

    p->n = n;
    p->vertice = (TPoint*)malloc(n * sizeof(TPoint));
    if (!p->vertice) {
        printf("Memory allocation error\n");
        return FALSE;
    }

    // Input vertex coordinates
    for (int i = 0; i < n; i++) {
        if (fp) {
            if (fscanf(fp, "%f %f", &p->vertice[i].x, &p->vertice[i].y) != 2) {
                printf("Error reading coordinates for vertex %d\n", i);
                free(p->vertice);
                return FALSE;
            }
        }
        else {
            printf("Enter coordinates for vertex %d (x y): ", i + 1);
            if (scanf("%f %f", &p->vertice[i].x, &p->vertice[i].y) != 2) {
                printf("Error inputting coordinates for vertex %d\n", i);
                free(p->vertice);
                return FALSE;
            }
        }
    }

    // Validate polygon
    if (!isValidPolygon(p)) {
        printf("Error: polygon is invalid (possibly duplicate points)\n");
        free(p->vertice);
        return FALSE;
    }

    return TRUE;
}

int addPolygonesFromFile(const char* source_filename, const char* dest_filename) {
    FILE* source = fopen(source_filename, "r");
    if (!source) {
        printf("Error: cannot open source file %s\n", source_filename);
        return FALSE;
    }

    FILE* dest = fopen(dest_filename, "a+");
    if (!dest) {
        printf("Error: cannot open destination file %s\n", dest_filename);
        fclose(source);
        return FALSE;
    }

    // Read polygon count from source
    unsigned int source_count;
    if (fscanf(source, "%u", &source_count) != 1) {
        printf("Error: invalid source file format\n");
        fclose(source);
        fclose(dest);
        return FALSE;
    }

    // Read current polygon count from dest
    unsigned int dest_count = 0;
    rewind(dest);
    if (fscanf(dest, "%u", &dest_count) != 1) {
        dest_count = 0;
    }

    // Add each polygon
    int added_count = 0;
    for (unsigned int i = 0; i < source_count; i++) {
        Polygone poly;
        if (!inputPolygone(source, &poly)) {
            printf("Warning: skipped invalid polygon %d\n", i);
            continue;
        }

        // Write to destination file
        if (writePolygone(dest, &poly)) {
            added_count++;
            printf("Added polygon %d with %d vertices\n", i, poly.n);
        }
        else {
            printf("Error writing polygon %d\n", i);
        }
        freePolygone(&poly);
    }

    // Update total count in destination file
    if (added_count > 0) {
        rewind(dest);
        fprintf(dest, "%u", dest_count + added_count);
        printf("Successfully added %d polygons from file %s\n", added_count, source_filename);
    }

    fclose(source);
    fclose(dest);
    return added_count;
}

// File operations
int writePolygone_binary(FILE* fp, Polygone* p) {
    assert(fp != 0);
    assert(p != 0);
    int size = fwrite(&p->n, sizeof(NTYPE), 1, fp);
    if (size != 1) return 0;
    for (int i = 0; i < p->n; i++) {
        int size1 = fwrite(&p->vertice[i].x, sizeof(PTYPE), 1, fp);
        int size2 = fwrite(&p->vertice[i].y, sizeof(PTYPE), 1, fp);
        if (size1 != 1 || size2 != 1) return 0;
    }
    return 1;
}

// Text file polygon writing
int writePolygone(FILE* fp, Polygone* p) {
    assert(fp != 0);
    assert(p != 0);

    // Write vertex count
    if (fprintf(fp, "\n%u", p->n) < 0) {
        return FALSE;
    }

    // Write vertex coordinates
    for (int i = 0; i < p->n; i++) {
        if (fprintf(fp, " %.6f %.6f", p->vertice[i].x, p->vertice[i].y) < 0) {
            return FALSE;
        }
    }

    return TRUE;
}

// Read polygon from text file
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

// Output polygon function
void outputPolygone(const Polygone* p, FILE* fp) {
    if (!p) return;

    if (fp == NULL) {
        // Output to console
        printf("Polygon with %u vertices: ", p->n);
        for (int i = 0; i < p->n; i++) {
            printf("(%.2f, %.2f) ", p->vertice[i].x, p->vertice[i].y);
        }
        printf("\n");
    }
    else {
        // Output to file
        fprintf(fp, "Polygon with %u vertices: ", p->n);
        for (int i = 0; i < p->n; i++) {
            fprintf(fp, "(%.2f, %.2f) ", p->vertice[i].x, p->vertice[i].y);
        }
        fprintf(fp, "\n");
    }
}

// Output functions
void showPolygoneFile(FILE* fp, NTYPE k) {
    // Simplified implementation - show all polygons
    showPolygonesFile(fp);
}

void showPolygonesFile(FILE* fp) {
    if (!fp) return;

    rewind(fp);
    unsigned int count;
    if (fscanf(fp, "%u", &count) != 1) return;

    for (unsigned int i = 0; i < count; i++) {
        Polygone p;
        if (readPolygoneFromText(fp, &p)) {
            outputPolygon(p);
            freePolygone(&p);
        }
    }
}

// Geometric functions
PTYPE area(TPoint p1, TPoint p2, TPoint p3) {
    TVECT v1 = setVector(p2, p1);
    TVECT v2 = setVector(p2, p3);
    PTYPE par_area = lengthVector(vectorMultVector(v1, v2));
    return par_area / 2.0;
}

int freePolygone(Polygone* p) {
    if (p->vertice) {
        free(p->vertice);
        p->vertice = NULL;
    }
    p->n = 0;
    return 0;
}

PTYPE area_polygon(Polygone p) {
    if (p.n < 3) return 0.0;

    PTYPE total_area = 0.0;
    for (int i = 0; i < p.n; i++) {
        TPoint current = p.vertice[i];
        TPoint next = p.vertice[(i + 1) % p.n];
        total_area += (current.x * next.y - next.x * current.y);
    }
    return fabs(total_area) / 2.0;
}

NTYPE inPolygon(Polygone p, TPoint point) {
    NTYPE power = p.n;
    PTYPE res = 0;
    for (int i = 0; i < power - 1; i++) {
        res += area(p.vertice[i], point, p.vertice[i + 1]);
    }
    res += area(p.vertice[0], point, p.vertice[power - 1]);
    if (isEqual(area_polygon(p), res)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// Polygone file operations
NTYPE pointsPolygones(FILE* fp, TPoint point) {
    assert(fp != 0);
    int res = 0;
    unsigned int M;

    rewind(fp);
    if (fscanf(fp, "%u", &M) != 1) return 0;

    for (int i = 0; i < M; i++) {
        Polygone p;
        if (readPolygoneFromText(fp, &p)) {
            if (inPolygon(p, point)) {
                res++;
            }
            freePolygone(&p);
        }
    }
    return res;
}

int minAreaPolygone(FILE* fp, Polygone* p) {
    assert(fp != NULL);
    Polygone temp;
    PTYPE min_area = 0;
    int found = FALSE;
    unsigned int M;

    rewind(fp);
    if (fscanf(fp, "%u", &M) != 1) return FALSE;

    for (int i = 0; i < M; i++) {
        if (!readPolygoneFromText(fp, &temp)) continue;

        PTYPE area_val = area_polygon(temp);
        if (!found || area_val < min_area) {
            min_area = area_val;
            found = TRUE;
            if (p->vertice) {
                free(p->vertice);
            }
            p->n = temp.n;
            p->vertice = (TPoint*)malloc(temp.n * sizeof(TPoint));
            for (int j = 0; j < temp.n; j++) {
                p->vertice[j].x = temp.vertice[j].x;
                p->vertice[j].y = temp.vertice[j].y;
            }
        }
        freePolygone(&temp);
    }
    return found;
}

int isConvexPolygone(const Polygone* p) {
    if (p->n < 3) return FALSE;
    int sign = 0;
    for (int i = 0; i < p->n; i++) {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        TPoint p3 = p->vertice[(i + 2) % p->n];

        TVECT v1 = setVector(p1, p2);
        TVECT v2 = setVector(p2, p3);
        TVECT cross = vectorMultVector(v1, v2);
        PTYPE cross_product_z = cross.z;

        if (cross_product_z != 0) {
            int current_sign = (cross_product_z > 0) ? 1 : -1;
            if (sign == 0) {
                sign = current_sign;
            }
            else if (sign != current_sign) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

NTYPE numberConvexPolygones(FILE* fp) {
    assert(fp != NULL);
    unsigned int M;
    int count = 0;

    rewind(fp);
    if (fscanf(fp, "%u", &M) != 1) return 0;

    for (int i = 0; i < M; i++) {
        Polygone p;
        if (readPolygoneFromText(fp, &p)) {
            if (isConvexPolygone(&p)) {
                count++;
            }
            freePolygone(&p);
        }
    }
    return count;
}

// Additional geometric functions
int isEqualPolygone(const Polygone* p1, const Polygone* p2) {
    if (p1->n != p2->n) return FALSE;

    for (int i = 0; i < p1->n; i++) {
        if (!isEqual(p1->vertice[i].x, p2->vertice[i].x) ||
            !isEqual(p1->vertice[i].y, p2->vertice[i].y)) {
            return FALSE;
        }
    }
    return TRUE;
}

PTYPE perimeterPolygone(const Polygone* p) {
    if (p->n < 2) return 0.0;

    PTYPE perimeter = 0.0;
    for (int i = 0; i < p->n; i++) {
        TPoint current = p->vertice[i];
        TPoint next = p->vertice[(i + 1) % p->n];
        TVECT side = setVector(current, next);
        perimeter += lengthVector(side);
    }
    return perimeter;
}

// Simple output function for file_forming.c compatibility
void outputPolygon(Polygone p) {
    outputPolygone(&p, NULL);
}