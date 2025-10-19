#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "Polygone.h"

#define EPSILON 1e-6f

static int read_one_polygone(FILE *fp, Polygone *p)
{
    if (fread(&p->n, sizeof(NTYPE), 1, fp) != 1 || p->n < 3)
        return FALSE;
    p->vertice = (TPoint *)malloc(p->n * sizeof(TPoint));
    if (!p->vertice)
        return FALSE;
    if (fread(p->vertice, sizeof(TPoint), p->n, fp) != p->n)
    {
       printf("readone+"); 
        free(p->vertice);
        p->vertice = NULL;
        return FALSE;
        printf("-\n");
    }
    return TRUE;
}

static int write_one_polygone(FILE *fp, const Polygone *p)
{
    if (fwrite(&p->n, sizeof(NTYPE), 1, fp) != 1)
        return FALSE;
    if (fwrite(p->vertice, sizeof(TPoint), p->n, fp) != p->n)
        return FALSE;
    return TRUE;
}

static int read_one_polygone_from_text_file(FILE *fp, Polygone *p)
{    
    if (fscanf(fp, "%u", &p->n) != 1) {
        return FALSE;
    }

    if (p->n < 3) return FALSE;
    
    p->vertice = (TPoint *)malloc(p->n * sizeof(TPoint));
    if (!p->vertice) {
        return FALSE;
    }

    for (NTYPE i = 0; i < p->n; i++) {
        if (fscanf(fp, "%f %f", &p->vertice[i].x, &p->vertice[i].y) != 2) {
            free(p->vertice);
            p->vertice = NULL;
            return FALSE;
        }
    }

    return TRUE;
}


static int write_one_polygone_to_text_file(FILE *fp, const Polygone *p)
{
    
    if (fprintf(fp, "%u ", p->n) < 0) {
        return FALSE;
    }

    for (NTYPE i = 0; i < p->n; i++) {
        if (fprintf(fp, "%g %g ", p->vertice[i].x, p->vertice[i].y) < 0) {
            return FALSE;
        }
    }
    return TRUE;
}

