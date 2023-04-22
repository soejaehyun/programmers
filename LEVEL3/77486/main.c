#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct __tree_t {
    struct __tree_t **entry;
    int entry_cnt;

    char* name;
    int profit;

    struct __tree_t *parent;
} tree_t;

static int *init_referral_info_from_enroll(const char *enroll[], size_t enroll_len, const char *referral[], size_t referral_len, size_t *enroll_cnt_len) {
    int *enroll_cnt = NULL;

    if (!enroll || !enroll_len || !referral || !referral_len) return NULL;

    *enroll_cnt_len = enroll_len + 1;

    enroll_cnt = (int *)calloc(1, *enroll_cnt_len * sizeof(int));
    if (!enroll_cnt) return NULL;


    for (size_t i = 0; i < referral_len; i++) {
        const char *str = referral[i];

        if (strncmp("-", str, strlen(str)) == 0) {
            enroll_cnt[0]++;
            continue;
        }
        for (size_t j = 0; j < enroll_len; j++) {
            if (strncmp(enroll[j], str, strlen(str)) == 0) {
                enroll_cnt[j + 1]++;
                break;
            }
        }
    }

    return enroll_cnt;
}

static void printf_enroll_cnt(int *enroll_cnt, size_t enroll_cnt_len, const char *enroll[], size_t enroll_len) {
    //printf("%d\n", enroll_cnt_len);
    for (size_t i = 0; i < enroll_cnt_len; i++) {
        printf("%s - %d\n", i ? enroll[i - 1] : "center", enroll_cnt[i]);
    }
}

static tree_t *create_tree_node(const char *name, size_t entry_cnt) {
    tree_t *node = (tree_t *)malloc(sizeof(tree_t));
    if (!node) return NULL;

    node->entry = (tree_t **)calloc(1, entry_cnt * sizeof(tree_t *));
    node->entry_cnt = entry_cnt;

    node->name = strdup(name);
    node->profit = 0;

    node->parent = NULL;

    return node;
}

static tree_t *init_trees(int *enroll_cnt, size_t entroll_cnt_len, const char *enroll[], size_t enroll_len, const char *referral[], size_t referral_len) {
    tree_t *root = NULL;
    tree_t **enroll_tmp = NULL;

    if (!enroll || !enroll_len || !referral || !referral_len) return NULL;

    //root = (tree_t *)malloc(sizeof(tree_t));
    enroll_tmp = (tree_t **)malloc(entroll_cnt_len * sizeof(tree_t *));
    if (!enroll_tmp) return NULL;

    for (size_t i = 0; i < entroll_cnt_len; i++) {
        enroll_tmp[i] = create_tree_node(i ? enroll[i - 1] : "center", enroll_cnt[i]);
        //printf("name: %s, entry_cnt: %d\n", enroll_tmp[i]->name, enroll_tmp[i]->entry_cnt);
    }

    root = enroll_tmp[0];
    for (size_t i = 0; i < referral_len; i++) {
        const char *str = referral[i];

        if (strncmp("-", str, strlen(str)) == 0) {
            for (size_t h = 0; h < root->entry_cnt; h++) {
                if (!root->entry[h]) {
                    root->entry[h] = enroll_tmp[i + 1];
                    enroll_tmp[i + 1]->parent = root;
                    break;
                }
            }
            continue;
        }
        for (size_t j = 0; j < enroll_len; j++) {
            if (strncmp(enroll[j], str, strlen(str)) == 0) {
                for (size_t h = 0; h < enroll_tmp[j + 1]->entry_cnt; h++) {
                    if (!enroll_tmp[j + 1]->entry[h]) {
                        enroll_tmp[j + 1]->entry[h] = enroll_tmp[i + 1];
                        enroll_tmp[i + 1]->parent = enroll_tmp[j + 1];
                        break;
                    }
                }
                break;
            }
        }
    }

    free(enroll_tmp);

    return root;
}

static void printf_trees(tree_t *root) {
    if (root) {
        printf("[[ %s ]] (entry_cnt: %d, p: %s, profit: %d)\n", root->name, root->entry_cnt, root->parent ? root->parent->name : "NULL", root->profit);
        for (size_t i = 0; i < root->entry_cnt; i++) {
            printf("%s ", root->entry[i]->name);
        }
        printf("\n");
        for (size_t i = 0; i < root->entry_cnt; i++) {
            printf_trees(root->entry[i]);
        }
    }
}

static tree_t *find_seller_referral(tree_t *root, const char *name) {
    if (strncmp(root->name, name, strlen(name)) == 0) {
        return root;
    }

    if (!root->entry_cnt) {
        //printf("%s next entry is zero\n", root->name);
        return NULL;
    }

    for (int i = 0; i < root->entry_cnt; i++) {
        tree_t *node;
        //printf("%d / %d currently name : %s\n", i, root->entry_cnt, root->entry[i]->name);
        if ( (node = find_seller_referral(root->entry[i], name)) ) {
            return node;
        }
    }
    return NULL;
}

