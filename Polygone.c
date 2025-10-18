#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "Polygone.h"
#include "file_forming.h"

#define LEN 30
/*
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
*/
PTYPE area_polygon(Polygone p) {
    if (p.n < 3) return 0;
    PTYPE area = 0;
    for (int i = 0; i < p.n; i++) {
        int j = (i + 1) % p.n;
        area += p.vertice[i].x * p.vertice[j].y;
        area -= p.vertice[j].x * p.vertice[i].y;
    }
    
    return fabs(area) / 2.0;
}
/*
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
*/
int minAreaPolygone(FILE* fp, Polygone* p) {
    assert(fp != NULL);
    assert(p != NULL);
    p->vertice = NULL;
    p->n = 0;
    Polygone temp;
    PTYPE min_area = 0;
    int found = FALSE;
    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1) {
        return FALSE;
    }
    for (int i = 0; i < M; i++) {
        if (fread(&temp.n, sizeof(unsigned int), 1, fp) != 1) {
            if (p->vertice) free(p->vertice);
            return FALSE;
        }
        temp.vertice = (TPoint*)malloc(temp.n * sizeof(TPoint));
        if (temp.vertice == NULL) {
            if (p->vertice) free(p->vertice);
            return FALSE;
        }
        int read_error = FALSE;
        for (int j = 0; j < temp.n; j++) {
            if (fread(&temp.vertice[j].x, sizeof(PTYPE), 1, fp) != 1 ||
                fread(&temp.vertice[j].y, sizeof(PTYPE), 1, fp) != 1) {
                read_error = TRUE;
                break;
            }
        }
        if (read_error) {
            free(temp.vertice);
            if (p->vertice) free(p->vertice);
            return FALSE;
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
            if (p->vertice == NULL) {
                free(temp.vertice);
                return FALSE;
            }
            for (int j = 0; j < temp.n; j++) {
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
        /// Cross product
        TVECT cross_product = vectorMultVector(v1, v2);
        PTYPE cross_product_z = cross_product.z;

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

/*

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
*/
/*
int isEqualPoint(struct TPoint a, struct TPoint b)
{
    return fabs(a.x - b.x) < 1e-6 && fabs(a.y - b.y) < 1e-6;
}

int isEqualPolygone(const struct Polygone *p1, const struct Polygone *p2)
{
    if (p1->n != p2->n)
        return FALSE; // тоді у нас різна кількість вершин

    int n = p1->n; // p1, not p
    int start = -1;

    for (int i = 0; i < n; i++)
    {
        if (isEqualPoint(p1->vertice[0], p2->vertice[i]))
        {
            start = i;
            break;
        }
    }
    if (start == -1)
        return FALSE;

    int same_dir = TRUE;
    int opposite_dir = TRUE;
    for (int i = 0; i < n; i++)
    {
        int j = (start + i) % n;
        int k = (start - i + n) % n;
        if (!isEqualPoint(p1->vertice[i], p2->vertice[j]))
            same_dir = FALSE;
        if (!isEqualPoint(p1->vertice[i], p2->vertice[k]))
            opposite_dir = FALSE;
    }
    return (same_dir || opposite_dir);
}

int isPresentPolygone(FILE *fp, const struct Polygone *p)
{
    assert(fp != NULL);
    rewind(fp);

    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1)
        return FALSE;

    for (unsigned int i = 0; i < M; i++)
    {
        struct Polygone temp;
        if (fread(&temp.n, sizeof(unsigned int), 1, fp) != 1)
        {
            return FALSE; // break -> FALSE
        }

        temp.vertice = (TPoint *)malloc(temp.n * sizeof(TPoint));
        if (temp.vertice == NULL)
            return FALSE;

        for (unsigned int j = 0; j < temp.n; j++)
        {
            // add validation
            if (fread(&temp.vertice[j].x, sizeof(PTYPE), 1, fp) != 1 ||
                fread(&temp.vertice[j].y, sizeof(PTYPE), 1, fp) != 1)
            {
                free(temp.vertice);
                return FALSE;
            }
        }

        int equal = isEqualPolygone(p, &temp);
        free(temp.vertice);

        if (equal)
            return TRUE;
    }

    return FALSE; // to func
}
*/

PTYPE perimeter_polygon(Polygone p) {
    if (p.n < 2) return 0;  
    PTYPE perim = 0;
    for (int i = 0; i < p.n - 1; i++) {
        TVECT v = setVector(p.vertice[i], p.vertice[i + 1]);
        perim += lengthVector(v);
    }
    TVECT v_last = setVector(p.vertice[p.n - 1], p.vertice[0]);
    perim += lengthVector(v_last);
    return perim;
}

int maxPerimeterPolygone(FILE* fp, Polygone* p) {
    assert(fp != NULL);
    assert(p != NULL);
    p->vertice = NULL;
    p->n = 0;
    Polygone temp;
    PTYPE max_perim = 0;
    int found = FALSE;
    unsigned int M;
    if (fread(&M, sizeof(unsigned int), 1, fp) != 1) {
        return FALSE;
    }
    for (int i = 0; i < M; i++) {
        if (fread(&temp.n, sizeof(unsigned int), 1, fp) != 1) {
            if (p->vertice) free(p->vertice);
            return FALSE;
        }
        temp.vertice = (TPoint*)malloc(temp.n * sizeof(TPoint));
        if (temp.vertice == NULL) {
            if (p->vertice) free(p->vertice);
            return FALSE;
        }
        int read_error = FALSE;
        for (int j = 0; j < temp.n; j++) {
            if (fread(&temp.vertice[j].x, sizeof(PTYPE), 1, fp) != 1 ||
                fread(&temp.vertice[j].y, sizeof(PTYPE), 1, fp) != 1) {
                read_error = TRUE;
                break;
            }
        }
        if (read_error) {
            free(temp.vertice);
            if (p->vertice) free(p->vertice);
            return FALSE;
        }
        PTYPE perim = perimeter_polygon(temp);
        if (!found || perim > max_perim) {
            max_perim = perim;
            found = TRUE;
            if (p->vertice) {
                free(p->vertice);
            }
            p->n = temp.n;
            p->vertice = (TPoint*)malloc(temp.n * sizeof(TPoint));
            if (p->vertice == NULL) {
                free(temp.vertice);
                return FALSE;
            }
            for (int j = 0; j < temp.n; j++) {
                p->vertice[j].x = temp.vertice[j].x;
                p->vertice[j].y = temp.vertice[j].y;
            }
        }
        free(temp.vertice);
    }
    return found;
}