// ВИПРАВЛЕНО: Допоміжна функція для is_point_inside, щоб уникнути UB
static PTYPE area_from_points(TPoint p1, TPoint p2, TPoint p3)
{
    return fabsf(p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0f;
}


void free_polygone(Polygone *p)
{
    if (p && p->vertice)
    {
        free(p->vertice);
        p->vertice = NULL;
        p->n = 0;
    }
}

// task а)
void add_polygone_from_console(const char *filename)
{

    Polygone p = {0, NULL};
    printf("Enter number of vertices N (>=3): ");
    if (scanf("%u", &p.n) != 1 || p.n < 3)
    {
        printf("Error: number of vertices must be 3 or more.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    p.vertice = (TPoint *)malloc(p.n * sizeof(TPoint));
    if (!p.vertice)
    {
        printf("Memory allocation error!\n");
        return;
    }

    printf("Enter coordinates for %u vertices (format: x y):\n", p.n);
    for (NTYPE i = 0; i < p.n; i++)
    {
        printf("Vertex %u: ", i + 1);
        if (scanf("%f %f", &p.vertice[i].x, &p.vertice[i].y) != 2)
        {
            printf("Error reading coordinates.\n");
            free_polygone(&p);
            return;
        }
    }
    
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
        printf("Error: File must have .bin or .txt extension.\n");
        free_polygone(&p);
        return;
    }

    if(is_present_in_file(filename, &p)){
        free_polygone(&p); return;
    }

    FILE *fp = fopen(filename, is_binary ? "r+b" : "r+");
    if (!fp) {
        fp = fopen(filename, is_binary ? "w+b" : "w+");
        if (!fp) {
            printf("Error: Cannot open or create file %s\n", filename);
            free_polygone(&p);
            return;
        }
        NTYPE zero = 0;
        if (is_binary) {
            fwrite(&zero, sizeof(NTYPE), 1, fp);
        } else {
            fprintf(fp, "%10u ", zero);
        }
    }

    NTYPE count;
    rewind(fp);
    if (is_binary) {
        fread(&count, sizeof(NTYPE), 1, fp);
    } else {
        fscanf(fp, "%u", &count);
    }

    
    fseek(fp, 0, SEEK_END);
    if (is_binary) {
        write_one_polygone(fp, &p);
    } else {
        write_one_polygone_to_text_file(fp, &p);
    }

    count++;
    rewind(fp);
    if (is_binary) {
        fwrite(&count, sizeof(NTYPE), 1, fp);
    } else {
        fprintf(fp, "%10u ", count);
    }

    fclose(fp); 
    free_polygone(&p);
    printf("Polygon added successfully.\n");
}

int is_valid(const char *filename){
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
        printf("Error: Source file %s must have .bin or .txt extension.\n", filename);
        return FALSE;
    }

    FILE *fp = fopen(filename, is_binary ? "rb" : "r");
    if (!fp) {
// printf("Error: Cannot open file %s\n", filename);
        return FALSE;
    }

    int count;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1) {
// printf("Error: Failed to read polygon count from %s\n", filename);
            fclose(fp);
            return FALSE;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1) {
// printf("Error: Failed to read polygon count from %s\n", filename);
            fclose(fp);
            return FALSE;
        }
    }

    if (count < 1) {
// printf("Error: Invalid polygon count in %s\n", filename);
        fclose(fp);
        return FALSE;
    }

    for (NTYPE i = 0; i < count; i++) {
        int n;
        if (is_binary) {
            if (fread(&n, sizeof(NTYPE), 1, fp) != 1 || n < 3) {
                // printf("Error: Failed to read vertex count for polygon %u in %s\n", i, filename);
                fclose(fp);
                return FALSE;
            }
            // Skip vertices
            if (fseek(fp, n * sizeof(TPoint), SEEK_CUR) != 0) {
                // printf("Error: Failed to skip vertices for polygon %u in %s\n", i, filename);
                fclose(fp);
                return FALSE;
            }
        } else {
            if (fscanf(fp, "%u", &n) != 1 || n < 3) {
                printf("%d\n", n);
                // printf("Error: Failed to read vertex count for polygon %u in %s\n", i, filename);
                fclose(fp);
                return FALSE;
            }
            // Skip vertices
            float x, y;
            for (NTYPE j = 0; j < n; j++) {
                if (fscanf(fp, "%f %f", &x, &y) != 2) {
                    // printf("Error: Failed to read vertex %u for polygon %u in %s\n", j, i, filename);
                    fclose(fp);
                    return FALSE;
                }
            }
        }
    }
    fclose(fp);
    return TRUE;

}

