#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*
"()()"	true
"(())()"	true
")()("	false
"(()("	false
*/

int main(void) {
    bool retval = false;
    //const char *s = "()()";
    //const char *s = "(())()";
    //const char *s = ")()(";
    const char *s = "(()(";
    int op = 0;

    for (const char *sp = s; *sp; sp++) {
        //printf("%c\n", *sp);
        if (*sp == '(') op++;
        else if (*sp == ')') {
            op--;
            if (op < 0) break;
        }
    }

    if (op == 0) retval = true;
    printf("%s ==> %d\n", s, retval);
    return 0;
}