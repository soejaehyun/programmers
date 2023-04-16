#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gcd(int a, int b) {
    int num1, num2;
    
    if (a > b) { num1 = a; num2 = b; }
    else { num1 = b; num2 = a; }

    if (num2 == 0) return num1;
    return gcd(num2, num1 % num2);
}
int main(void) {
    int w = 1, h = 1;
    int answer;

    answer = (long long)w * (long long)h - (w + h - gcd(w, h));
    printf("%d\n", answer);
    return 0;
}