#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

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
    // ....
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

// text file
int writePolygone(FILE* fp, Polygone* p) {
    assert(fp != 0);
    assert(p != 0);
    //
    
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
    if(p->vertice)free(p->vertice);
    return 0;
}

PTYPE area_polygon(Polygone p) {
 
 
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
        /// Векторний добуток

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

PTYPE distancePoints(TPoint p1, TPoint p2);
PTYPE perimeterPolygone(const Polygone* p);
PTYPE area_polygon(Polygone p);
int isConvexPolygone_fixed(const Polygone* p);

PTYPE distancePoints(TPoint p1, TPoint p2) {
    PTYPE dx = p2.x - p1.x;
    PTYPE dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

PTYPE perimeterPolygone(const Polygone* p) {
    if (p->n < 2) return 0.0;
    PTYPE perimeter = 0.0;
    for (int i = 0; i < p->n - 1; i++) {
        perimeter += distancePoints(p->vertice[i], p->vertice[i + 1]);
    }
    perimeter += distancePoints(p->vertice[p->n - 1], p->vertice[0]);
    return perimeter;
}

PTYPE area_polygon(Polygone p) {
    if (p.n < 3) return 0.0;
    PTYPE area = 0.0;
    for (int i = 0; i < p.n - 1; i++) {
        area += p.vertice[i].x * p.vertice[i + 1].y;
        area -= p.vertice[i + 1].x * p.vertice[i].y;
    }
    area += p.vertice[p.n - 1].x * p.vertice[0].y;
    area -= p.vertice[0].x * p.vertice[p.n - 1].y;
    return fabs(area) / 2.0;
}

PTYPE cross_product_2d(TPoint p1, TPoint p2, TPoint p3) {
    PTYPE v1x = p2.x - p1.x;
    PTYPE v1y = p2.y - p1.y;
    PTYPE v2x = p3.x - p2.x;
    PTYPE v2y = p3.y - p2.y;
    return v1x * v2y - v1y * v2x;
}

int isConvexPolygone_fixed(const Polygone* p) {
    if (p->n < 3) return 0;

    int sign = 0;
    for (int i = 0; i < p->n; i++) {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        TPoint p3 = p->vertice[(i + 2) % p->n];

        PTYPE cross_product_z = cross_product_2d(p1, p2, p3);

        if (fabs(cross_product_z) > __DBL_EPSILON__) {
            int current_sign = (cross_product_z > 0) ? 1 : -1;
            if (sign == 0) {
                sign = current_sign;
            } else if (sign != current_sign) {
                return 0;
            }
        }
    }

    return 1;
}