static void process_acount_result(tree_t *node, int tot_profit) {
    int p_profit, m_profit;
    
    if (tot_profit < 1) return;
    //printf("%s -> %d\n", node->name, tot_profit);
    if (!node->parent) {
        node->profit += tot_profit;
        return;
    }

    p_profit = tot_profit / 10;
    m_profit = tot_profit - p_profit;

    node->profit += m_profit;

    if (node->parent) process_acount_result(node->parent, p_profit);

    return;
}

#if 0
static binary_tree_t *create_tree_node(const char *name) {
    binary_tree_t *node = (binary_tree_t *)malloc(sizeof(binary_tree_t));
    if (!node) return NULL;

    node->l = NULL;
    node->r = NULL;
    node->name = strdup(name);
    node->profit = 0;

    return node;
}

static void destroy_tree_node(binary_tree_t *node) {
    if (node) {
        if (node->name) free(node->name);
        free(node);
    }
}

static void destroy_tree_nodes(binary_tree_t *root) {
    if (root) {
        if (!root->l && !root->r) {
            destroy_tree_node(root);
            return;
        }

        if (root->l) {
            destroy_tree_nodes(root->l);
            root->l = NULL;
        }
        if (root->r) {
            destroy_tree_nodes(root->r);
            root->r = NULL;
        }
        destroy_tree_node(root);
    }
}

static void print_tree_nodes(binary_tree_t *root) {
    if (root->name) printf("%s\n", root->name);

    if (root->l) print_tree_nodes(root->l);
    if (root->r) print_tree_nodes(root->r);
}

static binary_tree_t *init_tree_nodes_by_referral(const char *enroll[], size_t enroll_len, const char *referral[], size_t referral_len) {
    int retval = -1;
    binary_tree_t *root = NULL;
    binary_tree_t **root_tmp;

    if (!enroll || !enroll_len || !referral || !referral_len) return NULL;

    root_tmp = (binary_tree_t **)malloc(enroll_len * sizeof(binary_tree_t *));
    if (!root_tmp) return NULL;

    /* create all nodes */
    for (size_t i = 0; i < enroll_len; i++) {
        root_tmp[i] = create_tree_node(enroll[i]);
        if (!root_tmp[i]) goto done;
    }

    /* root of binary tree */
    root = create_tree_node("center");
    if (!root) goto done;

    for (size_t i = 0; i < referral_len; i++) {
        if (strncmp(referral[i], "-", 1) == 0) {
            if (!root->l) root->l = root_tmp[i];
            if (!root->r) root->r = root_tmp[i];
        } else {
            
        }
    }

    retval = 0;
    if (root_tmp) free(root_tmp);

done:
    if (retval < 0) {
        if (root_tmp) {
            for (size_t i = 0; i < enroll_len; i++) {
                if (!root_tmp[i]) break;
                destroy_tree_node(root_tmp[i]);
            }
            free(root_tmp);
        }
        if (root) {
            destroy_tree_nodes(root);
            root = NULL;
        }
    }

    return root;
}
#endif

#define DEFAULT_PROFIT  100

int main(void) {
    const char *enroll[] = {"john", "mary", "edward", "sam", "emily", "jaimie", "tod", "young"};
    const char *referral[] = {"-", "-", "mary", "edward", "mary", "mary", "jaimie", "edward"};
    //const char *seller[] = {"young", "john", "tod", "emily", "mary"};
    //int amount[] = {12, 4, 2, 5, 10};
    const char *seller[] = {"sam", "emily", "jaimie", "edward"};
    int amount[] = {2, 3, 5, 4};
    size_t enroll_len = sizeof(enroll) / sizeof(const char *);
    size_t referral_len = sizeof(referral) / sizeof(const char *);
    size_t seller_len = sizeof(seller) / sizeof(const char *);
    size_t amount_len = sizeof(amount) / sizeof(int);

    //printf("%d %d %d %d\n", enroll_len, referral_len, seller_len, amount_len);
    size_t enroll_cnt_len;
    int *enroll_cnt;
    tree_t *root;

    enroll_cnt = init_referral_info_from_enroll(enroll, enroll_len, referral, referral_len, &enroll_cnt_len);
    printf_enroll_cnt(enroll_cnt, enroll_cnt_len, enroll, enroll_len);
    
    root = init_trees(enroll_cnt, enroll_cnt_len, enroll, enroll_len, referral, referral_len);
    printf_trees(root);

    for (size_t i = 0; i < seller_len; i++) {
        const char *seller_name = seller[i];
        int tot_profit = amount[i] * DEFAULT_PROFIT;
        tree_t *node;

        //printf("start search !!! ( %d %s )\n", i, seller_name);
        node = find_seller_referral(root, seller_name);
        if (!node) {
            //printf("fail to search node ( %s )\n", seller_name);
            continue;
        }
        printf("%s ( %d ) -> ", node->name, tot_profit);
        for (tree_t *tmp = node->parent; tmp; tmp = tmp->parent) {
            printf("%s ", tmp->name);
        }
        printf("\n");

        process_acount_result(node, tot_profit);
        //printf_trees(root);
    }
    printf_trees(root);

    int *answer = (int *)malloc(enroll_len * sizeof (int));
    for (size_t i = 0; i < enroll_len; i++) {
        tree_t *node = find_seller_referral(root, enroll[i]);
        if (!node) continue;

        answer[i] = node->profit;
        printf("%d ", answer[i]);
    }

    return 0;
}