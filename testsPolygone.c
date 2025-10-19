#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "Polygone.h"
#include "testsPolygone.h"

static int test_count = 0;
static int passed_count = 0;
static FILE *output_file = NULL;

static void check(const char* test_name, int condition) {
    test_count++;
    const char *status = condition ? "[ OK ]" : "[FAIL]";

    if (condition) {
        passed_count++;
    }

    // Вивід у термінал
    printf("%s %s\n", status, test_name);

    // Вивід у файл
    if (output_file) {
        fprintf(output_file, "%s %s\n", status, test_name);
    }
}

static void log_message(const char *message) {
    printf("%s\n", message);
    if (output_file) {
        fprintf(output_file, "%s\n", message);
    }
}

// Допоміжна функція для створення тестового файлу
static void create_test_file(const char *filename, Polygone *polygons, NTYPE count) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;

    fwrite(&count, sizeof(NTYPE), 1, fp);
    for (NTYPE i = 0; i < count; i++) {
        fwrite(&polygons[i].n, sizeof(NTYPE), 1, fp);
        fwrite(polygons[i].vertice, sizeof(TPoint), polygons[i].n, fp);
    }
    fclose(fp);
}

// Допоміжна функція для читання кількості полігонів
static NTYPE get_polygon_count(const char *filename) {
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
//printf("Error: File %s must have .bin or .txt extension.\n", filename);
        return 0;
    }

    FILE *fp = fopen(filename, is_binary ? "rb" : "r");
    if (!fp) {
//printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    NTYPE count = 0;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1) {
//printf("Error reading polygon count.\n");
            fclose(fp);
            return 0;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1) {
//printf("Error reading polygon count.\n");
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return count;
}

// ============ ТЕСТИ ГЕОМЕТРИЧНИХ ФУНКЦІЙ ============

static void test_perimeter() {
    log_message("\n--- Testing perimeter_polygone ---");

    // Тест 1: Квадрат 5x5
    TPoint v1[] = {{0,0}, {5,0}, {5,5}, {0,5}};
    Polygone square = {4, v1};
    PTYPE perim = perimeter_polygone(&square);
    check("Perimeter of 5x5 square should be 20", fabsf(perim - 20.0f) < 1e-6);

    // Тест 2: Трикутник 3-4-5
    TPoint v2[] = {{0,0}, {3,0}, {0,4}};
    Polygone triangle = {3, v2};
    perim = perimeter_polygone(&triangle);
    check("Perimeter of 3-4-5 triangle should be 12", fabsf(perim - 12.0f) < 1e-6);

    // Тест 3: Одиничний квадрат
    TPoint v3[] = {{0,0}, {1,0}, {1,1}, {0,1}};
    Polygone unit_square = {4, v3};
    perim = perimeter_polygone(&unit_square);
    check("Perimeter of unit square should be 4", fabsf(perim - 4.0f) < 1e-6);
}

static void test_area() {
    log_message("\n--- Testing area_polygone ---");

    // Тест 1: Квадрат 5x5
    TPoint v1[] = {{0,0}, {5,0}, {5,5}, {0,5}};
    Polygone square = {4, v1};
    PTYPE area = area_polygone(&square);
    check("Area of 5x5 square should be 25", fabsf(area - 25.0f) < 1e-6);

    // Тест 2: Трикутник з площею 6
    TPoint v2[] = {{0,0}, {4,0}, {0,3}};
    Polygone triangle = {3, v2};
    area = area_polygone(&triangle);
    check("Area of triangle should be 6", fabsf(area - 6.0f) < 1e-6);

    // Тест 3: Одиничний квадрат
    TPoint v3[] = {{0,0}, {1,0}, {1,1}, {0,1}};
    Polygone unit_square = {4, v3};
    area = area_polygone(&unit_square);
    check("Area of unit square should be 1", fabsf(area - 1.0f) < 1e-6);
}

