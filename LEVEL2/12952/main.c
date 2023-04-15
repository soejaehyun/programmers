#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int check_queen(int raw, int *nqueen) {
    for (int i = 0; i < raw; i++) {
        if (nqueen[i] == nqueen[raw]) return 0; /* 같은 열에 위치했는지를 체크 */
        if (abs(nqueen[i] - nqueen[raw]) == abs(i-raw)) return 0;
        /* 대각선에 있는지를 체크 -> 행의 차이와 열의 차이가 같으면 대각선에 위치한다고 판단함 */
    }
    return 1;
}

static void queen(int raw, int *nqueen, int n, int *retval) {
    if (raw == n) {
        (*retval)++;
        return;
    }
    for (int col = 0; col < n; col++) {
        nqueen[raw] = col;
        if (check_queen(raw, nqueen)) queen(raw + 1, nqueen, n, retval);
    }
}

int main(void) {
    int n = 4;
    int retval = 0;
    int *nqueen = (int *)calloc(1, n * sizeof(int));
    
    queen(0, nqueen, n, &retval);
    printf("%d\n", retval);

    return 0;
}