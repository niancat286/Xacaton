#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#include "Polygone.h"
#include "file_forming.h"
#include "vectors.h"

#define MAX_POLYGONS 100000

int writePolygone(FILE* fp, Polygone* p) {
    if (!fp || !p) return FALSE;
    if (fprintf(fp, "%u", (unsigned)p->n) < 0) return FALSE;
    for (int i = 0; i < p->n; ++i) {
        if (fprintf(fp, " %.6f %.6f", p->vertice[i].x, p->vertice[i].y) < 0) return FALSE;
    }
    if (fprintf(fp, "\n") < 0) return FALSE;
    return TRUE;
}

int readPolygoneFromText(FILE* fp, Polygone* p) {
    if (!fp || !p) return FALSE;
    unsigned n;
    if (fscanf(fp, "%u", &n) != 1) return FALSE;
    if (n < 3) return FALSE;
    p->n = (NTYPE)n;
    p->vertice = (TPoint*)malloc(p->n * sizeof(TPoint));
    if (!p->vertice) return FALSE;
    for (unsigned i = 0; i < n; ++i) {
        if (fscanf(fp, "%f %f", &p->vertice[i].x, &p->vertice[i].y) != 2) {
            free(p->vertice);
            p->vertice = NULL;
            p->n = 0;
            return FALSE;
        }
    }
    return TRUE;
}

/* Read all polygons stored in a text file.
   Returns dynamically allocated array of Polygone with an extra sentinel
   element (arr[count].n == 0). Caller must free each polygon via
   freePolygone(&arr[i]) then free(arr). */
Polygone* readPolygones(FILE* fp) {
    if (!fp) return NULL;
    rewind(fp);
    unsigned count;
    if (fscanf(fp, "%u", &count) != 1) return NULL;
    Polygone* arr = (Polygone*)malloc((count + 1) * sizeof(Polygone));
    if (!arr) return NULL;
    for (unsigned i = 0; i < count; ++i) {
        arr[i].n = 0;
        arr[i].vertice = NULL;
        if (!readPolygoneFromText(fp, &arr[i])) {
            for (unsigned j = 0; j < i; ++j) freePolygone(&arr[j]);
            free(arr);
            return NULL;
        }
    }
    arr[count].n = 0;
    arr[count].vertice = NULL;
    return arr;
}

/* Input polygon from file or console.
   If fp != NULL -> read from file; otherwise read from console using scanf_s.
   Validate polygon via isValidPolygon before returning TRUE. */
int inputPolygone(FILE* fp, Polygone* p) {
    if (!p) return FALSE;
    unsigned n;
    if (fp) {
        if (fscanf(fp, "%u", &n) != 1) return FALSE;
    } else {
        printf("Enter number of polygon vertices: ");
        if (scanf_s("%u", &n) != 1) {
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            return FALSE;
        }
    }
    if (n < 3) {
        if (!fp) printf("Error: number of vertices must be >= 3\n");
        return FALSE;
    }
    p->n = (NTYPE)n;
    p->vertice = (TPoint*)malloc(p->n * sizeof(TPoint));
    if (!p->vertice) return FALSE;
    for (unsigned i = 0; i < n; ++i) {
        if (fp) {
            if (fscanf(fp, "%f %f", &p->vertice[i].x, &p->vertice[i].y) != 2) {
                free(p->vertice);
                p->vertice = NULL;
                p->n = 0;
                return FALSE;
            }
        } else {
            printf("Enter coordinates for vertex %u (x y): ", i + 1);
            if (scanf_s("%f %f", &p->vertice[i].x, &p->vertice[i].y) != 2) {
                int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                free(p->vertice);
                p->vertice = NULL;
                p->n = 0;
                return FALSE;
            }
        }
    }
    if (!isValidPolygon(p)) {
        if (!fp) printf("Error: polygon is invalid (duplicate points or self-intersection)\n");
        free(p->vertice);
        p->vertice = NULL;
        p->n = 0;
        return FALSE;
    }
    return TRUE;
}

static int segmentsIntersect(const TPoint a1, const TPoint a2, const TPoint b1, const TPoint b2) {
    PTYPE x1 = a2.x - a1.x, y1 = a2.y - a1.y;
    PTYPE x2 = b1.x - a1.x, y2 = b1.y - a1.y;
    PTYPE x3 = b2.x - a1.x, y3 = b2.y - a1.y;
    PTYPE d1 = x1 * y2 - y1 * x2;
    PTYPE d2 = x1 * y3 - y1 * x3;
    x1 = b2.x - b1.x; y1 = b2.y - b1.y;
    x2 = a1.x - b1.x; y2 = a1.y - b1.y;
    x3 = a2.x - b1.x; y3 = a2.y - b1.y;
    PTYPE d3 = x1 * y2 - y1 * x2;
    PTYPE d4 = x1 * y3 - y1 * x3;
    return (d1 * d2 < 0) && (d3 * d4 < 0);
}

