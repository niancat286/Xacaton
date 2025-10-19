#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "Polygone.h"

#define EPSILON 1e-6f

static int read_one_polygone(FILE *fp, Polygone *p)
{
    if (fread(&p->n, sizeof(NTYPE), 1, fp) != 1)
        return FALSE;
    p->vertice = (TPoint *)malloc(p->n * sizeof(TPoint));
    if (!p->vertice)
        return FALSE;
    if (fread(p->vertice, sizeof(TPoint), p->n, fp) != p->n)
    {
        free(p->vertice);
        p->vertice = NULL;
        return FALSE;
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

    FILE *fp = fopen(filename, "r+b");
    if (!fp)
    {
        fp = fopen(filename, "w+b");
        if (!fp)
        {
            printf("Error: Cannot open or create file %s\n", filename);
            free_polygone(&p);
            return;
        }
        NTYPE zero = 0;
        fwrite(&zero, sizeof(NTYPE), 1, fp);
    }

    NTYPE count;
    rewind(fp);
    fread(&count, sizeof(NTYPE), 1, fp);

    fseek(fp, 0, SEEK_END);
    write_one_polygone(fp, &p);

    count++;
    rewind(fp);
    fwrite(&count, sizeof(NTYPE), 1, fp);

    fclose(fp);
    free_polygone(&p);
    printf("Polygon added successfully.\n");
}

// task б)
void append_polygons_from_file(const char *dest_filename, const char *src_filename)
{
    FILE *fp_src = fopen(src_filename, "rb");
    if (!fp_src)
    {
        printf("Error: Cannot open source file %s\n", src_filename);
        return;
    }

    NTYPE src_count;
    if (fread(&src_count, sizeof(NTYPE), 1, fp_src) != 1 || src_count == 0)
    {
        printf("Source file %s is empty or corrupted.\n", src_filename);
        fclose(fp_src);
        return;
    }

    FILE *fp_dest = fopen(dest_filename, "r+b");
    if (!fp_dest)
    {
        fp_dest = fopen(dest_filename, "w+b");
        NTYPE zero = 0;
        fwrite(&zero, sizeof(NTYPE), 1, fp_dest);
    }

    NTYPE dest_count;
    rewind(fp_dest);
    fread(&dest_count, sizeof(NTYPE), 1, fp_dest);
    fseek(fp_dest, 0, SEEK_END);

    for (NTYPE i = 0; i < src_count; i++)
    {
        Polygone p;
        if (read_one_polygone(fp_src, &p))
        {
            write_one_polygone(fp_dest, &p);
            free_polygone(&p);
        }
    }
    fclose(fp_src);

    NTYPE total_count = dest_count + src_count;
    rewind(fp_dest);
    fwrite(&total_count, sizeof(NTYPE), 1, fp_dest);
    fclose(fp_dest);
    printf("Appended %u polygons from %s to %s.\n", src_count, src_filename, dest_filename);
}

// task в)
void display_all_polygons(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Cannot open file %s\n", filename);
        return;
    }
    NTYPE count;
    if (fread(&count, sizeof(NTYPE), 1, fp) != 1 || count == 0)
    {
        printf("File is empty or corrupted.\n");
        fclose(fp);
        return;
    }
    printf("=== Content of '%s' (Total: %u) ===\n", filename, count);
    for (NTYPE i = 0; i < count; i++)
    {
        Polygone p;
        if (read_one_polygone(fp, &p))
        {
            printf("\n--- Polygon Index %u ---\n", i);
            printf("Vertices: %u, Area: %.2f, Perimeter: %.2f, Convex: %s\n", p.n, area_polygone(&p), perimeter_polygone(&p), is_convex(&p) ? "Yes" : "No");
            free_polygone(&p);
        }
    }
    printf("==================================\n");
    fclose(fp);
}

// task г)
void display_polygone_by_index(const char *filename, NTYPE index)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Cannot open file %s\n", filename);
        return;
    }
    NTYPE count;
    fread(&count, sizeof(NTYPE), 1, fp);
    if (index >= count)
    {
        printf("Error: Index %u is out of bounds (0-%u).\n", index, count - 1);
        fclose(fp);
        return;
    }
    for (NTYPE i = 0; i < index; i++)
    {
        NTYPE num_vertices;
        fread(&num_vertices, sizeof(NTYPE), 1, fp);
        fseek(fp, num_vertices * sizeof(TPoint), SEEK_CUR);
    }
    Polygone p;
    if (read_one_polygone(fp, &p))
    {
        printf("--- Polygon Index %u ---\n", index);
        printf("Vertices: %u\n", p.n);
        for (NTYPE i = 0; i < p.n; i++)
            printf("  (%.2f, %.2f)\n", p.vertice[i].x, p.vertice[i].y);
        free_polygone(&p);
    }
    fclose(fp);
}

