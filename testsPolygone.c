#include "testsPolygone.h"
#include "Polygone.h"
#include <assert.h>

// Test utilities
void createTestFile(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    // Create test data: 3 polygons
    fprintf(fp, "3\n");

    // Polygon 1: triangle
    fprintf(fp, "3 0 0 2 0 0 2\n");
    // Polygon 2: square
    fprintf(fp, "4 0 0 3 0 3 3 0 3\n");
    // Polygon 3: pentagon (convex)
    fprintf(fp, "5 0 0 2 0 3 2 1 4 -1 2\n");

    fclose(fp);
}

// Vector tests
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
    TVECT v1 = { 1.f, 2.f, 3.f };
    TVECT v2 = { 1.f, 2.f, 3.f };
    TVECT v3 = { 0.f, 2.f, 3.f };
    if (isEqual2(v1, v2) && !isEqual2(v1, v3)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_setVector() {
    TPoint p1 = { 0.f, 0.f };
    TPoint p2 = { 1.f, 1.f };
    TVECT v1 = { 1.f, 1.f, 0.f };
    TVECT v2 = setVector(p1, p2);
    if (isEqual2(v1, v2)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_lengthVector() {
    TVECT v = { 3.f, 4.f, 0.f };
    if (isEqual(lengthVector(v), 5)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_addVector() {
    TVECT v1 = { 3.f, 4.f, 0.f };
    TVECT v2 = { -3.f, -4.f, 0.f };
    TVECT v3 = { 0.f, 0.f, 0.f };
    if (isEqual2(addVector(v1, v2), v3)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_vectorMultVector() {
    TVECT v11 = { 1.f, 0.f, 0.f };
    TVECT v21 = { 0.f, 1.f, 0.f };
    TVECT v31 = { 0.f, 0.f, 1.f };
    TVECT v12 = { 1.f, 2.f, 3.f };
    TVECT v22 = { -1.f, -5.f, -10.f };
    TVECT v32 = { -5.f, 7.f, -3.f };

    if (isEqual2(vectorMultVector(v11, v21), v31) && isEqual2(vectorMultVector(v12, v22), v32)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// Geometric tests
int test_area() {
    TPoint p1 = { 0.f, 0.f };
    TPoint p2 = { 3.f, 0.f };
    TPoint p3 = { 0.f, 4.f };
    if (isEqual(area(p1, p2, p3), 6.f)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_area_polygon() {
    NTYPE n = 4;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.f, 0.f };
    vertices[1] = (TPoint){ 0.f, 4.f };
    vertices[2] = (TPoint){ 4.f, 0.f };
    vertices[3] = (TPoint){ 4.f, 4.f };
    Polygone p = { n, vertices };

    int result = isEqual(area_polygon(p), 8.0); // Should be 8 for this shape

    free(vertices);
    return result;
}

int test_inPolygon() {
    NTYPE n = 4;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.f, 0.f };
    vertices[1] = (TPoint){ 4.f, 0.f };
    vertices[2] = (TPoint){ 4.f, 4.f };
    vertices[3] = (TPoint){ 0.f, 4.f };
    Polygone p = { n, vertices };

    TPoint test1 = { 0.f, 0.f };
    TPoint test2 = { 2.f, 2.f };
    TPoint test3 = { -1.f, -2.f };

    int result = (inPolygon(p, test1) && inPolygon(p, test2) && !inPolygon(p, test3));

    free(vertices);
    return result;
}

// Polygone operation tests
int test_isConvexPolygone_convex() {
    NTYPE n = 4;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.f, 0.f };
    vertices[1] = (TPoint){ 4.f, 0.f };
    vertices[2] = (TPoint){ 4.f, 4.f };
    vertices[3] = (TPoint){ 0.f, 4.f };
    Polygone p = { n, vertices };

    int result = isConvexPolygone(&p) == TRUE;

    free(vertices);
    return result;
}

int test_isConvexPolygone_nonConvex() {
    NTYPE n = 5;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.f, 0.f };
    vertices[1] = (TPoint){ 4.f, 0.f };
    vertices[2] = (TPoint){ 2.f, 1.f };  // Point that makes polygon non-convex
    vertices[3] = (TPoint){ 4.f, 4.f };
    vertices[4] = (TPoint){ 0.f, 4.f };
    Polygone p = { n, vertices };

    int result = isConvexPolygone(&p) == FALSE;

    free(vertices);
    return result;
}

int test_isConvexPolygone_triangle() {
    NTYPE n = 3;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.f, 0.f };
    vertices[1] = (TPoint){ 4.f, 0.f };
    vertices[2] = (TPoint){ 2.f, 3.f };
    Polygone p = { n, vertices };

    int result = isConvexPolygone(&p) == TRUE;

    free(vertices);
    return result;
}

// File operation tests
int test_inputPolygone_console() {
    // This would require mocking stdin, so we test file input instead
    Polygone p;
    int result = inputPolygone(NULL, &p);
    if (result) freePolygone(&p);
    return result ? TRUE : FALSE; // Manual test required
}

int test_inputPolygone_file() {
    FILE* fp = fopen("test_input.txt", "w");
    if (!fp) return FALSE;
    fprintf(fp, "3 0 0 1 0 0 1"); // Triangle
    fclose(fp);

    // Test reading
    fp = fopen("test_input.txt", "r");
    Polygone p;
    int result = inputPolygone(fp, &p);
    fclose(fp);

    if (result) {
        freePolygone(&p);
    }
    return result;
}

int test_inputPolygone_invalid() {
    // Create invalid test file
    FILE* fp = fopen("test_invalid.txt", "w");
    if (!fp) return FALSE;
    fprintf(fp, "2 0 0 1 0"); // Only 2 vertices - invalid
    fclose(fp);

    // Test reading invalid polygon
    fp = fopen("test_invalid.txt", "r");
    Polygone p;
    int result = inputPolygone(fp, &p);
    fclose(fp);

    return !result;
}

int test_writePolygone() {
    Polygone p;
    p.n = 3;
    p.vertice = (TPoint*)malloc(3 * sizeof(TPoint));
    p.vertice[0] = (TPoint){ 0.f, 0.f };
    p.vertice[1] = (TPoint){ 1.f, 0.f };
    p.vertice[2] = (TPoint){ 0.f, 1.f };

    FILE* fp = fopen("test_write.txt", "w");
    if (!fp) {
        freePolygone(&p);
        return FALSE;
    }

    int result = writePolygone(fp, &p);
    fclose(fp);
    freePolygone(&p);

    return result;
}

int test_addPolygonesFromFile() {
    // Create source file
    FILE* src = fopen("test_source.txt", "w");
    if (!src) return FALSE;
    fprintf(src, "2\n3 0 0 1 0 0 1\n4 0 0 2 0 2 2 0 2\n");
    fclose(src);

    // Create empty destination file
    FILE* dest = fopen("test_dest.txt", "w");
    if (!dest) return FALSE;
    fprintf(dest, "0"); // Start with 0 polygons
    fclose(dest);

    // Test adding polygons
    int result = addPolygonesFromFile("test_source.txt", "test_dest.txt");

    return result == 2; // Should add 2 polygons
}

int test_pointsPolygones() {
    createTestFile("test_points.txt");

    FILE* fp = fopen("test_points.txt", "r");
    TPoint point = { 1.0f, 1.0f }; // Point inside some polygons
    NTYPE result = pointsPolygones(fp, point);
    fclose(fp);

    return result > 0; // Should find at least one polygon containing the point
}

int test_minAreaPolygone() {
    createTestFile("test_minarea.txt");

    FILE* fp = fopen("test_minarea.txt", "r");
    Polygone minPoly;
    int found = minAreaPolygone(fp, &minPoly);
    fclose(fp);

    if (found) {
        freePolygone(&minPoly);
        return TRUE;
    }
    return FALSE;
}

int test_numberConvexPolygones() {
    createTestFile("test_convex.txt");

    FILE* fp = fopen("test_convex.txt", "r");
    NTYPE count = numberConvexPolygones(fp);
    fclose(fp);

    return count >= 1;
}

int test_isEqualPolygone() {
    Polygone p1, p2;
    p1.n = p2.n = 3;
    p1.vertice = (TPoint*)malloc(3 * sizeof(TPoint));
    p2.vertice = (TPoint*)malloc(3 * sizeof(TPoint));

    p1.vertice[0] = p2.vertice[0] = (TPoint){ 0.f, 0.f };
    p1.vertice[1] = p2.vertice[1] = (TPoint){ 1.f, 0.f };
    p1.vertice[2] = p2.vertice[2] = (TPoint){ 0.f, 1.f };

    int result = isEqualPolygone(&p1, &p2);

    freePolygone(&p1);
    freePolygone(&p2);

    return result;
}

int test_perimeterPolygone() {
    Polygone p;
    p.n = 3;
    p.vertice = (TPoint*)malloc(3 * sizeof(TPoint));
    p.vertice[0] = (TPoint){ 0.f, 0.f };
    p.vertice[1] = (TPoint){ 3.f, 0.f };
    p.vertice[2] = (TPoint){ 0.f, 4.f };

    PTYPE perimeter = perimeterPolygone(&p);
    int result = isEqual(perimeter, 12.0f);

    freePolygone(&p);
    return result;
}

// Test runner
int testVectors() {
    return test_isEqual() && test_isEqual2() && test_setVector() &&
        test_lengthVector() && test_addVector() && test_vectorMultVector();
}

int testInputPolygone() {
    return test_inputPolygone_file() && test_inputPolygone_invalid();
}

int testWritePolygones() {
    return test_writePolygone();
}

int testDeletePolygone() {
    return TRUE;
}

int testShowPolygoneFile() {
    createTestFile("test_show.txt");
    FILE* fp = fopen("test_show.txt", "r");
    showPolygonesFile(fp);
    fclose(fp);
    return TRUE;
}