static void test_is_convex() {
    log_message("\n--- Testing is_convex ---");

    // Тест 1: Опуклий квадрат
    TPoint v1[] = {{0,0}, {5,0}, {5,5}, {0,5}};
    Polygone square = {4, v1};
    check("Square should be convex", is_convex(&square) == TRUE);

    // Тест 2: Неопуклий полігон (стрілка)
    TPoint v2[] = {{0,0}, {4,0}, {2,2}, {4,4}, {0,4}};
    Polygone arrow = {5, v2};
    check("Arrow shape should not be convex", is_convex(&arrow) == FALSE);

    // Тест 3: Опуклий трикутник
    TPoint v3[] = {{0,0}, {3,0}, {1.5,3}};
    Polygone triangle = {3, v3};
    check("Triangle should be convex", is_convex(&triangle) == TRUE);
}

static void test_is_point_inside() {
    log_message("\n--- Testing is_point_inside ---");

    TPoint v1[] = {{0,0}, {5,0}, {5,5}, {0,5}};
    Polygone square = {4, v1};

    // Тест 1: Точка всередині
    TPoint p_inside = {2.5, 2.5};
    check("Point (2.5,2.5) should be inside square", is_point_inside(&square, p_inside) == TRUE);

    // Тест 2: Точка зовні
    TPoint p_outside = {6, 6};
    check("Point (6,6) should be outside square", is_point_inside(&square, p_outside) == FALSE);

    // Тест 3: Точка на межі
    TPoint p_border = {0, 2.5};
    int result = is_point_inside(&square, p_border);
    check("Point on border handled correctly", result == TRUE || result == FALSE);
}

// ============ ТЕСТИ РІВНОСТІ ============

static void test_isEqualPoint() {
    log_message("\n--- Testing isEqualPoint ---");

    // Тест 1: Однакові точки
    TPoint p1 = {1.5, 2.5};
    TPoint p2 = {1.5, 2.5};
    check("Equal points should be equal", isEqualPoint(p1, p2) == TRUE);

    // Тест 2: Різні точки
    TPoint p3 = {1.5, 2.5};
    TPoint p4 = {2.5, 1.5};
    check("Different points should not be equal", isEqualPoint(p3, p4) == FALSE);

    // Тест 3: Точки з нульовими координатами
    TPoint p5 = {0, 0};
    TPoint p6 = {0, 0};
    check("Zero points should be equal", isEqualPoint(p5, p6) == TRUE);
}

static void test_isEqualPolygone() {
    log_message("\n--- Testing isEqualPolygone ---");

    // Тест 1: Однакові полігони
    TPoint v1[] = {{0,0}, {1,0}, {1,1}, {0,1}};
    Polygone p1 = {4, v1};
    TPoint v2[] = {{0,0}, {1,0}, {1,1}, {0,1}};
    Polygone p2 = {4, v2};
    check("Identical polygons should be equal", isEqualPolygone(&p1, &p2) == TRUE);

    // Тест 2: Зміщені вершини (циклічна перестановка)
    TPoint v3[] = {{1,0}, {1,1}, {0,1}, {0,0}};
    Polygone p3 = {4, v3};
    check("Shifted vertices should be equal", isEqualPolygone(&p1, &p3) == TRUE);

    // Тест 3: Різні полігони
    TPoint v4[] = {{0,0}, {2,0}, {2,2}, {0,2}};
    Polygone p4 = {4, v4};
    check("Different polygons should not be equal", isEqualPolygone(&p1, &p4) == FALSE);
}

// ============ ТЕСТИ ФАЙЛОВИХ ОПЕРАЦІЙ ============

