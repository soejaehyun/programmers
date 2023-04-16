#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
4	5	[1, 0, 3, 1, 2]	[0, 3, 0, 4, 0]	16
2	7	[1, 0, 2, 0, 1, 0, 2]	[0, 2, 0, 1, 0, 2, 0]	30
*/

int main(void) {
    //int cap = 4, n = 5;
    //int deliveries[] = {1, 0, 3, 1, 2};
    //int pickups[] = {0, 3, 0, 4, 0};
    int cap = 2, n = 7;
    int deliveries[] = {1, 0, 2, 0, 1, 0, 2};
    int pickups[] = {0, 2, 0, 1, 0, 2, 0};
    size_t deliveries_len = sizeof(deliveries) / sizeof(int);
    size_t pickups_len = sizeof(pickups) / sizeof(int);
    int retval = 0;

    int delivery_num = 0, pickup_num = 0;

    for (int i = n - 1; i >= 0; i--) {
        delivery_num += deliveries[i];
        pickup_num += pickups[i];

        while (delivery_num > 0 || pickup_num > 0) {
            retval += (i + 1) * 2;
            printf("%d %d %d %d\n", i, delivery_num, pickup_num, retval);
            delivery_num -= cap;
            pickup_num -= cap;
        }
    }
    printf("%d\n", retval);
    return 0;
}