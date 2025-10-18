#include "testsPolygone.h"
#include "Polygone.h"
#include <assert.h>
#include <stdio.h>

// Test utilities
void createTestFile(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    // Create test data: 3 polygons
    fprintf(fp, "3\n");
    // Polygon 1: triangle
    fprintf(fp, "3 0.0 0.0 2.0 0.0 0.0 2.0\n");
    // Polygon 2: square
    fprintf(fp, "4 0.0 0.0 3.0 0.0 3.0 3.0 0.0 3.0\n");
    // Polygon 3: pentagon 
    fprintf(fp, "5 0.0 0.0 2.0 0.0 3.0 2.0 1.0 4.0 -1.0 2.0\n");

    fclose(fp);
}

void createTestFileWithOne(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    fprintf(fp, "1\n");
    fprintf(fp, "3 0.0 0.0 1.0 0.0 0.0 1.0\n");  // Simple triangle

    fclose(fp);
}

void createTestFileWithInvalid(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    fprintf(fp, "1\n");
    fprintf(fp, "2 0.0 0.0 1.0 0.0\n");  // Invalid: n=2

    fclose(fp);
}

// Vector tests
int test_isEqual() {
    PTYPE a = 5.0f, b = 7.0f, c = 5.0f;
    if (!isEqual(a, b) && isEqual(a, c)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_isEqual2() {
    TVECT v1 = { 1.0f, 2.0f, 0.0f };
    TVECT v2 = { 1.0f, 2.0f, 0.0f };
    TVECT v3 = { 0.0f, 2.0f, 0.0f };
    if (isEqual2(v1, v2) && !isEqual2(v1, v3)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_setVector() {
    TPoint p1 = { 0.0f, 0.0f };
    TPoint p2 = { 1.0f, 1.0f };
    TVECT v1 = { 1.0f, 1.0f, 0.0f };
    TVECT v2 = setVector(p1, p2);
    if (isEqual2(v1, v2)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_lengthVector() {
    TVECT v = { 3.0f, 4.0f, 0.0f };
    if (isEqual(lengthVector(v), 5.0f)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_addVector() {
    TVECT v1 = { 3.0f, 4.0f, 0.0f };
    TVECT v2 = { -3.0f, -4.0f, 0.0f };
    TVECT v3 = { 0.0f, 0.0f, 0.0f };
    if (isEqual2(addVector(v1, v2), v3)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_vectorMultVector() {
    TVECT v11 = { 1.0f, 0.0f, 0.0f };
    TVECT v21 = { 0.0f, 1.0f, 0.0f };
    TVECT v31 = { 0.0f, 0.0f, 1.0f };
    TVECT v12 = { 1.0f, 2.0f, 3.0f };
    TVECT v22 = { -1.0f, -5.0f, -10.0f };
    TVECT v32 = { -5.0f, 7.0f, -3.0f };

    if (isEqual2(vectorMultVector(v11, v21), v31) && isEqual2(vectorMultVector(v12, v22), v32)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// Geometric tests
int test_area() {
    NTYPE n = 3;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.0f, 0.0f };
    vertices[1] = (TPoint){ 3.0f, 0.0f };
    vertices[2] = (TPoint){ 0.0f, 4.0f };
    Polygone p = { n, vertices };
    PTYPE a = area_polygon(p);
    free(vertices);
    return isEqual(a, 6.0f);
}

int test_area_polygon() {
    NTYPE n = 4;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.0f, 0.0f };
    vertices[1] = (TPoint){ 0.0f, 4.0f };
    vertices[2] = (TPoint){ 4.0f, 0.0f };
    vertices[3] = (TPoint){ 4.0f, 4.0f };
    Polygone p = { n, vertices };
    PTYPE area = area_polygon(p);
    free(vertices);
    if (isEqual(area, 16.0f)) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int test_inPolygon() {
    // Create dynamic vertices (must be freed)
    NTYPE n = 4;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.0f, 0.0f };
    vertices[1] = (TPoint){ 2.0f, 0.0f };
    vertices[2] = (TPoint){ 2.0f, 2.0f };
    vertices[3] = (TPoint){ 0.0f, 2.0f };
    Polygone p = { n, vertices };

    TPoint point = { 1.0f, 1.0f };
    int inside = pointsPolygoneInside(&p, point);

    freePolygone(&p); // frees vertices
    return inside;
}

int test_isConvexPolygone_convex() {
    NTYPE n = 4;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.0f, 0.0f };
    vertices[1] = (TPoint){ 2.0f, 0.0f };
    vertices[2] = (TPoint){ 2.0f, 2.0f };
    vertices[3] = (TPoint){ 0.0f, 2.0f };
    Polygone p = { n, vertices };

    int convex = isConvexPolygone(&p);
    freePolygone(&p);
    return convex;
}

int test_isConvexPolygone_nonConvex() {
    NTYPE n = 4;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.0f, 0.0f };
    vertices[1] = (TPoint){ 2.0f, 0.0f };
    vertices[2] = (TPoint){ 1.0f, 1.0f };
    vertices[3] = (TPoint){ 0.0f, 2.0f };
    Polygone p = { n, vertices };

    int convex = isConvexPolygone(&p);
    freePolygone(&p);
    return !convex;
}

int test_isConvexPolygone_triangle() {
    NTYPE n = 3;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.0f, 0.0f };
    vertices[1] = (TPoint){ 2.0f, 0.0f };
    vertices[2] = (TPoint){ 1.0f, 2.0f };
    Polygone p = { n, vertices };

    int convex = isConvexPolygone(&p);
    freePolygone(&p);
    return convex;
}

// File operation tests
int test_inputPolygone_file() {
    FILE* fp = fopen("test_input.txt", "w");
    fprintf(fp, "3 0.0 0.0 1.0 0.0 0.0 1.0\n");
    fclose(fp);

    Polygone p;
    fp = fopen("test_input.txt", "r");
    int result = inputPolygone(fp, &p);
    fclose(fp);
    if (result) freePolygone(&p);
    remove("test_input.txt");
    return result;
}

int test_inputPolygone_invalid() {
    FILE* fp = fopen("test_invalid.txt", "w");
    fprintf(fp, "2 0.0 0.0 1.0 0.0\n"); // Invalid n=2
    fclose(fp);

    Polygone p;
    fp = fopen("test_invalid.txt", "r");
    int result = inputPolygone(fp, &p);
    fclose(fp);
    remove("test_invalid.txt");
    return !result;
}

int test_writePolygone() {
    NTYPE n = 3;
    TPoint* vertices = (TPoint*)malloc(n * sizeof(TPoint));
    vertices[0] = (TPoint){ 0.0f, 0.0f };
    vertices[1] = (TPoint){ 1.0f, 0.0f };
    vertices[2] = (TPoint){ 0.0f, 1.0f };
    Polygone p = { n, vertices };

    FILE* fp = fopen("test_write.txt", "w");
    int result = writePolygone(fp, &p);
    fclose(fp);
    freePolygone(&p);
    remove("test_write.txt");
    return result;
}

int test_addPolygonesFromFile() {
    // Test 1: Basic add without duplicates
    createTestFileWithOne("test_source.txt");
    FILE* dest = fopen("test_dest.txt", "w");
    fprintf(dest, "0\n");
    fclose(dest);

    int added1 = addPolygonesFromFile("test_source.txt", "test_dest.txt");
    if (added1 != 1) {
        remove("test_source.txt");
        remove("test_dest.txt");
        return FALSE;
    }

    // Test 2: With duplicate - should not add
    int added2 = addPolygonesFromFile("test_source.txt", "test_dest.txt"); // Same source
    if (added2 != 0) {
        remove("test_source.txt");
        remove("test_dest.txt");
        return FALSE;
    }

    // Test 3: With invalid in source
    createTestFileWithInvalid("invalid_source.txt");
    int added3 = addPolygonesFromFile("invalid_source.txt", "test_dest.txt");
    if (added3 != 0) {
        remove("test_source.txt");
        remove("test_dest.txt");
        remove("invalid_source.txt");
        return FALSE;
    }

    // Test 4: Non-existent source file
    remove("test_source.txt");
    int added4 = addPolygonesFromFile("nonexistent.txt", "test_dest.txt");
    remove("test_dest.txt");
    remove("invalid_source.txt");
    return added4 == 0;
}

int test_addSinglePolygonFromConsole_success() {
    // Simulate console input by redirecting stdin to temp file
    FILE* input = fopen("temp_input.txt", "w");
    if (!input) return 0;
    fprintf(input, "3\n0.0 0.0 1.0 0.0 0.0 1.0\n");
    fclose(input);

    FILE* saved = stdin;
    if (!freopen("temp_input.txt", "r", stdin)) return 0;

    FILE* dest = fopen("test_single_dest.txt", "w");
    if (!dest) return 0;
    fprintf(dest, "0\n");
    fclose(dest);

    int result = addSinglePolygonFromConsole("test_single_dest.txt");

    // restore stdin (Windows)
    freopen("CON", "r", stdin);

    remove("test_single_dest.txt");
    remove("temp_input.txt");
    return result == 1;
}

int test_addSinglePolygonFromConsole_duplicate() {
    createTestFileWithOne("test_single_dest.txt");

    // For duplicate test we need to provide same polygon on stdin
    FILE* input = fopen("temp_input.txt", "w");
    if (!input) return 0;
    fprintf(input, "3\n0.0 0.0 1.0 0.0 0.0 1.0\n");
    fclose(input);

    if (!freopen("temp_input.txt", "r", stdin)) return 0;
    int result = addSinglePolygonFromConsole("test_single_dest.txt");
    freopen("CON", "r", stdin);

    remove("test_single_dest.txt");
    remove("temp_input.txt");
    return result == 0; // Should fail due to duplicate
}

int test_addSinglePolygonFromConsole_invalid() {
    // Simulate invalid input (n=2)
    FILE* input = fopen("temp_invalid.txt", "w");
    if (!input) return 0;
    fprintf(input, "2\n0.0 0.0 1.0 0.0\n");
    fclose(input);

    FILE* dest = fopen("test_invalid_dest.txt", "w");
    if (!dest) return 0;
    fprintf(dest, "0\n");
    fclose(dest);

    if (!freopen("temp_invalid.txt", "r", stdin)) return 0;
    int result = addSinglePolygonFromConsole("test_invalid_dest.txt");
    freopen("CON", "r", stdin);

    remove("test_invalid_dest.txt");
    remove("temp_invalid.txt");
    return result == 0; // Should fail due to invalid n
}

int test_isEqualPolygone() {
    Polygone p1, p2;
    p1.n = p2.n = 3;
    p1.vertice = (TPoint*)malloc(3 * sizeof(TPoint));
    p2.vertice = (TPoint*)malloc(3 * sizeof(TPoint));

    p1.vertice[0] = (TPoint){ 0.0f, 0.0f };
    p1.vertice[1] = (TPoint){ 1.0f, 0.0f };
    p1.vertice[2] = (TPoint){ 0.0f, 1.0f };

    p2.vertice[0] = (TPoint){ 1.0f, 0.0f }; // Rotated
    p2.vertice[1] = (TPoint){ 0.0f, 1.0f };
    p2.vertice[2] = (TPoint){ 0.0f, 0.0f };

    int result = isEqualPolygone(&p1, &p2);

    freePolygone(&p1);
    freePolygone(&p2);

    return result;
}

int test_perimeterPolygone() {
    Polygone p;
    p.n = 3;
    p.vertice = (TPoint*)malloc(3 * sizeof(TPoint));
    p.vertice[0] = (TPoint){ 0.0f, 0.0f };
    p.vertice[1] = (TPoint){ 3.0f, 0.0f };
    p.vertice[2] = (TPoint){ 0.0f, 4.0f };

    PTYPE perimeter = perimeterPolygone(&p);
    int result = isEqual(perimeter, 12.0f);

    freePolygone(&p);
    return result;
}

// Test runner (if needed, integrate into main.c)
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
    return TRUE; // Placeholder
}

int testShowPolygoneFile() {
    createTestFile("test_show.txt");
    FILE* fp = fopen("test_show.txt", "r");
    showPolygonesFile(fp); // Assume it works if no crash
    fclose(fp);
    remove("test_show.txt");
    return TRUE;
}
