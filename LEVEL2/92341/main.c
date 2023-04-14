#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
[180, 5000, 10, 600]	["05:34 5961 IN", "06:00 0000 IN", "06:34 0000 OUT", "07:59 5961 OUT", "07:59 0148 IN", "18:59 0000 IN", "19:09 0148 OUT", "22:59 5961 IN", "23:00 5961 OUT"]	[14600, 34400, 5000]
[120, 0, 60, 591]	["16:00 3961 IN","16:00 0202 IN","18:00 3961 OUT","18:00 0202 OUT","23:58 3961 IN"]	[0, 591]
[1, 461, 1, 10]	["00:00 1234 IN"]	[14841]
*/
typedef struct __fees_t {
    int base_rate;
    int base_time;
    int unit_rate;
    int unit_time;
} fees_t;

static fees_t* init_fees(int fees[], size_t fees_len) {
    fees_t *new;
    
    if (fees_len != 4) return NULL;
    
    new = (fees_t *)malloc(sizeof(fees_t));
    if (!new) return NULL;

    new->base_time = fees[0];
    new->base_rate = fees[1];
    new->unit_time = fees[2];
    new->unit_rate = fees[3];

    return new;
}

static void deinit_fees(fees_t *fee_t) {
    if (fee_t) free(fee_t);
}

static void printf_fees(fees_t *fee_t) {
    printf("기본 시간(분) : %d\n", fee_t->base_time);
    printf("기본 요금(원) : %d\n", fee_t->base_rate);
    printf("단위 시간(분) : %d\n", fee_t->unit_time);
    printf("단위 요금(원) : %d\n", fee_t->unit_rate);
}

typedef struct __record_t {
    struct __record_t *next;
    char *time;
    int is_in;
} record_t;

typedef struct __records_t {
    struct __records_t *next;

    char *car_num;
    record_t *rc;
} records_t;

static record_t *create_record(char *time, char *is_in) {
    record_t *rc;

    rc = (record_t *)malloc(sizeof(record_t));
    rc->next = NULL;
    rc->time = strdup(time);
    if (strncmp(is_in, "IN", 2) == 0) rc->is_in = 1;
    else if (strncmp(is_in, "OUT", 3) == 0) rc->is_in = 0;
    return rc;
}

static void print_record(record_t *rc) {
    for (record_t *tmp = rc; tmp; tmp = tmp->next) {
        printf("time : %s (%s)\n", tmp->time, tmp->is_in ? "IN" : "OUT");
    }
}

static void destroy_record(record_t *rc) {
    record_t *tmp = rc;
    while (tmp) {
        record_t *del = tmp;
        tmp = tmp->next;
        if (del) {
            if (del->time) free(del->time);
            free(del);
        }
    }
}

static records_t *create_records_from_car_num(char *cnum) {
    records_t *rcs;
    
    rcs = (records_t *)malloc(sizeof(records_t));
    rcs->next = NULL;
    rcs->car_num = strdup(cnum);
    rcs->rc = NULL;
    return rcs;
}

static records_t *create_records_from_car_num_with_record(char *cnum, char *time, char *is_in) {
    records_t *rcs;
    
    rcs = (records_t *)malloc(sizeof(records_t));
    rcs->next = NULL;
    rcs->car_num = strdup(cnum);
    rcs->rc = create_record(time, is_in);
    return rcs;
}

static void print_records(records_t *rcs) {
    for (records_t *tmp = rcs; tmp; tmp = tmp->next) {
        printf("car_num : %s\n", tmp->car_num);
    }
}

static void print_records_with_record(records_t *rcs) {
    for (records_t *tmp = rcs; tmp; tmp = tmp->next) {
        printf("car_num : %s\n", tmp->car_num);
        print_record(tmp->rc);
    }
}

static void destroy_records_with_record(records_t *rcs) {
    records_t *tmp = rcs;
    while (tmp) {
        records_t *del = tmp;
        tmp = tmp->next;
        if (del) {
            if (del->car_num) free(del->car_num);
            destroy_record(del->rc);
            free(del);
        }
    }
}

typedef struct __retval_t {
    int cnum;
    int tot_fee;
    int tot_time;
} retval_t;

static int compare(const void *a, const void *b) {
    int num1 = ((retval_t *)a)->cnum;    // void 포인터를 int 포인터로 변환한 뒤 역참조하여 값을 가져옴
    int num2 = ((retval_t *)b)->cnum;    // void 포인터를 int 포인터로 변환한 뒤 역참조하여 값을 가져옴

    if (num1 < num2)    // a가 b보다 작을 때는
        return -1;      // -1 반환

    if (num1 > num2)    // a가 b보다 클 때는
        return 1;       // 1 반환

    return 0;    // a와 b가 같을 때는 0 반환
}