// task б)
void append_polygons_from_file(const char *dest_filename, const char *src_filename)
{
    
    if(!is_valid(src_filename)){
        printf("%s is corrupted or contains invalid polygons.\n", src_filename);
        return;
    }
    const char *src_ext = strrchr(src_filename, '.');
    int src_is_binary = src_ext && strcmp(src_ext, ".bin") == 0;
    int src_is_text = src_ext && strcmp(src_ext, ".txt") == 0;
    if (!src_is_binary && !src_is_text) {
        printf("Error: Source file %s must have .bin or .txt extension.\n", src_filename);
        return;
    }


    const char *dest_ext = strrchr(dest_filename, '.');
    int dest_is_binary = dest_ext && strcmp(dest_ext, ".bin") == 0;
    int dest_is_text = dest_ext && strcmp(dest_ext, ".txt") == 0;
    if (!dest_is_binary && !dest_is_text) {
        printf("Error: Destination file %s must have .bin or .txt extension.\n", dest_filename);
        return;
    }

    FILE *fp_src = fopen(src_filename, src_is_binary ? "rb" : "r");
    if (!fp_src) {
        printf("Error: Cannot open source file %s\n", src_filename);
        return;
    }

    int src_count;
    if (src_is_binary) {
        if (fread(&src_count, sizeof(NTYPE), 1, fp_src) != 1 || src_count <= 0) {
            printf("Source file %s is empty or corrupted.\n", src_filename);
            fclose(fp_src);
            return;
        }
    } else {
        if (fscanf(fp_src, "%u", &src_count) != 1 || src_count <= 0) {
            printf("Source file %s is empty or corrupted.\n", src_filename);
            fclose(fp_src);
            return;
        }
    }

    FILE *fp_dest = fopen(dest_filename, dest_is_binary ? "r+b" : "r+");
    if (!fp_dest) {
        fp_dest = fopen(dest_filename, dest_is_binary ? "w+b" : "w+");
        if (!fp_dest) {
            printf("Error: Cannot open or create destination file %s\n", dest_filename);
            fclose(fp_src);
            return;
        }
        NTYPE zero = 0;
        if (dest_is_binary) {
            fwrite(&zero, sizeof(NTYPE), 1, fp_dest);
        } else {
            fprintf(fp_dest, "%10u ", zero);
        }
    }
    
    int dest_count;
    rewind(fp_dest);
    if (dest_is_binary) {
        fread(&dest_count, sizeof(NTYPE), 1, fp_dest) != 1;
    } else {
        fscanf(fp_dest, "%u", &dest_count);
    }

    // Move to end of destination file
    fseek(fp_dest, 0, SEEK_END);
    int present = 0;
    for (NTYPE i = 0; i < src_count; i++) {
        Polygone p;
        p.vertice = NULL;
        if (src_is_binary ? read_one_polygone(fp_src, &p) : read_one_polygone_from_text_file(fp_src, &p)) {
            if(is_present_in_file(dest_filename, &p)){
                present++;
                free_polygone(&p);
                continue;
            }
            if (dest_is_binary) {
                if (!write_one_polygone(fp_dest, &p)) {
                    printf("Error writing polygon to %s.\n", dest_filename);
                    free_polygone(&p);
                    fclose(fp_src);
                    fclose(fp_dest);
                    return;
                }
            } else {
                if (!write_one_polygone_to_text_file(fp_dest, &p)) {
                    printf("Error writing polygon to %s.\n", dest_filename);
                    free_polygone(&p);
                    fclose(fp_src);
                    fclose(fp_dest);
                    return;
                }
            }
            free_polygone(&p);
        } else {
            printf("Error reading polygon from %s.\n", src_filename);
free_polygone(&p);
            fclose(fp_src);
            fclose(fp_dest);
            return;
        }
    }
    fclose(fp_src);

    NTYPE total_count = dest_count + src_count - present;
    rewind(fp_dest);
    if (dest_is_binary) {
        fwrite(&total_count, sizeof(NTYPE), 1, fp_dest);
    } else {
        fprintf(fp_dest, "%10u ", total_count);
    }
    fclose(fp_dest);
    printf("Appended %u polygons from %s to %s.\n", src_count-present, src_filename, dest_filename);
}

// task в)
void display_all_polygons(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
        printf("Error: File %s must have .bin or .txt extension.\n", filename);
        return;
    }

    FILE *fp = fopen(filename, is_binary ? "rb" : "r");
    if (!fp) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    NTYPE count;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1 || count == 0) {
            printf("File is empty or corrupted.\n");
            fclose(fp);
            return;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1 || count == 0) {
            printf("File is empty or corrupted.\n");
            fclose(fp);
            return;
        }
    }

    printf("=== Content of '%s' (Total: %u) ===\n", filename, count);
    for (NTYPE i = 0; i < count; i++) {
        Polygone p;
        if (is_binary ? read_one_polygone(fp, &p) : read_one_polygone_from_text_file(fp, &p)) {
            printf("\n--- Polygon Index %u ---\n", i);
            printf("Vertices: %u, Area: %.2f, Perimeter: %.2f, Convex: %s\n", 
                   p.n, area_polygone(&p), perimeter_polygone(&p), is_convex(&p) ? "Yes" : "No");
            free_polygone(&p);
        } else {
            printf("Error reading polygon %u from %s.\n", i, filename);
            free_polygone(&p);
            fclose(fp);
            return;
        }
    }
    printf("==================================\n");
    fclose(fp);
}