// task д)
int delete_polygone_by_index(const char *filename, NTYPE index)
{
    FILE *fp_orig = fopen(filename, "rb");
    if (!fp_orig)
        return FALSE;
    NTYPE count;
    fread(&count, sizeof(NTYPE), 1, fp_orig);
    if (index >= count)
    {
        fclose(fp_orig);
        return FALSE;
    }

    const char *temp_filename = "temp.bin";
    FILE *fp_temp = fopen(temp_filename, "wb");
    if (!fp_temp)
    {
        fclose(fp_orig);
        return FALSE;
    }

    NTYPE new_count = count - 1;
    fwrite(&new_count, sizeof(NTYPE), 1, fp_temp);
    for (NTYPE i = 0; i < count; i++)
    {
        Polygone p;
        read_one_polygone(fp_orig, &p);
        if (i != index)
        {
            write_one_polygone(fp_temp, &p);
        }
        free_polygone(&p);
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
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return FALSE;
    NTYPE count;
    if (fread(&count, sizeof(NTYPE), 1, fp) != 1)
    {
        fclose(fp);
        return FALSE;
    }

    int found = FALSE;
    for (NTYPE i = 0; i < count; i++)
    {
        Polygone temp;
        if (!read_one_polygone(fp, &temp))
            break;
        if (isEqualPolygone(p, &temp))
        {
            found = TRUE;
        }
        free_polygone(&temp);
        if (found)
            break;
    }
    fclose(fp);
    return found;
}

// task є)
int find_max_perimeter_polygone(const char *filename, Polygone *result)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return FALSE;
    NTYPE count;
    if (fread(&count, sizeof(NTYPE), 1, fp) != 1 || count == 0)
    {
        fclose(fp);
        return FALSE;
    }

    if (!read_one_polygone(fp, result))
    {
        fclose(fp);
        return FALSE;
    }
    PTYPE max_perimeter = perimeter_polygone(result);

    for (NTYPE i = 1; i < count; i++)
    {
        Polygone temp;
        if (!read_one_polygone(fp, &temp))
            break;
        PTYPE current_perimeter = perimeter_polygone(&temp);
        if (current_perimeter > max_perimeter)
        {
            max_perimeter = current_perimeter;
            free_polygone(result);
            *result = temp;
        }
        else
        {
            free_polygone(&temp);
        }
    }
    fclose(fp);
    return TRUE;
}

// task ж)
int find_min_area_polygone(const char *filename, Polygone *result)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return FALSE;
    NTYPE count;
    if (fread(&count, sizeof(NTYPE), 1, fp) != 1 || count == 0)
    {
        fclose(fp);
        return FALSE;
    }

    if (!read_one_polygone(fp, result))
    {
        fclose(fp);
        return FALSE;
    }
    PTYPE min_area = area_polygone(result);

    for (NTYPE i = 1; i < count; i++)
    {
        Polygone temp;
        if (!read_one_polygone(fp, &temp))
            break;
        PTYPE current_area = area_polygone(&temp);
        if (current_area < min_area)
        {
            min_area = current_area;
            free_polygone(result);
            *result = temp;
        }
        else
        {
            free_polygone(&temp);
        }
    }
    fclose(fp);
    return TRUE;
}

// task з)
NTYPE count_convex_polygons(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return 0;
    NTYPE count, convex_count = 0;
    fread(&count, sizeof(NTYPE), 1, fp);
    for (NTYPE i = 0; i < count; i++)
    {
        Polygone temp;
        if (read_one_polygone(fp, &temp))
        {
            if (is_convex(&temp))
                convex_count++;
            free_polygone(&temp);
        }
    }
    fclose(fp);
    return convex_count;
}

// task і)
NTYPE count_polygons_containing_point(const char *filename, TPoint p)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return 0;
    NTYPE count, containing_count = 0;
    fread(&count, sizeof(NTYPE), 1, fp);
    for (NTYPE i = 0; i < count; i++)
    {
        Polygone temp;
        if (read_one_polygone(fp, &temp))
        {
            if (is_point_inside(&temp, p))
                containing_count++;
            free_polygone(&temp);
        }
    }
    fclose(fp);
    return containing_count;
}

// task й)
void filter_polygons(const char *file_a, const char *file_b, int (*predicate)(const Polygone *))
{
    FILE *fp_a = fopen(file_a, "rb"), *fp_b = fopen(file_b, "wb");
    if (!fp_a || !fp_b)
    {
        printf("Error opening files for filtering.\n");
        if (fp_a)
            fclose(fp_a);
        if (fp_b)
            fclose(fp_b);
        return;
    }

    NTYPE count_a, count_b = 0;
    fread(&count_a, sizeof(NTYPE), 1, fp_a);
    fseek(fp_b, sizeof(NTYPE), SEEK_SET);
    for (NTYPE i = 0; i < count_a; i++)
    {
        Polygone p;
        if (read_one_polygone(fp_a, &p))
        {
            if (predicate(&p))
            {
                write_one_polygone(fp_b, &p);
                count_b++;
            }
            free_polygone(&p);
        }
    }
    rewind(fp_b);
    fwrite(&count_b, sizeof(NTYPE), 1, fp_b);
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