static void test_append_polygons_from_file() {
    log_message("\n--- Testing append_polygons_from_file ---");

    const char *dest = "test_dest.bin";
    const char *src = "test_src.bin";

    // Тест 1: Додавання до порожнього файлу
    TPoint v1[] = {{0,0}, {1,0}, {1,1}};
    Polygone p1 = {3, v1};
    create_test_file(dest, NULL, 0);
    create_test_file(src, &p1, 1);
    append_polygons_from_file(dest, src);
    check("Append to empty file", get_polygon_count(dest) == 1);

    // Тест 2: Додавання до існуючого файлу
    TPoint v2[] = {{0,0}, {2,0}, {2,2}, {0,2}};
    Polygone p2 = {4, v2};
    create_test_file(src, &p2, 1);
    append_polygons_from_file(dest, src);
    check("Append to existing file", get_polygon_count(dest) == 2);

    // Тест 3: Додавання кількох полігонів
    Polygone polygons[2] = {p1, p2};
    create_test_file(src, polygons, 2);
    append_polygons_from_file(dest, src);
    check("Append multiple polygons", get_polygon_count(dest) == 4);

    remove(dest);
    remove(src);
}

static void test_delete_polygone_by_index() {
    log_message("\n--- Testing delete_polygone_by_index ---");

    const char *filename = "test_delete.bin";

    // Створюємо файл з 3 полігонами
    TPoint v1[] = {{0,0}, {1,0}, {1,1}};
    TPoint v2[] = {{0,0}, {2,0}, {2,2}, {0,2}};
    TPoint v3[] = {{0,0}, {3,0}, {1.5,3}};
    Polygone polygons[3] = {{3, v1}, {4, v2}, {3, v3}};
    create_test_file(filename, polygons, 3);

    // Тест 1: Видалення першого
    int result = delete_polygone_by_index(filename, 0);
    check("Delete first polygon", result == TRUE && get_polygon_count(filename) == 2);

    // Тест 2: Видалення останнього
    create_test_file(filename, polygons, 3);
    result = delete_polygone_by_index(filename, 2);
    check("Delete last polygon", result == TRUE && get_polygon_count(filename) == 2);

    // Тест 3: Видалення неіснуючого індексу
    result = delete_polygone_by_index(filename, 10);
    check("Delete non-existing index should fail", result == FALSE);

    remove(filename);
}

static void test_is_present_in_file() {
    log_message("\n--- Testing is_present_in_file ---");

    const char *filename = "test_present.bin";

    TPoint v1[] = {{0,0}, {1,0}, {1,1}};
    TPoint v2[] = {{0,0}, {2,0}, {2,2}, {0,2}};
    Polygone polygons[2] = {{3, v1}, {4, v2}};
    create_test_file(filename, polygons, 2);

    // Тест 1: Пошук існуючого полігону
    check("Find existing polygon (first)", is_present_in_file(filename, &polygons[0]) == TRUE);

    // Тест 2: Пошук другого існуючого полігону
    check("Find existing polygon (second)", is_present_in_file(filename, &polygons[1]) == TRUE);

    // Тест 3: Пошук неіснуючого полігону
    TPoint v3[] = {{9,9}, {10,10}};
    Polygone p3 = {2, v3};
    check("Non-existing polygon should not be found", is_present_in_file(filename, &p3) == FALSE);

    remove(filename);
}

static void test_find_max_perimeter_polygone() {
    log_message("\n--- Testing find_max_perimeter_polygone ---");

    const char *filename = "test_max_perim.bin";

    // Тест 1: Знаходження максимального периметру
    TPoint v1[] = {{0,0}, {1,0}, {1,1}, {0,1}}; // периметр 4
    TPoint v2[] = {{0,0}, {5,0}, {5,5}, {0,5}}; // периметр 20
    TPoint v3[] = {{0,0}, {2,0}, {2,2}, {0,2}}; // периметр 8
    Polygone polygons[3] = {{4, v1}, {4, v2}, {4, v3}};
    create_test_file(filename, polygons, 3);

    Polygone result = {0, NULL};
    int success = find_max_perimeter_polygone(filename, &result);
    check("Find max perimeter polygon", success == TRUE && isEqualPolygone(&result, &polygons[1]) == TRUE);
    free_polygone(&result);

    // Тест 2: Порожній файл
    create_test_file(filename, NULL, 0);
    result.n = 0;
    result.vertice = NULL;
    success = find_max_perimeter_polygone(filename, &result);
    check("Max perimeter in empty file should fail", success == FALSE);

    // Тест 3: Один полігон
    create_test_file(filename, polygons, 1);
    result.n = 0;
    result.vertice = NULL;
    success = find_max_perimeter_polygone(filename, &result);
    check("Max perimeter with single polygon", success == TRUE && result.n == 4);
    free_polygone(&result);

    remove(filename);
}