// task г)
void display_polygone_by_index(const char *filename, NTYPE index)
{
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
        printf("Error: File %s must have .bin or .txt extension.\n", filename);
        return;
    }

    FILE *fp = fopen(filename, is_binary ? "rb" : "r");
    if (!fp) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    NTYPE count;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1) {
            printf("File is empty or corrupted.\n");
            fclose(fp);
            return;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1) {
            printf("File is empty or corrupted.\n");
            fclose(fp);
            return;
        }
    }

    if (index >= count) {
        printf("Error: Index %u is out of bounds (0-%u).\n", index, count - 1);
        fclose(fp);
        return;
    }

    if (is_binary) {
        for (NTYPE i = 0; i < index; i++) {
            NTYPE num_vertices;
            if (fread(&num_vertices, sizeof(NTYPE), 1, fp) != 1) {
                printf("Error reading polygon %u from %s.\n", i, filename);
                fclose(fp);
                return;
            }
            fseek(fp, num_vertices * sizeof(TPoint), SEEK_CUR);
        }
    } else {
        for (NTYPE i = 0; i < index; i++) {
            NTYPE num_vertices;
            if (fscanf(fp, "%u", &num_vertices) != 1) {
                printf("Error reading polygon %u from %s.\n", i, filename);
                fclose(fp);
                return;
            }
            for (NTYPE j = 0; j < num_vertices; j++) {
                float x, y;
                if (fscanf(fp, "%f %f", &x, &y) != 2) {
                    printf("Error reading vertex %u of polygon %u from %s.\n", j+1, i, filename);
                    fclose(fp);
                    return;
                }
            }
        }
    }

    Polygone p;
    if (is_binary ? read_one_polygone(fp, &p) : read_one_polygone_from_text_file(fp, &p)) {
        printf("--- Polygon Index %u ---\n", index);
        printf("Vertices: %u\n", p.n);
        for (NTYPE i = 0; i < p.n; i++)
            printf("  (%.2f, %.2f)\n", p.vertice[i].x, p.vertice[i].y);
        free_polygone(&p);
    } else {
        printf("Error reading polygon %u from %s.\n", index, filename);
    }

    fclose(fp);
}

// task д)
int delete_polygone_by_index(const char *filename, NTYPE index)
{
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
//printf("Error: File %s must have .bin or .txt extension.\n", filename);
        return FALSE;
    }

    FILE *fp_orig = fopen(filename, is_binary ? "rb" : "r");
    if (!fp_orig) {
//printf("Error: Cannot open file %s\n", filename);
        return FALSE;
    }

    NTYPE count;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp_orig) != 1) {
            fclose(fp_orig);
            return FALSE;
        }
    } else {
        if (fscanf(fp_orig, "%u", &count) != 1) {
            fclose(fp_orig);
            return FALSE;
        }
    }

    if (index >= count) {
//printf("Error: Index %u is out of bounds (0-%u).\n", index, count - 1);
        fclose(fp_orig);
        return FALSE;
    }

    const char *temp_filename = is_binary ? "temp.bin" : "temp.txt";
    FILE *fp_temp = fopen(temp_filename, is_binary ? "wb" : "w");
    if (!fp_temp) {
//printf("Error: Cannot create temporary file %s\n", temp_filename);
        fclose(fp_orig);
        return FALSE;
    }

    NTYPE new_count = count - 1;
    if (is_binary) {
        if (fwrite(&new_count, sizeof(NTYPE), 1, fp_temp) != 1) {
            fclose(fp_orig);
            fclose(fp_temp);
            return FALSE;
        }
    } else {
        if (fprintf(fp_temp, "%10u ", new_count) < 0) {
            fclose(fp_orig);
            fclose(fp_temp);
            return FALSE;
        }
    }

    if (is_binary) {
        for (NTYPE i = 0; i < count; i++) {
            Polygone p;
            if (read_one_polygone(fp_orig, &p)) {
                if (i != index) {
                    if (!write_one_polygone(fp_temp, &p)) {
                        free_polygone(&p);
                        fclose(fp_orig);
                        fclose(fp_temp);
                        return FALSE;
                    }
                }
                free_polygone(&p);
            } else {
                fclose(fp_orig);
                fclose(fp_temp);
                return FALSE;
            }
        }
    } else {
        for (NTYPE i = 0; i < count; i++) {
            Polygone p;
            if (read_one_polygone_from_text_file(fp_orig, &p)) {
                if (i != index) {
                    if (!write_one_polygone_to_text_file(fp_temp, &p)) {
                        free_polygone(&p);
                        fclose(fp_orig);
                        fclose(fp_temp);
                        return FALSE;
                    }
                }
                free_polygone(&p);
            } else {
                fclose(fp_orig);
                fclose(fp_temp);
                return FALSE;
            }
        }
    }

    fclose(fp_orig);
    fclose(fp_temp);
    remove(filename);
    rename(temp_filename, filename);
    return TRUE;
}

