#include "testsPolygone.h"
#include "Polygone.h"
#include <assert.h>

int test_isEqual() {
    PTYPE a = 5.f, b = 7.f, c = 5.f;
    if (!isEqual(a, b) && isEqual(a, c)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_isEqual2() {
    TVECT v1 = {1.f, 2.f, 3.f};
    TVECT v2 = {1.f, 2.f, 3.f};
    TVECT v3 = {0.f, 2.f, 3.f};
    if (isEqual2(v1, v2) && !isEqual2(v1, v3)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_setVector() {
    TPoint p1 = {0.f, 0.f};
    TPoint p2 = {1.f, 1.f};
    TVECT v1 = {1.f, 1.f, 0.f};
    TVECT v2 = setVector(p1, p2);
    if (isEqual2(v1, v2)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_lengthVector() {
    TVECT v = {3.f, 4.f, 0.f};
    if (isEqual(lengthVector(v), 5)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_addVector() {
    TVECT v1 = {3.f, 4.f, 0.f};
    TVECT v2 = {-3.f, -4.f, 0.f};
    TVECT v3 = {0.f, 0.f, 0.f};
    if (isEqual2(addVector(v1, v2), v3)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_multVector() {
    TVECT v1 = {3.f, 4.f, 0.f};
    TVECT v2 = {0.f, 0.f, 0.f};
    PTYPE a = 0;
    if (isEqual2(multVector(a, v1), v2)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_subVector() {
    TVECT v1 = {3.f, 4.f, 0.f};
    TVECT v2 = {3.f, 4.f, 0.f};
    TVECT v3 = {0.f, 0.f, 0.f};
    if (isEqual2(subVector(v1, v2), v3)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_scalarMultVector() {
    TVECT v1 = {0.f, 0.f, 0.f};
    TVECT v2 = {3.f, 4.f, 0.f};
    if (isEqual(lengthVector(v1), 0) && isEqual(lengthVector(v2), 5)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_vectorMultVector() {
    TVECT v11 = {1.f, 0.f, 0.f};
    TVECT v21 = {0.f, 1.f, 0.f};
    TVECT v31 = {0.f, 0.f, 1.f};
    TVECT v12 = {1.f, 2.f, 3.f};
    TVECT v22 = {-1.f, -5.f, -10.f};
    TVECT v32 = {-5.f, 7.f, -3.f};

    if (isEqual2(vectorMultVector(v11, v21), v31) && isEqual2(vectorMultVector(v12, v22), v32)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_area() {
    TPoint p1 = {0.f, 0.f};
    TPoint p2 = {3.f, 0.f};
    TPoint p3 = {0.f, 4.f};
    if (isEqual(area(p1, p2, p3), 6.f)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_area_polygon() {
    NTYPE n = 4;
    TPoint p0 = {0.f, 0.f};
    TPoint p1 = {0.f, 4.f};
    TPoint p2 = {4.f, 0.f};
    TPoint p3 = {4.f, 4.f};
    TPoint vertices[] = {p0, p1, p2, p3};
    Polygone p = {n, vertices};
    if(isEqual(area_polygon(p), 16)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_inPolygon() {
    NTYPE n = 4;
    TPoint p0 = {0.f, 0.f};
    TPoint p1 = {4.f, 0.f};
    TPoint p2 = {4.f, 4.f};
    TPoint p3 = {0.f, 4.f};
    TPoint vertices[] = {p0, p1, p2, p3};
    Polygone p = {n, vertices};
    TPoint test1 = {0.f, 0.f};
    TPoint test2 = {2.f, 2.f};
    TPoint test3 = {-1.f, -2.f};
    if (inPolygon(p, test1) && inPolygon(p, test2) && !inPolygon(p, test3)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

NTYPE test_pointsPolygones() {
    FILE* fp1 = fopen("polygones.txt", "r");

    TPoint check1 = {5.f, 7.f};
    NTYPE expected1 = 0;
    NTYPE got1 = pointsPolygones(fp1, check1); 
    fclose(fp1);

    FILE* fp2 = fopen("polygones.txt", "r");    
    TPoint check2 = {3.01, 3.01};
    NTYPE expected2 = 2;
    NTYPE got2 = pointsPolygones(fp2, check2);
    fclose(fp2);

    FILE* fp3 = fopen("polygones.txt", "r");
    TPoint check3 = {-2.5, 8.f};
    NTYPE expected3 = 1;
    NTYPE got3 = pointsPolygones(fp3, check3);
    fclose(fp3);

    return expected1 == got1 && expected2 == got2 &&  expected3 == got3;
}

int test_isConvexPolygone_convex(){
    NTYPE n = 4;
    TPoint p0 = {0.f, 0.f};
    TPoint p1 = {4.f, 0.f};
    TPoint p2 = {4.f, 4.f};
    TPoint p3 = {0.f, 4.f};
    TPoint vertices[] = {p0, p1, p2, p3};
    Polygone p = {n, vertices};
    return isConvexPolygone(&p) == TRUE;
}

int test_isConvexPolygone_nonConvex(){
    NTYPE n = 5;
    TPoint p0 = {0.f, 0.f};
    TPoint p1 = {4.f, 0.f};
    TPoint p2 = {2.f, 1.f};  // Точка, що робить многокутник неопуклим
    TPoint p3 = {4.f, 4.f};
    TPoint p4 = {0.f, 4.f};
    TPoint vertices[] = {p0, p1, p2, p3, p4};
    Polygone p = {n, vertices};
    return isConvexPolygone(&p) == FALSE;
}

int test_isConvexPolygone_triangle(){
    NTYPE n = 3;
    TPoint p0 = {0.f, 0.f};
    TPoint p1 = {4.f, 0.f};
    TPoint p2 = {2.f, 3.f};
    TPoint vertices[] = {p0, p1, p2};
    Polygone p = {n, vertices};
    return isConvexPolygone(&p) == TRUE;
}

int test_isConvexPolygone_invalid(){
    NTYPE n = 2;
    TPoint p0 = {0.f, 0.f};
    TPoint p1 = {4.f, 0.f};
    TPoint vertices[] = {p0, p1};
    Polygone p = {n, vertices};
    return isConvexPolygone(&p) == FALSE;  // Менше ніж 3 вершини
}

int test_minAreaPolygone() { 
    const char* filename = "file_test_minAreaPolygone";
    FILE* fp = fopen(filename, "rb");
    if(fp != NULL) return FALSE;
    Polygone minPolygone = {0, NULL};
    int found = minAreaPolygone(fp, &minPolygone);
    fclose(fp);
    if (found && minPolygone.n == 3 &&
        minPolygone.vertice[0].x == 0 && minPolygone.vertice[0].y == 0 &&
        minPolygone.vertice[1].x == 2 && minPolygone.vertice[1].y == 0 &&
        minPolygone.vertice[2].x == 0 && minPolygone.vertice[2].y == 2) {
        free(minPolygone.vertice);
        return TRUE;
    } else {
        if (minPolygone.vertice) {
            free(minPolygone.vertice);
        }
        return FALSE;
    }
}

int test_numberConvexPolygones() {
    FILE* fp = fopen("file_test_for_numberConvexPolygones", "rb");
    if(fp != NULL) return FALSE;
    int convexCount = numberConvexPolygones(fp);
    fclose(fp);
    return convexCount == 3 ? TRUE : FALSE;
}

int testShowPolygoneFile() {
    FILE* fp = fopen("polygones.txt", "r");
    showPolygonesFile(fp);
    fclose(fp);
    return 1;
}

int testInputPolygone(){
    // Test 1 : cOnsole input
    Polygone p0;
    int r1 = inputPolygone(NULL, &p0);
    if(r1!= TRUE) return FALSE;
    freePolygone(&p0);
    printf("first passed");

    // Test 2 : file input
    FILE* fp = fopen("1.txt","rt");
    Polygone p;
    int r2 = inputPolygone(fp, &p);
    if(r2!= TRUE) return FALSE;

    printf("second passed");
    Polygone p2;
    p2.n = 3;
    p2.vertice = (TPoint*) calloc(6,sizeof(TPoint));
    p2.vertice[0].x = 0;
    p2.vertice[0].y = 0;

    p2.vertice[1].x = 0;
    p2.vertice[1].y = 1;
    
    p2.vertice[2].x = 1;
    p2.vertice[2].y = 0;

    //if(!isEqualPolygone(&p,&p2)) return FALSE; 

    freePolygone(&p);
    freePolygone(&p2);

    // Test 3 : file input - bad file
    fp = fopen("xxx.txt","rt");
    Polygone p3;
    int r4 = inputPolygone(fp, &p3);
    if(r4!= FALSE) return FALSE;

    freePolygone(&p3);
    printf("Input tests passed");

    return TRUE;
}