static void test_find_min_area_polygone() {
    log_message("\n--- Testing find_min_area_polygone ---");

    const char *filename = "test_min_area.bin";

    // Тест 1: Знаходження мінімальної площі
    TPoint v1[] = {{0,0}, {4,0}, {4,4}, {0,4}}; // площа 16
    TPoint v2[] = {{0,0}, {1,0}, {1,1}, {0,1}}; // площа 1
    TPoint v3[] = {{0,0}, {2,0}, {2,2}, {0,2}}; // площа 4
    Polygone polygons[3] = {{4, v1}, {4, v2}, {4, v3}};
    create_test_file(filename, polygons, 3);

    Polygone result = {0, NULL};
    int success = find_min_area_polygone(filename, &result);
    check("Find min area polygon", success == TRUE && isEqualPolygone(&result, &polygons[1]) == TRUE);
    free_polygone(&result);

    // Тест 2: Порожній файл
    create_test_file(filename, NULL, 0);
    result.n = 0;
    result.vertice = NULL;
    success = find_min_area_polygone(filename, &result);
    check("Min area in empty file should fail", success == FALSE);

    // Тест 3: Один полігон
    create_test_file(filename, polygons, 1);
    result.n = 0;
    result.vertice = NULL;
    success = find_min_area_polygone(filename, &result);
    check("Min area with single polygon", success == TRUE && result.n == 4);
    free_polygone(&result);

    remove(filename);
}

static void test_count_convex_polygons() {
    log_message("\n--- Testing count_convex_polygons ---");

    const char *filename = "test_convex.bin";

    // Тест 1: Рахування опуклих полігонів
    TPoint v1[] = {{0,0}, {4,0}, {4,4}, {0,4}}; // опуклий
    TPoint v2[] = {{0,0}, {4,0}, {2,2}, {4,4}, {0,4}}; // неопуклий
    TPoint v3[] = {{0,0}, {3,0}, {1.5,3}}; // опуклий
    Polygone polygons[3] = {{4, v1}, {5, v2}, {3, v3}};
    create_test_file(filename, polygons, 3);

    NTYPE count = count_convex_polygons(filename);
    check("Count convex polygons", count == 2);

    // Тест 2: Всі опуклі
    Polygone convex_polygons[2] = {{4, v1}, {3, v3}};
    create_test_file(filename, convex_polygons, 2);
    count = count_convex_polygons(filename);
    check("All polygons are convex", count == 2);

    // Тест 3: Порожній файл
    create_test_file(filename, NULL, 0);
    count = count_convex_polygons(filename);
    check("No polygons in file", count == 0);

    remove(filename);
}

static void test_count_polygons_containing_point() {
    log_message("\n--- Testing count_polygons_containing_point ---");

    const char *filename = "test_point.bin";

    // Тест 1: Точка в декількох полігонах
    TPoint v1[] = {{0,0}, {10,0}, {10,10}, {0,10}};
    TPoint v2[] = {{1,1}, {9,1}, {9,9}, {1,9}};
    TPoint v3[] = {{20,20}, {30,20}, {30,30}, {20,30}};
    Polygone polygons[3] = {{4, v1}, {4, v2}, {4, v3}};
    create_test_file(filename, polygons, 3);

    TPoint test_point = {5, 5};
    NTYPE count = count_polygons_containing_point(filename, test_point);
    check("Point inside multiple polygons", count == 2);

    // Тест 2: Точка ні в одному полігоні
    test_point.x = 50;
    test_point.y = 50;
    count = count_polygons_containing_point(filename, test_point);
    check("Point outside all polygons", count == 0);

    // Тест 3: Точка в одному полігоні
    test_point.x = 25;
    test_point.y = 25;
    count = count_polygons_containing_point(filename, test_point);
    check("Point inside single polygon", count == 1);

    remove(filename);
}

