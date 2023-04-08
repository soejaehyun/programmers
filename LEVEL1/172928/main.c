#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void free_ipark(int **ipark, size_t ipark_raw) {
    if (!ipark) return;

    for (size_t i = 0; i < ipark_raw; i++) {
        if (ipark[ipark_raw]) free(ipark[ipark_raw]);
    }
    if (ipark) free(ipark);
}

static int **init_park_to_integer_array_malloc(const char *park[], size_t park_len, int *stx, int *sty) {
    int **ipark = NULL;
    size_t ipark_len;

    if (!park || !park_len) return NULL;

    ipark_len = strlen(park[0]);
    // S O X
    ipark = (int **)malloc(park_len * sizeof(int *));
    if (!ipark) return NULL;

    for (size_t i = 0; i < park_len; i++) {
        ipark[i] = (int *)malloc(ipark_len * sizeof(int));
    }

    for (size_t i = 0; i < park_len; i++) {
        const char *str = park[i];
        size_t str_len = strlen(str);
        for (size_t y = 0; y < str_len; y++) {
            if (str[y] == 'S') {
                ipark[i][y] = 1;
                *stx = i;
                *sty = y;
            }
            else if (str[y] == 'O') ipark[i][y] = 0;
            else if (str[y] == 'X') ipark[i][y] = -1;
        }
    }

    return ipark;
}

static void print_ipark(int **ipark, size_t ipark_raw, size_t ipark_col) {
    if (!ipark) return;

    for (size_t i = 0; i < ipark_raw; i++) {
        for (size_t y = 0; y < ipark_col; y++) {
            printf("%d ", ipark[i][y]);
        }
        printf("\n");
    }
}

int main(void) {
    //const char *park[] = {"SOO","OOO","OOO"};
    //const char *routes[] = {"E 2","S 2","W 1"};
    //const char *park[] = {"SOO","OXX","OOO"};
    //const char *routes[] = {"E 2","S 2","W 1"};
    const char *park[] = {"OSO","OOO","OXO","OOO"};
    const char *routes[] = {"E 2","S 3","W 1"};
    size_t park_len = sizeof(park) / sizeof(const char *);
    size_t routes_len = sizeof(routes) / sizeof(const char *);

    int **ipark;
    int stx, sty;

    printf("%d %d\n", park_len, routes_len);
    ipark = init_park_to_integer_array_malloc(park, park_len, &stx, &sty);
    if (!ipark) return 0;

    print_ipark(ipark, park_len, strlen(park[0]));
    printf("(%d, %d)\n", stx, sty);

    for (size_t i = 0; i < routes_len; i++) {
        char *tokens[2];
        char rest[16];
        char *tmp;
        strncpy(rest, routes[i], strlen(routes[i]));
        rest[strlen(routes[i])] = '\0';
        //printf("%s\n", rest);

        tokens[0] = strtok(rest, " ");
        tokens[1] = strtok(NULL, " ");
        int distance = atoi(tokens[1]);
        printf("%s %d\n", tokens[0], distance);
        //    N
        // W     E
        //    S
        // park_len, strlen(park[0])
        if (!strncmp(tokens[0], "N", 1)) {
            printf("stx: %d, dis: %d, max: %d\n", stx, distance, park_len);
            int tmp = stx;
            if (stx - distance < 0) continue;
            for (int i = stx-1; i >= tmp-distance; i--) {
                if (ipark[i][sty] < 0) break;
                if (i == tmp-distance) stx = i;
            }
        }
        else if (!strncmp(tokens[0], "W", 1)) {
            printf("sty: %d, dis: %d, max: %d\n", sty, distance, strlen(park[0]));
            int tmp = sty;
            if (sty - distance < 0) continue;
            for (int y = sty-1; y >= tmp-distance; y--) {
                if (ipark[stx][y] < 0) break;
                if (y == tmp-distance) sty = y;
            }
        }
        else if (!strncmp(tokens[0], "S", 1)) {
            printf("stx: %d, dis: %d, max: %d\n", stx, distance, park_len);
            int tmp = stx;
            if (stx + distance >= park_len) continue;
            for (int i = stx+1; i <= tmp+distance; i++) {
                if (ipark[i][sty] < 0) break;
                if (i == tmp+distance) stx = i;
            }
        }
        else if (!strncmp(tokens[0], "E", 1)) {
            printf("sty: %d, dis: %d, max: %d\n", sty, distance, strlen(park[0]));
            int tmp = sty;
            if (sty + distance >= strlen(park[0])) continue;
            for (int y = sty+1; y <= tmp+distance; y++) {
                if (ipark[stx][y] < 0) break;
                if (y == tmp+distance) sty = y;
            }
        }

        printf("(%d, %d)\n", stx, sty);

        //printf("%s %s\n", tokens[0], tokens[1]);
    }

    // segfault를 부르는 함수? 프로그래머스 컴파일러에서만 그런거같음..
    free_ipark(ipark, park_len);

    return 0;
}
