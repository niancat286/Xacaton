#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "Polygone.h"
#include "file_forming.h"

#define LEN 30

int inputPolygone(FILE* fp, Polygone* p){

    NTYPE n;
    
    if(fp)
        fscanf(fp,"%u",&n);    
    else{
        printf("N=");
        fscanf(stdin," %u",&n);
    }

    if(n<=2) return FALSE;
    
    p->n = n;
    p->vertice = (TPoint*) malloc(n * 2 * sizeof(TPoint));
    int scan_res = 0;
    return TRUE;
}

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

int writePolygone(FILE* fp, Polygone* p) {
    assert(fp != 0);
    assert(p != 0);
    return 0;
}

void showPolygonesFile(FILE* fp) {
    Polygone* polygones = readPolygones(fp);
    int i = 0;
    while(1) {
        if (polygones[i].n == 0) {
            break;
        }
        outputPolygon(polygones[i]); 
        i++;
    }
}

PTYPE area(TPoint p1, TPoint p2, TPoint p3) {
    TVECT v1 = setVector(p2, p1);
    TVECT v2 = setVector(p2, p3);
    PTYPE par_area = lengthVector(vectorMultVector(v1, v2));
    return par_area / 2.0;
}

int freePolygone(Polygone* p){
    if(p->vertice) free(p->vertice);
    return 0;
}

PTYPE area_polygon(Polygone p) {
    double sum = 0.0;
    if (p.n < 3) return 0.0;
    for (int i = 0; i < p.n; ++i) {
        int j = (i + 1) % p.n;
        sum += (double)p.vertice[i].x * p.vertice[j].y - (double)p.vertice[j].x * p.vertice[i].y;
    }
    return (PTYPE)(0.5 * fabs(sum));
}

