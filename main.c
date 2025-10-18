#include <stdio.h>
#include "Polygone.h"
#include <stdlib.h>

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu() {
    printf("\n--- POLYGON MANIPULATION MENU ---\n");
    printf("  FILE OPERATIONS:\n");
    printf("  1. Add polygon from console to DB\n");
    printf("  2. Append polygons from another file\n");
    printf("  3. Display all polygons from DB\n");
    printf("  4. Display polygon by index\n");
    printf("  5. Delete polygon by index\n");
    printf("  ANALYSIS & SEARCH:\n");
    printf("  6. Find polygon with MAX perimeter\n");
    printf("  7. Find polygon with MIN area\n");
    printf("  8. Count convex polygons\n");
    printf("  9. Count polygons containing a point\n");
    printf("  10. Check if a polygon (from console) is in DB\n");
    printf("  11. Filter convex polygons to a new file\n");
    printf("  0. Exit\n");
    printf("Your choice: ");
}

int main() {
    const char* main_db = "polygons.bin";
    int choice;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                add_polygone_from_console(main_db);
                break;
            case 2: {
                char src_filename[100];
                printf("Enter source filename: ");
                scanf("%99s", src_filename);
                clear_input_buffer();
                append_polygons_from_file(main_db, src_filename);
                break;
            }
            case 3:
                display_all_polygons(main_db);
                break;
            case 4: {
                NTYPE index;
                printf("Enter index to display: ");
                scanf("%u", &index);
                clear_input_buffer();
                display_polygone_by_index(main_db, index);
                break;
            }
            case 5: {
                NTYPE index;
                printf("Enter index to delete: ");
                scanf("%u", &index);
                clear_input_buffer();
                if (delete_polygone_by_index(main_db, index)) {
                    printf("Successfully deleted.\n");
                } else {
                    printf("Error: index not found or file error.\n");
                }
                break;
            }
            case 6: {
                Polygone p = {0, NULL};
                if (find_max_perimeter_polygone(main_db, &p)) {
                    printf("Found polygon with MAX perimeter (%.2f):\n", perimeter_polygone(&p));
                    display_polygone_by_index(main_db, 0);
                    free_polygone(&p);
                } else {
                    printf("Could not find (file is empty?).\n");
                }
                break;
            }
            case 7: {
                Polygone p = {0, NULL};
                if (find_min_area_polygone(main_db, &p)) {
                    printf("Found polygon with MIN area (%.2f):\n", area_polygone(&p));
                    free_polygone(&p);
                } else {
                    printf("Could not find (file is empty?).\n");
                }
                break;
            }
            case 8:
                printf("Number of convex polygons: %u\n", count_convex_polygons(main_db));
                break;
            case 9: {
                TPoint point;
                printf("Enter point coordinates (x y): ");
                scanf("%f %f", &point.x, &point.y);
                clear_input_buffer();
                printf("Number of polygons containing point (%.2f, %.2f): %u\n",
                    point.x, point.y, count_polygons_containing_point(main_db, point));
                break;
            }
            case 10: {
                Polygone p = {0, NULL};
                printf("Enter a polygon from console to check for its presence.\n");
                printf("Enter number of vertices (>=3): ");
                scanf("%u", &p.n);
                clear_input_buffer();
                if (p.n >= 3) {
                    p.vertice = (TPoint*)malloc(p.n * sizeof(TPoint));
                    printf("Enter coordinates for %u vertices:\n", p.n);
                    for (NTYPE i = 0; i < p.n; i++) {
                        scanf("%f %f", &p.vertice[i].x, &p.vertice[i].y);
                        clear_input_buffer();
                    }
                    if (is_present_in_file(main_db, &p)) {
                        printf("This polygon IS PRESENT in the file.\n");
                    } else {
                        printf("This polygon IS NOT in the file.\n");
                    }
                    free_polygone(&p);
                }
                break;
            }
            case 11: {
                const char* filtered_db = "convex_only.bin";
                filter_polygons(main_db, filtered_db, is_convex);
                printf("Filtered polygons saved to '%s'. Displaying content:\n", filtered_db);
                display_all_polygons(filtered_db);
                break;
            }
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}