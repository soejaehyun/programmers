#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TERMS_PER_TYPE_LEN  26

static void init_terms_per_type(const char *terms[], size_t terms_len, int terms_per_type[]) {
    for (size_t i = 0; i < terms_len; i++) {
        char *str = malloc(strlen(terms[i]) + 1);
        strncpy(str, terms[i], strlen(terms[i]));
        str[strlen(terms[i])] = '\0';

        char *tokens[2];
        int tokens_i[2];

        tokens[0] = strtok(str, " ");
        tokens[1] = strtok(NULL, " ");

        tokens_i[0] = tokens[0][0];
        tokens_i[1] = atoi(tokens[1]);
        printf("%s %s ==> %d %d\n", tokens[0], tokens[1], tokens_i[0], tokens_i[1]);

        terms_per_type[tokens_i[0]-'A'] = tokens_i[1];
        free(str);
    }
}

static void print_terms_per_type(int terms_per_type[], int terms_per_type_len) {
    printf("====================\n");
    for (int i = 0; i < terms_per_type_len; i++) {
        if (!terms_per_type[i]) continue;
        printf("[%d] %c --> %d\n", i, i + 'A', terms_per_type[i]);
    }
    printf("====================\n");
}

static int check_discard_personal_info(const char* today, const char* privacies, int terms_per_type[], int terms_type) {
    char term_day[16] = {0,};
    int year, mon, day;
    int term;
    int retval = -1;

    sscanf(privacies, "%04d.%02d.%02d", &year, &mon, &day);
    printf("%04d.%02d.%02d\n", year, mon, day);

    term = terms_per_type[terms_type - 'A'];
    
    if ((mon + term) % 12 == 0) {
        year += (mon + term) / 12 - 1;
        mon = 12;
    } else {
        year += (mon + term) / 12;
        mon = (mon + term) % 12;
    }
    //if (mon == 0) mon = 1;

    snprintf(term_day, sizeof(term_day) - 1, "%04d.%02d.%02d", year, mon, day);
    //printf("%s\n", term_day);

    retval = strncmp(today, term_day, strlen(today));
    printf("%s ? %s ==> %d\n", today, term_day, retval);

    if (retval >= 0) return 1;
    else return 0;
}

int main(void) {
    //const char *today = "2022.05.19";
    //const char *terms[] = {"A 6", "B 12", "C 3"};
    //const char *privacies[] = {"2021.05.02 A", "2021.07.01 B", "2022.02.19 C", "2022.02.20 C"};
    /* result : [1, 3] */
    const char *today = "2020.01.01";
    const char *terms[] = {"Z 3", "D 5"};
    const char *privacies[] = {"2019.01.01 D", "2019.11.15 Z", "2019.08.02 D", "2019.07.01 D", "2018.12.28 Z"};
    /* result : [1, 4, 5] */
    size_t terms_len = sizeof(terms) / sizeof(const char *);
    size_t privacies_len = sizeof(privacies) / sizeof(const char *);

    int terms_per_type[TERMS_PER_TYPE_LEN] = {0,};

    init_terms_per_type(terms, terms_len, terms_per_type);
    print_terms_per_type(terms_per_type, TERMS_PER_TYPE_LEN);

    for (size_t i = 0; i < privacies_len; i++) {
        char *str = malloc(strlen(privacies[i]) + 1);
        strncpy(str, privacies[i], strlen(privacies[i]));
        str[strlen(privacies[i])] = '\0';

        char *tokens[2];
        int term_type;

        tokens[0] = strtok(str, " ");
        tokens[1] = strtok(NULL, " ");

        term_type = tokens[1][0];
        if (check_discard_personal_info(today, tokens[0], terms_per_type, term_type) > 0) {
            printf("%d discard !!!!!!!1 \n", i+1);
        }
    }
    /*
    char *tokens[2];
    tokens[0] = strtok(terms, " ");
    tokens[1] = strtok(NULL, " ");
    */

    return 0;
}