NTYPE inPolygon(Polygone p, TPoint point) {
    NTYPE power = p.n;
    PTYPE res = 0;
    for (int i = 0; i < power - 1; i++) {
        res += area(p.vertice[i], point, p.vertice[i+1]);
    }
    res += area(p.vertice[0], point, p.vertice[power-1]);
    if (isEqual(area_polygon(p), res)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

NTYPE pointsPolygones(FILE* fp, TPoint point) {
    assert(fp != 0);
    int i = 0, res = 0, n = 0;
    unsigned int M;
    fread(&M, sizeof(unsigned int), 1, fp);
    for (int i = 0; i < M; i++) {
        Polygone p;
        fread(&p.n, sizeof(unsigned int), 1, fp);
        p.vertice = (TPoint*) malloc(p.n*sizeof(TPoint));
        for (int j = 0; j < p.n; j++) {
            fread(&p.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&p.vertice[j].y, sizeof(PTYPE), 1, fp);
        }
        if (inPolygon(p, point)) {
            res++;
        }
        free(p.vertice);
    }
    return res;
}

int minAreaPolygone(FILE* fp, Polygone* p) {
    assert(fp != NULL);
    Polygone temp;
    PTYPE min_area = 0;
    int found = FALSE;
    unsigned int M;
    fread(&M, sizeof(unsigned int), 1, fp);
    for (int i = 0; i < M; i++) {
        fread(&temp.n, sizeof(unsigned int), 1, fp);
        temp.vertice = (TPoint*)malloc(temp.n * sizeof(TPoint));
        for (int j = 0; j < temp.n; j++) {
            fread(&temp.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&temp.vertice[j].y, sizeof(PTYPE), 1, fp);
        }
        PTYPE area = area_polygon(temp);
        if (!found || area < min_area) {
            min_area = area;
            found = TRUE;
            if (p->vertice) {
                free(p->vertice);
            }
            p->n = temp.n;
            p->vertice = (TPoint*)malloc(temp.n * sizeof(TPoint));
            for (int j = 0; j < temp.n; j++){
                p->vertice[j].x = temp.vertice[j].x;
                p->vertice[j].y = temp.vertice[j].y;
            }
        }
        free(temp.vertice);
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
        PTYPE cross_product_z = vectorMultVector(v1, v2).z;
        if (cross_product_z != 0) {
            int current_sign = (cross_product_z > 0) ? 1 : -1;
            if (sign == 0){
                sign = current_sign;
            }else if (sign != current_sign){
                return FALSE;
            }
        }
    }
    return TRUE;
}

NTYPE numberConvexPolygones(FILE* fp) {
    assert(fp != NULL);
    unsigned int M;
    fread(&M, sizeof(unsigned int), 1, fp);
    int count = 0;
    for (int i = 0; i < M; i++) {
        Polygone p;
        fread(&p.n, sizeof(unsigned int), 1, fp);
        p.vertice = (TPoint*)malloc(p.n * sizeof(TPoint));
        for (int j = 0; j < p.n; j++) {
            fread(&p.vertice[j].x, sizeof(PTYPE), 1, fp);
            fread(&p.vertice[j].y, sizeof(PTYPE), 1, fp);
        }
        if (isConvexPolygone(&p)) {
            count++;
        }
        free(p.vertice);
    }
    return count;
}

#ifdef RUN_POLYGONE_SELFTEST

// #include <stdio.h>
// #include <string.h>

// static int float_eq(float a, float b) {
//     float d = a - b;
//     if (d < 0) d = -d;
//     return d < 1e-5f;
// }

// static void test_area() {
//     TPoint p1 = {0.f, 0.f};
//     TPoint p2 = {3.f, 0.f};
//     TPoint p3 = {0.f, 4.f};
//     float a = area(p1, p2, p3);
//     assert(float_eq(a, 6.0f));
// }

// static void test_write_and_read_binary() {
//     Polygone p;
//     p.n = 3;
//     p.vertice = (TPoint*) malloc(sizeof(TPoint) * 3);
//     p.vertice[0].x = 0.0f; p.vertice[0].y = 0.0f;
//     p.vertice[1].x = 1.0f; p.vertice[1].y = 0.0f;
//     p.vertice[2].x = 0.0f; p.vertice[2].y = 1.0f;

//     const char *fname = "test_bin_polys.bin";
//     FILE *f = fopen(fname, "wb");
//     assert(f != NULL);
//     unsigned int M = 1;
//     fwrite(&M, sizeof(unsigned int), 1, f);
//     int wr = writePolygone_binary(f, &p);
//     assert(wr == 1);
//     fclose(f);

//     FILE *fr = fopen(fname, "rb");
//     assert(fr != NULL);
//     unsigned int Mr;
//     fread(&Mr, sizeof(unsigned int), 1, fr);
//     assert(Mr == 1);
//     unsigned int nr;
//     fread(&nr, sizeof(unsigned int), 1, fr);
//     assert(nr == 3);
//     TPoint *verts = (TPoint*) malloc(sizeof(TPoint) * nr);
//     for (unsigned int i = 0; i < nr; ++i) {
//         fread(&verts[i].x, sizeof(PTYPE), 1, fr);
//         fread(&verts[i].y, sizeof(PTYPE), 1, fr);
//     }
//     fclose(fr);

//     assert(float_eq(verts[0].x, p.vertice[0].x));
//     assert(float_eq(verts[0].y, p.vertice[0].y));
//     assert(float_eq(verts[1].x, p.vertice[1].x));
//     assert(float_eq(verts[1].y, p.vertice[1].y));
//     assert(float_eq(verts[2].x, p.vertice[2].x));
//     assert(float_eq(verts[2].y, p.vertice[2].y));

//     free(verts);
//     free(p.vertice);
//     remove(fname);
// }

// static void test_area_polygon() {
//     Polygone p;
//     p.n = 4;
//     p.vertice = (TPoint*) malloc(sizeof(TPoint) * 4);
//     p.vertice[0].x = 0.f; p.vertice[0].y = 0.f;
//     p.vertice[1].x = 0.f; p.vertice[1].y = 4.f;
//     p.vertice[2].x = 4.f; p.vertice[2].y = 4.f;
//     p.vertice[3].x = 4.f; p.vertice[3].y = 0.f;
//     PTYPE ar = area_polygon(p);
//     assert(float_eq((float)ar, 16.0f));
//     free(p.vertice);
// }

// int main(void) {
//     test_area();
//     test_write_and_read_binary();
//     test_area_polygon();
//     printf("Self-tests passed.\n");
//     return 0;
// }

#endif