int isValidPolygon(const Polygone* p) {
    if (!p) return FALSE;
    if (p->n < 3) return FALSE;
    for (int i = 0; i < p->n; ++i) {
        for (int j = i + 1; j < p->n; ++j) {
            if (isEqual(p->vertice[i].x, p->vertice[j].x) &&
                isEqual(p->vertice[i].y, p->vertice[j].y)) {
                return FALSE;
            }
        }
    }
    for (int i = 0; i < p->n; ++i) {
        TPoint a1 = p->vertice[i];
        TPoint a2 = p->vertice[(i + 1) % p->n];
        for (int j = i + 1; j < p->n; ++j) {
            int nextj = (j + 1) % p->n;
            if (nextj == i) continue;
            if (i == 0 && nextj == 0) continue;
            TPoint b1 = p->vertice[j];
            TPoint b2 = p->vertice[nextj];
            if (segmentsIntersect(a1, a2, b1, b2)) return FALSE;
        }
    }
    return TRUE;
}

// Menory cleanup
int freePolygone(Polygone* p) {
    if (!p) return 0;
    if (p->vertice) {
        free(p->vertice);
        p->vertice = NULL;
    }
    p->n = 0;
    return 0;
}

// Geometry functions
PTYPE area_polygon(Polygone p) {
    if (p.n < 3) return 0.0f;
    PTYPE sum = 0.0f;
    for (int i = 0; i < p.n; ++i) {
        int j = (i + 1) % p.n;
        sum += p.vertice[i].x * p.vertice[j].y - p.vertice[j].x * p.vertice[i].y;
    }
    return fabsf(sum) / 2.0f;
}

PTYPE perimeterPolygone(const Polygone* p) {
    if (!p || p->n < 2) return 0.0f;
    PTYPE per = 0.0f;
    for (int i = 0; i < p->n; ++i) {
        int j = (i + 1) % p->n;
        TVECT v = setVector(p->vertice[i], p->vertice[j]);
        per += lengthVector(v);
    }
    return per;
}

int pointsPolygoneInside(const Polygone* p, TPoint p0) {
    if (!p) return FALSE;
    int inside = 0;
    for (int i = 0, j = p->n - 1; i < p->n; j = i++) {
        PTYPE xi = p->vertice[i].x, yi = p->vertice[i].y;
        PTYPE xj = p->vertice[j].x, yj = p->vertice[j].y;
        int intersect = ((yi > p0.y) != (yj > p0.y)) &&
                        (p0.x < (xj - xi) * (p0.y - yi) / (yj - yi + 1e-12f) + xi);
        if (intersect) inside = !inside;
    }
    return inside ? TRUE : FALSE;
}

int isConvexPolygone(const Polygone* p) {
    if (!p || p->n < 3) return FALSE;
    int sign = 0;
    for (int i = 0; i < p->n; ++i) {
        TPoint a = p->vertice[i];
        TPoint b = p->vertice[(i + 1) % p->n];
        TPoint c = p->vertice[(i + 2) % p->n];
        TVECT v1 = setVector(a, b);
        TVECT v2 = setVector(b, c);
        TVECT cross = vectorMultVector(v1, v2);
        PTYPE z = cross.z;
        if (!isEqual(z, 0.0f)) {
            int s = (z > 0.0f) ? 1 : -1;
            if (sign == 0) sign = s;
            else if (sign != s) return FALSE;
        }
    }
    return TRUE;
}

/* Polygon equality with rotation and reversal */
int isEqualPolygone(const Polygone* p1, const Polygone* p2) {
    if (!p1 || !p2) return FALSE;
    if (p1->n != p2->n) return FALSE;
    int n = p1->n;
    for (int off = 0; off < n; ++off) {
        int ok = 1;
        for (int i = 0; i < n; ++i) {
            int idx = (off + i) % n;
            if (!isEqual(p1->vertice[idx].x, p2->vertice[i].x) ||
                !isEqual(p1->vertice[idx].y, p2->vertice[i].y)) { ok = 0; break; }
        }
        if (ok) return TRUE;
    }
    for (int off = 0; off < n; ++off) {
        int ok = 1;
        for (int i = 0; i < n; ++i) {
            int idx = (off + i) % n;
            int ridx = (n - i - 1) % n;
            if (!isEqual(p1->vertice[idx].x, p2->vertice[ridx].x) ||
                !isEqual(p1->vertice[idx].y, p2->vertice[ridx].y)) { ok = 0; break; }
        }
        if (ok) return TRUE;
    }
    return FALSE;
}