// task е)
int is_present_in_file(const char *filename, const Polygone *p)
{
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
//printf("Error: File %s must have .bin or .txt extension.\n", filename);
        return FALSE;
    }

    FILE *fp = fopen(filename, is_binary ? "rb" : "r");
    if (!fp) {
//printf("Error: Cannot open file %s\n", filename);
        return FALSE;
    }

    NTYPE count;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1) {
            fclose(fp);
            return FALSE;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1) {
            fclose(fp);
            return FALSE;
        }
    }

    int found = FALSE;
    for (NTYPE i = 0; i < count; i++) {
        Polygone temp;
        if (is_binary ? read_one_polygone(fp, &temp) : read_one_polygone_from_text_file(fp, &temp)) {
            if (isEqualPolygone(p, &temp)) {
                found = TRUE;
            }
            free_polygone(&temp);
            if (found) break;
        } else {
            free_polygone(&temp);
            fclose(fp);
            return FALSE;
        }
    }

    fclose(fp);
    return found;
}

// task є)
int find_max_perimeter_polygone(const char *filename, Polygone *result)
{
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
//printf("Error: File %s must have .bin or .txt extension.\n", filename);
        return FALSE;
    }

    FILE *fp = fopen(filename, is_binary ? "rb" : "r");
    if (!fp) {
//printf("Error: Cannot open file %s\n", filename);
        return FALSE;
    }

    NTYPE count;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1 || count == 0) {
            fclose(fp);
            return FALSE;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1 || count == 0) {
            fclose(fp);
            return FALSE;
        }
    }

    if (!(is_binary ? read_one_polygone(fp, result) : read_one_polygone_from_text_file(fp, result))) {
        fclose(fp);
        return FALSE;
    }
    PTYPE max_perimeter = perimeter_polygone(result);

    // Compare remaining polygons
    for (NTYPE i = 1; i < count; i++) {
        Polygone temp;
        if (is_binary ? read_one_polygone(fp, &temp) : read_one_polygone_from_text_file(fp, &temp)) {
            PTYPE current_perimeter = perimeter_polygone(&temp);
            if (current_perimeter > max_perimeter) {
                max_perimeter = current_perimeter;
                free_polygone(result);
                *result = temp;
            } else {
                free_polygone(&temp);
            }
        } else {
            free_polygone(&temp);
            fclose(fp);
            free_polygone(result);
            return FALSE;
        }
    }

    fclose(fp);
    return TRUE;
}

