#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void) {
    //const char *wallpaper[] = {".#...", "..#..", "...#."};
    //const char *wallpaper[] = {"..........", ".....#....", "......##..", "...##.....", "....#....."};
    //const char *wallpaper[] = {".##...##.", "#..#.#..#", "#...#...#", ".#.....#.", "..#...#..", "...#.#...", "....#...."};
    const char *wallpaper[] = {"..", "#."};
    size_t wallpaper_len = sizeof(wallpaper) / sizeof(const char *);

    /*printf("%d\n", wallpaper_len);
    for (size_t i = 0; i < wallpaper_len; i++) {
        printf("%s\n", wallpaper[i]);
    }*/

    int lux, luy, rdx, rdy;
    lux = luy = rdx = rdy = -1;

    for (size_t x = 0; x < wallpaper_len; x++) {
        const char *str = wallpaper[x];
        size_t str_len = strlen(str);
        for (size_t y = 0; y < str_len; y++) {
            if (str[y] == '#') {
                // 파일이 제일 처음에 있는 x 위치
                if (lux < 0) lux = x;
                // luy가 음수이면 y값을 넣어두고 후에 비교 후
                // y 좌표보다 크면 y 좌표를 다시 넣어줌
                if (luy < 0) luy = y;
                else if (luy > y) luy = y;

                if (rdx < 0) rdx = x;
                else if (rdx < x) rdx = x;

                if (rdy < 0) rdy = y;
                else if (rdy < y) rdy = y;
            }
            //printf("%c", str[y]);
        }
        //printf("\n");
    }

    printf("start (%d, %d)\nend   (%d, %d)", lux, luy, rdx+1, rdy+1);

    return 0;
}