/* File operations: add from source to dest (skip duplicates),
   add single polygon from console to dest (skip duplicates) */
int addPolygonesFromFile(const char* source_filename, const char* dest_filename) {
    if (!source_filename || !dest_filename) return 0;
    FILE* src = fopen(source_filename, "r");
    if (!src) return 0;
    FILE* dst = fopen(dest_filename, "r+");
    if (!dst) {
        dst = fopen(dest_filename, "w+");
        if (!dst) { fclose(src); return 0; }
        fprintf(dst, "0\n");
        fflush(dst);
    }
    unsigned src_count = 0;
    if (fscanf(src, "%u", &src_count) != 1) { fclose(src); fclose(dst); return 0; }
    rewind(dst);
    unsigned dst_count = 0;
    if (fscanf(dst, "%u", &dst_count) != 1) dst_count = 0;
    Polygone* existing = readPolygones(dst);
    if (!existing && dst_count > 0) { fclose(src); fclose(dst); return 0; }
    unsigned added = 0;
    fseek(dst, 0, SEEK_END);
    for (unsigned i = 0; i < src_count; ++i) {
        Polygone poly; poly.n = 0; poly.vertice = NULL;
        if (!readPolygoneFromText(src, &poly)) { continue; }
        if (!isValidPolygon(&poly)) { freePolygone(&poly); continue; }
        bool dup = false;
        for (unsigned j = 0; j < dst_count; ++j) {
            if (isEqualPolygone(&existing[j], &poly)) { dup = true; break; }
        }
        if (!dup) {
            if (writePolygone(dst, &poly)) added++;
        }
        freePolygone(&poly);
    }
    rewind(dst);
    fprintf(dst, "%u\n", dst_count + added);
    if (existing) {
        for (unsigned i = 0; i < dst_count; ++i) freePolygone(&existing[i]);
        free(existing);
    }
    fclose(src);
    fclose(dst);
    return (int)added;
}

int addSinglePolygonFromConsole(const char* dest_filename) {
    if (!dest_filename) return 0;
    Polygone newp; newp.n = 0; newp.vertice = NULL;
    if (!inputPolygone(NULL, &newp)) return 0;
    FILE* dst = fopen(dest_filename, "r+");
    if (!dst) {
        dst = fopen(dest_filename, "w+");
        if (!dst) { freePolygone(&newp); return 0; }
        fprintf(dst, "0\n");
        fflush(dst);
    }
    unsigned dst_count = 0;
    if (fscanf(dst, "%u", &dst_count) != 1) dst_count = 0;
    Polygone* existing = readPolygones(dst);
    if (!existing && dst_count > 0) { fclose(dst); freePolygone(&newp); return 0; }
    for (unsigned i = 0; i < dst_count; ++i) {
        if (isEqualPolygone(&existing[i], &newp)) {
            for (unsigned j = 0; j < dst_count; ++j) freePolygone(&existing[j]);
            free(existing);
            fclose(dst);
            freePolygone(&newp);
            return 0;
        }
    }
    fseek(dst, 0, SEEK_END);
    writePolygone(dst, &newp);
    rewind(dst);
    fprintf(dst, "%u\n", dst_count + 1);
    for (unsigned j = 0; j < dst_count; ++j) freePolygone(&existing[j]);
    free(existing);
    fclose(dst);
    freePolygone(&newp);
    return 1;
}

void outputPolygone(const Polygone* p, FILE* fp) {
    if (!p) return;
    if (!fp) {
        printf("Vertices: %u, Points: ", (unsigned)p->n);
        for (int i = 0; i < p->n; ++i) {
            printf("(%.2f, %.2f) ", p->vertice[i].x, p->vertice[i].y);
        }
        printf("\n");
    } else {
        fprintf(fp, "Vertices: %u, Points: ", (unsigned)p->n);
        for (int i = 0; i < p->n; ++i) {
            fprintf(fp, "(%.2f, %.2f) ", p->vertice[i].x, p->vertice[i].y);
        }
        fprintf(fp, "\n");
    }
}