static void process_records_calc_fees(records_t *rcs, fees_t *ft, int rcs_cnt) {
    retval_t *ret = (retval_t *)malloc(rcs_cnt * sizeof(retval_t));
    int ret_cnt = 0;
    for (records_t *rcs_tmp = rcs; rcs_tmp; rcs_tmp = rcs_tmp->next) {
        int stime, etime;
        int tot_fee = 0;
        int tot_time = 0;
        stime = etime = -1;
        for (record_t *rc_tmp = rcs_tmp->rc; rc_tmp; rc_tmp = rc_tmp->next) {
            char tmp[32] = {0,};
            char *tokens[2];

            snprintf(tmp, sizeof(tmp) - 1, "%s", rc_tmp->time);
            
            tokens[0] = strtok(tmp, ":");
            tokens[1] = strtok(NULL, ":");

            if (rc_tmp->is_in) stime = atoi(tokens[0]) * 60 + atoi(tokens[1]);
            else etime = atoi(tokens[0]) * 60 + atoi(tokens[1]);

            if (rc_tmp->next == NULL && etime < 0) etime = 23 * 60 + 59;

            if (!(stime < 0) && !(etime < 0)) {
                tot_time += etime - stime;
                stime = etime = -1;
            }

            if (rc_tmp->next == NULL) {
                tot_fee += ft->base_rate;
                if (ft->base_time < tot_time) { 
                    tot_fee += ((tot_time - ft->base_time) / ft->unit_time) * ft->unit_rate;
                    if ((tot_time - ft->base_time) % ft->unit_time) tot_fee += ft->unit_rate;
                }
            }
        }
        ret[ret_cnt].cnum = atoi(rcs_tmp->car_num);
        ret[ret_cnt].tot_time = tot_time;
        ret[ret_cnt].tot_fee = tot_fee;
        ret_cnt++;
        //printf("%s: %d (%d)\n", rcs_tmp->car_num, tot_time, tot_fee);
    } 
    qsort(ret, rcs_cnt, sizeof(retval_t), compare);
    for (int i = 0; i < rcs_cnt; i++) {
        printf("%04d -> %d ( %d )\n", ret[i].cnum, ret[i].tot_time, ret[i].tot_fee);
    }
}

int main(void) {
    int fees[] = {180, 5000, 10, 600};
    const char *records[] = {"05:34 5961 IN", "06:00 0000 IN", "06:34 0000 OUT", "07:59 5961 OUT", "07:59 0148 IN", "18:59 0000 IN", "19:09 0148 OUT", "22:59 5961 IN", "23:00 5961 OUT"};
    //int fees[] = {120, 0, 60, 591};
    //const char *records[] = {"16:00 3961 IN","16:00 0202 IN","18:00 3961 OUT","18:00 0202 OUT","23:58 3961 IN"};
    size_t fees_len = sizeof(fees) / sizeof(int);
    size_t records_len = sizeof(records) / sizeof(const char *);

    records_t *rcs = NULL;
    int rcs_cnt = 0;
    fees_t *ft = init_fees(fees, fees_len);
    if (!ft) return 0;

    printf_fees(ft);

    for (size_t i = 0; i < records_len; i++) {
        const char *str = records[i];
        char tmp[32];
        char *tokens[3];

        snprintf(tmp, sizeof(tmp) - 1, "%s", str);

        tokens[0] = strtok(tmp, " ");
        tokens[1] = strtok(NULL, " ");
        tokens[2] = strtok(NULL, " ");

        //printf("%s %s %s\n", tokens[0], tokens[1], tokens[2]);
        if (!rcs) {
            rcs = create_records_from_car_num_with_record(tokens[1], tokens[0], tokens[2]);
            rcs_cnt++;
        }
        else {
            for (records_t *rcs_tmp = rcs; rcs_tmp; rcs_tmp = rcs_tmp->next) {
                if (strncmp(rcs_tmp->car_num, tokens[1], strlen(tokens[1])) == 0) {
                    for (record_t *rc_tmp = rcs_tmp->rc; rc_tmp; rc_tmp = rc_tmp->next) {
                        if (rc_tmp->next == NULL) {
                            rc_tmp->next = create_record(tokens[0], tokens[2]);
                            break;
                        }
                    }
                    break;
                }
                if (rcs_tmp->next == NULL) {
                    rcs_tmp->next = create_records_from_car_num_with_record(tokens[1], tokens[0], tokens[2]);
                    rcs_cnt++;
                    break;
                }
            }
        }
    }
    print_records_with_record(rcs);

    process_records_calc_fees(rcs, ft, rcs_cnt);

    deinit_fees(ft);
    destroy_records_with_record(rcs);

    return 0;
}