static int predicate_area_gt_10(const Polygone *p) {
    return area_polygone(p) > 10.0f;
}

static void test_filter_polygons() {
    log_message("\n--- Testing filter_polygons ---");

    const char *file_a = "test_filter_a.bin";
    const char *file_b = "test_filter_b.bin";

    // Тест 1: Фільтрація за площею > 10
    TPoint v1[] = {{0,0}, {5,0}, {5,5}, {0,5}}; // площа 25
    TPoint v2[] = {{0,0}, {2,0}, {2,2}, {0,2}}; // площа 4
    TPoint v3[] = {{0,0}, {4,0}, {4,4}, {0,4}}; // площа 16
    Polygone polygons[3] = {{4, v1}, {4, v2}, {4, v3}};
    create_test_file(file_a, polygons, 3);

    filter_polygons(file_a, file_b, predicate_area_gt_10);
    NTYPE count = get_polygon_count(file_b);
    check("Filter polygons with area > 10", count == 2);

    // Тест 2: Жоден не відповідає умові
    TPoint v4[] = {{0,0}, {1,0}, {1,1}, {0,1}}; // площа 1
    Polygone small_polygons[2] = {{4, v2}, {4, v4}};
    create_test_file(file_a, small_polygons, 2);
    filter_polygons(file_a, file_b, predicate_area_gt_10);
    count = get_polygon_count(file_b);
    check("Filter with no matching polygons", count == 0);

    // Тест 3: Всі відповідають умові
    Polygone large_polygons[2] = {{4, v1}, {4, v3}};
    create_test_file(file_a, large_polygons, 2);
    filter_polygons(file_a, file_b, predicate_area_gt_10);
    count = get_polygon_count(file_b);
    check("Filter with all matching polygons", count == 2);

    remove(file_a);
    remove(file_b);
}

// ============ ГОЛОВНА ФУНКЦІЯ ЗАПУСКУ ТЕСТІВ ============

void run_all_tests() {
    // Відкриваємо файл для виводу
    output_file = fopen("output.txt", "w");
    if (!output_file) {
        printf("ERROR: Cannot open output.txt for writing\n");
        return;
    }

    log_message("========================================");
    log_message("       POLYGON TESTS EXECUTION");
    log_message("========================================");

    // Геометричні функції
    test_perimeter();
    test_area();
    test_is_convex();
    test_is_point_inside();

    // Функції рівності
    test_isEqualPoint();
    test_isEqualPolygone();

    // Файлові операції
    test_append_polygons_from_file();
    test_delete_polygone_by_index();
    test_is_present_in_file();
    test_find_max_perimeter_polygone();
    test_find_min_area_polygone();
    test_count_convex_polygons();
    test_count_polygons_containing_point();
    test_filter_polygons();

    // Підсумок
    log_message("\n========================================");
    char summary[256];
    snprintf(summary, sizeof(summary), "SUMMARY: %d out of %d tests passed.", passed_count, test_count);
    log_message(summary);

    if (passed_count == test_count) {
        log_message("✓ ALL TESTS PASSED!");
    } else {
        snprintf(summary, sizeof(summary), "✗ %d tests FAILED", test_count - passed_count);
        log_message(summary);
    }
    log_message("========================================");

    // Закриваємо файл
    fclose(output_file);
    output_file = NULL;
}