// task ж)
int find_min_area_polygone(const char *filename, Polygone *result)
{
    const char *ext = strrchr(filename, '.');
    int is_binary = ext && strcmp(ext, ".bin") == 0;
    int is_text = ext && strcmp(ext, ".txt") == 0;
    if (!is_binary && !is_text) {
//printf("Error: File %s must have .bin or .txt extension.\n", filename);
        return FALSE;
    }

    FILE *fp = fopen(filename, is_binary ? "rb" : "r");
    if (!fp) {
//printf("Error: Cannot open file %s\n", filename);
        return FALSE;
    }

    NTYPE count;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1 || count == 0) {
            fclose(fp);
//printf("File is empty or corrupted.\n");
            return FALSE;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1 || count == 0) {
            fclose(fp);
//printf("File is empty or corrupted.\n");
            return FALSE;
        }
    }

    if (!(is_binary ? read_one_polygone(fp, result) : read_one_polygone_from_text_file(fp, result))) {
        fclose(fp);
//printf("Error reading first polygon from %s.\n", filename);
        return FALSE;
    }
    PTYPE min_area = area_polygone(result);

    for (NTYPE i = 1; i < count; i++) {
        Polygone temp;
        if (is_binary ? read_one_polygone(fp, &temp) : read_one_polygone_from_text_file(fp, &temp)) {
            PTYPE current_area = area_polygone(&temp);
            if (current_area < min_area) {
                min_area = current_area;
                free_polygone(result);
                *result = temp;
            } else {
                free_polygone(&temp);
            }
        } else {
            free_polygone(&temp);
            fclose(fp);
            free_polygone(result);
//printf("Error reading polygon %u from %s.\n", i, filename);
            return FALSE;
        }
    }

    fclose(fp);
    return TRUE;
}

// task з)
NTYPE count_convex_polygons(const char *filename)
{
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

    NTYPE count, convex_count = 0;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1) {
            fclose(fp);
            return 0;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1) {
            fclose(fp);
            return 0;
        }
    }

    for (NTYPE i = 0; i < count; i++) {
        Polygone temp;
        if (is_binary ? read_one_polygone(fp, &temp) : read_one_polygone_from_text_file(fp, &temp)) {
            if (is_convex(&temp)) convex_count++;
            free_polygone(&temp);
        } else {
            free_polygone(&temp);
            fclose(fp);
            return convex_count;
        }
    }

    fclose(fp);
    return convex_count;
}

// task і)
NTYPE count_polygons_containing_point(const char *filename, TPoint p)
{
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

    NTYPE count, containing_count = 0;
    if (is_binary) {
        if (fread(&count, sizeof(NTYPE), 1, fp) != 1) {
            fclose(fp);
            return 0;
        }
    } else {
        if (fscanf(fp, "%u", &count) != 1) {
            fclose(fp);
            return 0;
        }
    }

    for (NTYPE i = 0; i < count; i++) {
        Polygone temp;
        if (is_binary ? read_one_polygone(fp, &temp) : read_one_polygone_from_text_file(fp, &temp)) {
            if (is_point_inside(&temp, p)) containing_count++;
            free_polygone(&temp);
        } else {
            free_polygone(&temp);
            fclose(fp);
            return containing_count;
        }
    }

    fclose(fp);
    return containing_count;
}

// task й)
void filter_polygons(const char *file_a, const char *file_b, int (*predicate)(const Polygone *))
{
    if(!is_valid(file_a)){
        printf("%s is corrupted or contains invalid polygons.\n", file_a);
        return;
    }
    
    const char *ext_a = strrchr(file_a, '.');
    int a_is_binary = ext_a && strcmp(ext_a, ".bin") == 0;
    int a_is_text = ext_a && strcmp(ext_a, ".txt") == 0;
    if (!a_is_binary && !a_is_text) {
//printf("Error: Input file %s must have .bin or .txt extension.\n", file_a);
        return;
    }
    const char *ext_b = strrchr(file_b, '.');
    int b_is_binary = ext_b && strcmp(ext_b, ".bin") == 0;
    int b_is_text = ext_b && strcmp(ext_b, ".txt") == 0;
    if (!b_is_binary && !b_is_text) {
//printf("Error: Output file %s must have .bin or .txt extension.\n", file_b);
        return;
    }

    FILE *fp_a = fopen(file_a, a_is_binary ? "rb" : "r");
    FILE *fp_b = fopen(file_b, b_is_binary ? "wb" : "w");
    if (!fp_a || !fp_b) {
//printf("Error opening files for filtering.\n");
        if (fp_a) fclose(fp_a);
        if (fp_b) fclose(fp_b);
        return;
    }

    NTYPE count_a, count_b = 0;
    if (a_is_binary) {
        fread(&count_a, sizeof(NTYPE), 1, fp_a);
    } else {
        fscanf(fp_a, "%u", &count_a);
    }
    if (b_is_binary) {
        fseek(fp_b, sizeof(NTYPE), SEEK_SET);
    } else {
        fprintf(fp_b, "%10u ", 0);
    }

    for (NTYPE i = 0; i < count_a; i++) {
        Polygone p;
        if (a_is_binary ? read_one_polygone(fp_a, &p) : read_one_polygone_from_text_file(fp_a, &p)) {
            if (predicate(&p)) {
                if (b_is_binary) {
                    if (!write_one_polygone(fp_b, &p)) {
                        free_polygone(&p);
                        fclose(fp_a);
                        fclose(fp_b);
                        return;
                    }
                } else {
                    if (!write_one_polygone_to_text_file(fp_b, &p)) {
                        free_polygone(&p);
                        fclose(fp_a);
                        fclose(fp_b);
                        return;
                    }
                }
                count_b++;
            }
            free_polygone(&p);
        }
    }

    if (b_is_binary) {
        rewind(fp_b);
        fwrite(&count_b, sizeof(NTYPE), 1, fp_b);
    } else {
        rewind(fp_b);
        fprintf(fp_b, "%10u ", count_b);
    }

    fclose(fp_a);
    fclose(fp_b);
    printf("Filtered %u polygons into file %s\n", count_b, file_b);
}

