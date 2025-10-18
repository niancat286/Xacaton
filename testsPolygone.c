#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Polygone.h"
#include <math.h>
#include "testsPolygone.h"

static int test_count = 0;
static int passed_count = 0;

static void check(const char* test_name, int condition) {
    test_count++;
    if (condition) {
        passed_count++;
        printf("[ OK ] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
    }
}


static void test_geometry() {
    printf("\n--- Testing Geometry ---\n");
    TPoint v1[] = {{0,0}, {5,0}, {5,5}, {0,5}};
    Polygone square = {4, v1};
    check("Area of 5x5 square", fabsf(area_polygone(&square) - 25.0f) < 1e-6);
    check("Perimeter of 5x5 square", fabsf(perimeter_polygone(&square) - 20.0f) < 1e-6);
    check("Square is convex", is_convex(&square) == TRUE);

    TPoint v2[] = {{0,0}, {4,0}, {2,2}, {4,4}, {0,4}};
    Polygone arrow = {5, v2};
    check("Arrow shape is not convex", is_convex(&arrow) == FALSE);

    TPoint p_inside = {2, 2};
    TPoint p_outside = {6, 6};
    check("Point (2,2) is inside square", is_point_inside(&square, p_inside) == TRUE);
    check("Point (6,6) is outside square", is_point_inside(&square, p_outside) == FALSE);
}

static void test_equality() {
    printf("\n--- Testing Equality ---\n");
    TPoint v1[] = {{0,0}, {1,0}, {1,1}, {0,1}};
    Polygone p1 = {4, v1};

    TPoint v2[] = {{1,0}, {1,1}, {0,1}, {0,0}}; // Same, shifted
    Polygone p2 = {4, v2};

    TPoint v3[] = {{0,0}, {0,1}, {1,1}, {1,0}}; // Same, reversed
    Polygone p3 = {4, v3};

    TPoint v4[] = {{0,0}, {2,0}, {2,2}, {0,2}}; // Different
    Polygone p4 = {4, v4};

    check("Equality with shifted vertices", isEqualPolygone(&p1, &p2) == TRUE);
    check("Equality with reversed vertices", isEqualPolygone(&p1, &p3) == TRUE);
    check("Inequality for different polygons", isEqualPolygone(&p1, &p4) == FALSE);
}

static void test_file_operations() {
    printf("\n--- Testing File Operations ---\n");
    const char* test_filename = "test_file.bin";

    FILE* fp = fopen(test_filename, "wb");
    NTYPE count = 2;
    fwrite(&count, sizeof(NTYPE), 1, fp);
    TPoint v1[] = {{0,0}, {1,0}, {0,1}};
    Polygone p1 = {3, v1};
    fwrite(&p1.n, sizeof(NTYPE), 1, fp);
    fwrite(p1.vertice, sizeof(TPoint), p1.n, fp);
    TPoint v2[] = {{0,0}, {2,0}, {2,2}, {0,2}};
    Polygone p2 = {4, v2};
    fwrite(&p2.n, sizeof(NTYPE), 1, fp);
    fwrite(p2.vertice, sizeof(TPoint), p2.n, fp);
    fclose(fp);

    check("is_present_in_file finds existing polygon", is_present_in_file(test_filename, &p1) == TRUE);
    TPoint v3[] = {{9,9}};
    Polygone p3 = {1, v3};
    check("is_present_in_file doesn't find non-existing one", is_present_in_file(test_filename, &p3) == FALSE);

    Polygone min_p = {0, NULL};
    find_min_area_polygone(test_filename, &min_p);
    check("find_min_area_polygone finds the correct polygon", isEqualPolygone(&min_p, &p1) == TRUE);
    free_polygone(&min_p);

    remove(test_filename);
}

void run_all_tests() {
    test_geometry();
    test_equality();
    test_file_operations();

    printf("\n-----------------------------------\n");
    printf("SUMMARY: %d out of %d tests passed.\n", passed_count, test_count);
    printf("-----------------------------------\n");
}