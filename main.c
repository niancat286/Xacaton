#include <stdio.h>
#include "Polygone.h"
#include <stdlib.h>
#include "testsPolygone.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu() {
    printf("========================================\n");
    printf("              POLYGON.MAIN              \n");
    printf("========================================\n");
    printf("  EXIT:\n");
    printf("  0. Exit\n");
    printf("\n");
    printf("  FILE OPERATIONS:\n");
    printf("  1. Add polygon from console to DB\n");
    printf("  2. Append polygons from another file\n");
    printf("  3. Display all polygons from DB\n");
    printf("  4. Display polygon by index\n");
    printf("  5. Delete polygon by index\n");
    printf("\n");
    printf("  ANALYSIS & SEARCH:\n");
    printf("  6. Find polygon with MAX perimeter\n");
    printf("  7. Find polygon with MIN area\n");
    printf("  8. Count convex polygons\n");
    printf("  9. Count polygons containing a point\n");
    printf("  10. Check if a polygon (from console) is in DB\n");
    printf("  11. Filter convex polygons to a new file\n");
    printf("\n");
    printf("  TESTING:\n");
    printf("  12. Run all unit tests (output to console & file)\n");
    printf("========================================\n");
    printf("Your choice: ");
}

int main() {
    const char* main_db = "polygons.txt";
    int choice;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: {
                printf("\n--- ADD POLYGON FROM CONSOLE ---\n");
                add_polygone_from_console(main_db);
                break;
            }
            
            case 2: {
                printf("\n--- APPEND POLYGONS FROM FILE ---\n");
                char src_filename[100];
                printf("Enter source filename: ");
                scanf("%99s", src_filename);
                clear_input_buffer();
                append_polygons_from_file(main_db, src_filename);
                break;
            }
            
            case 3: {
                printf("\n--- DISPLAY ALL POLYGONS ---\n");
                display_all_polygons(main_db);
                break;
            }
            
            case 4: {
                printf("\n--- DISPLAY POLYGON BY INDEX ---\n");
                NTYPE index;
                printf("Enter index to display: ");
                scanf("%u", &index);
                clear_input_buffer();
                display_polygone_by_index(main_db, index);
                break;
            }
            
            case 5: {
                printf("\n--- DELETE POLYGON BY INDEX ---\n");
                NTYPE index;
                printf("Enter index to delete: ");
                scanf("%u", &index);
                clear_input_buffer();
                if (delete_polygone_by_index(main_db, index)) {
                    printf("Successfully deleted polygon at index %u.\n", index);
                } else {
                    printf("Error: Index not found or file error.\n");
                }
                break;
            }
            
            case 6: {
                printf("\n--- FIND MAX PERIMETER POLYGON ---\n");
                Polygone p = {0, NULL};
                if (find_max_perimeter_polygone(main_db, &p)) {
                    printf("Found polygon with MAX perimeter: %.2f\n", perimeter_polygone(&p));
                    printf("  Number of vertices: %u\n", p.n);
                    printf("  Area: %.2f\n", area_polygone(&p));
                    free_polygone(&p);
                } else {
                    printf("Could not find (file is empty or error occurred).\n");
                }
                break;
            }
            
            case 7: {
                printf("\n--- FIND MIN AREA POLYGON ---\n");
                Polygone p = {0, NULL};
                if (find_min_area_polygone(main_db, &p)) {
                    printf("Found polygon with MIN area: %.2f\n", area_polygone(&p));
                    printf("  Number of vertices: %u\n", p.n);
                    printf("  Perimeter: %.2f\n", perimeter_polygone(&p));
                    free_polygone(&p);
                } else {
                    printf("Could not find (file is empty or error occurred).\n");
                }
                break;
            }
            
            case 8: {
                printf("\n--- COUNT CONVEX POLYGONS ---\n");
                NTYPE count = count_convex_polygons(main_db);
                printf("Number of convex polygons: %u\n", count);
                break;
            }
            
            case 9: {
                printf("\n--- COUNT POLYGONS CONTAINING POINT ---\n");
                TPoint point;
                printf("Enter point coordinates (x y): ");
                scanf("%f %f", &point.x, &point.y);
                clear_input_buffer();
                NTYPE count = count_polygons_containing_point(main_db, point);
                printf("Number of polygons containing point (%.2f, %.2f): %u\n",
                    point.x, point.y, count);
                break;
            }
            
            case 10: {
                printf("\n--- CHECK POLYGON PRESENCE ---\n");
                Polygone p = {0, NULL};
                printf("Enter a polygon from console to check for its presence.\n");
                printf("Enter number of vertices (>=3): ");
                scanf("%u", &p.n);
                clear_input_buffer();
                
                if (p.n >= 3) {
                    p.vertice = (TPoint*)malloc(p.n * sizeof(TPoint));
                    if (!p.vertice) {
                        printf("Memory allocation error!\n");
                        break;
                    }
                    
                    printf("Enter coordinates for %u vertices:\n", p.n);
                    for (NTYPE i = 0; i < p.n; i++) {
                        printf("  Vertex %u (x y): ", i + 1);
                        scanf("%f %f", &p.vertice[i].x, &p.vertice[i].y);
                        clear_input_buffer();
                    }
                    
                    if (is_present_in_file(main_db, &p)) {
                        printf("This polygon IS PRESENT in the file.\n");
                    } else {
                        printf("This polygon IS NOT in the file.\n");
                    }
                    free_polygone(&p);
                } else {
                    printf("Invalid number of vertices.\n");
                }
                break;
            }
            
            case 11: {
                printf("\n--- FILTER CONVEX POLYGONS ---\n");
                const char* filtered_db = "convex_only.txt";
                filter_polygons(main_db, filtered_db, is_convex);
                printf("Filtered convex polygons saved to '%s'\n", filtered_db);
                printf("\nDisplaying content of filtered file:\n");
                display_all_polygons(filtered_db);
                break;
            }
            
            case 12: {
                printf("\n========================================\n");
                printf("  RUNNING ALL UNIT TESTS\n");
                printf("========================================\n");
                printf("Output will be displayed here and saved to 'output.txt'\n\n");
                
                run_all_tests();
                
                printf("\nTests completed! Check 'output.txt' for full results.\n");
                break;
            }
            
            case 0:
                printf("\n========================================\n");
                printf("  Thank you for using Polygon Manager!\n");
                printf("========================================\n");
                break;
                
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        
        if (choice != 0 && choice >= 1 && choice <= 12) {
            printf("\nPress Enter to continue...");
            getchar();
        }
        
    } while (choice != 0);

    return 0;
}