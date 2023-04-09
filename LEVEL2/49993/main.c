#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* "CBD"	["BACDE", "CBADF", "AECB", "BDA"]	2 */

#define SKILL_PRIORITY_TABLE_CNT 26

static void init_skill_priority_for_skill(const char *skill, int skill_priority[]) {
    char old_skill = -1;
    for (const char *sp = skill; *sp; sp++) {
        char cur_skill = *sp;

        skill_priority[cur_skill - 'A'] = old_skill;
        old_skill = cur_skill - 'A';
    }
}

static void print_skill_priority(int skill_priority[]) {
    for (int i = 0; i < SKILL_PRIORITY_TABLE_CNT; i++) {
        if (skill_priority[i] < 0) continue;
        printf("[%d] %c ==> %d(%c)\n", i, i + 'A', skill_priority[i], skill_priority[i] + 'A');
    }
}

int main(void) {
    const char *skill = "CBD";
    const char *skill_trees[] = {"BACDE", "CBADF", "AECB", "BDA"};
    size_t skill_trees_len = sizeof(skill_trees) / sizeof(const char *);

    int skill_priority[SKILL_PRIORITY_TABLE_CNT];
    memset(skill_priority, -1, sizeof(skill_priority));
    int retval = 0;

    init_skill_priority_for_skill(skill, skill_priority);
    print_skill_priority(skill_priority);

    for (size_t i = 0; i < skill_trees_len; i++) {
        const char *str = skill_trees[i];
        int skill_learning[SKILL_PRIORITY_TABLE_CNT] = {0,};
        for (const char *sp = str; *sp; sp++) {
            char c = *sp;

            if (skill_priority[c - 'A'] < 0) skill_learning[c - 'A'] = 1;
            else {
                if (skill_learning[skill_priority[c - 'A']]) skill_learning[c - 'A'] = 1;
                else break;
            }
            if (*(sp+1) == '\0') {
                retval++;
                printf("%s ==> %d\n", str, true);
            }
        }
    }
    printf("retval = %d\n", retval);

    return 0;
}