// task к)
PTYPE perimeter_polygone(const Polygone *p)
{
    PTYPE perimeter = 0.0;
    for (NTYPE i = 0; i < p->n; i++)
    {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        perimeter += sqrtf(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2));
    }
    return perimeter;
}

PTYPE area_polygone(const Polygone *p)
{
    if (p->n < 3)
        return 0.0;
    PTYPE area = 0.0f;
    for (NTYPE i = 0; i < p->n; i++)
    {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return fabsf(area) / 2.0f;
}

int is_convex(const Polygone *p)
{
    if (p->n < 3)
        return FALSE;
    int sign = 0;
    for (NTYPE i = 0; i < p->n; i++)
    {
        TPoint p1 = p->vertice[i];
        TPoint p2 = p->vertice[(i + 1) % p->n];
        TPoint p3 = p->vertice[(i + 2) % p->n];
        PTYPE cross_product = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
        if (fabsf(cross_product) > EPSILON)
        {
            int current_sign = (cross_product > 0) ? 1 : -1;
            if (sign == 0)
                sign = current_sign;
            else if (sign != current_sign)
                return FALSE;
        }
    }
    return TRUE;
}

int is_point_inside(const Polygone *p, TPoint point)
{
    if (p->n < 3)
        return FALSE;
    PTYPE total_area = area_polygone(p);
    PTYPE sum_tri_area = 0.0f;

    for (NTYPE i = 0; i < p->n; i++)
    {
        sum_tri_area += area_from_points(point, p->vertice[i], p->vertice[(i + 1) % p->n]);
    }

    return fabsf(total_area - sum_tri_area) < EPSILON;
}

// task к)
int isEqualPoint(TPoint a, TPoint b)
{
    return fabsf(a.x - b.x) < EPSILON && fabsf(a.y - b.y) < EPSILON;
}

int isEqualPolygone(const Polygone *p1, const Polygone *p2)
{
    if (p1->n != p2->n)
        return FALSE;
    NTYPE n = p1->n;
    for (NTYPE start_node = 0; start_node < n; start_node++)
    {
        if (isEqualPoint(p1->vertice[0], p2->vertice[start_node]))
        {
            int same_dir = TRUE;
            for (NTYPE i = 1; i < n; i++)
            {
                if (!isEqualPoint(p1->vertice[i], p2->vertice[(start_node + i) % n]))
                {
                    same_dir = FALSE;
                    break;
                }
            }
            if (same_dir)
                return TRUE;

            int opposite_dir = TRUE;
            for (NTYPE i = 1; i < n; i++)
            {
                if (!isEqualPoint(p1->vertice[i], p2->vertice[(start_node - i + n) % n]))
                {
                    opposite_dir = FALSE;
                    break;
                }
            }
            if (opposite_dir)
                return TRUE;
        }
    }
    return FALSE;
}
