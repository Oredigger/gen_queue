#include <float.h>
#include <limits.h>
#include "callback_helper.h"
#include <stdint.h>

static q_stat_t queue_data_op_ptr(queue_t **q, void *data, size_t siz, size_t id, size_t *idx, op_t op, cb_t cb)
{
    if (!q || (op != ENQUEUE && idx == NULL))
    {
        return 1;
    }

    if (!*q)
    {
        *q = new_queue();
    }

    q_stat_t stat = QUEUE_NO_ERROR;

    switch (op)
    {
        case ENQUEUE:
            stat = enqueue_ptr(*q, siz, id, data, cb);
            break;
        case INSERT:
            stat = insert_ptr(*q, siz, id, data, cb, *idx);
            break;
        case MODIFY:
            stat = modify_ptr(*q, siz, id, data, cb, *idx);
            break;
        default:
            return 1;
    }

    return (stat != QUEUE_NO_ERROR);
}

static q_stat_t queue_data_op_args(queue_t **q, void *data, size_t siz, size_t id, size_t *idx, new_fun new, void *new_args, op_t op, cb_t cb)
{
    if (!q || (op != ENQUEUE && idx == NULL))
    {
        return 1;
    }

    if (!*q)
    {
        *q = new_queue();
    }

    q_stat_t stat = QUEUE_NO_ERROR;

    switch (op)
    {
        case ENQUEUE:
            stat = enqueue_args(*q, siz, id, new, new_args, cb);
            break;
        case INSERT:
            stat = insert_args(*q, siz, id, new, new_args, cb, *idx);
            break;
        case MODIFY:  
            stat = modify_args(*q, siz, id, new, new_args, cb, *idx);
            break;
        default:
            return 1;
    }

    return (stat != QUEUE_NO_ERROR);
}

static void *queue_data_get(queue_t *q, size_t siz, size_t id, size_t *idx, op_t op)
{
    if (!q || (op == GET && idx == NULL))
    {
        return NULL;
    }

    switch (op)
    {
        case GET:
            return get(q, siz, id, *idx);
        case PEEK_FRONT:
            return peek_front(q, siz, id);
        case PEEK_BACK:
            return peek_back(q, siz, id);
        default:
            return NULL;
    }
}

static void print_int(void *ptr)
{
    if (ptr)
    {
        printf("%d\n", *((int *) ptr));
    }
}

static bool is_int_equal(void *a, void *b)
{
    return (a && b) ? *(int *) a == *(int *) b : a == b;
}

int queue_int_op(queue_t **q, int data, size_t *idx, op_t op)
{
    return queue_data_op_ptr(q, &data, sizeof(int), INT, idx, op, (cb_t) {NULL, print_int, is_int_equal, NULL});
}

int queue_int_get(queue_t *q, size_t *idx, op_t op)
{
    void *ptr = queue_data_get(q, sizeof(int), INT, idx, op);
    int   val = (!ptr) ? INT_MAX : *((int *) ptr);
    free(ptr);

    return val;
}

static void print_double(void *ptr)
{
    if (ptr)
    {
        printf("%f\n", *((double *) ptr));
    }
}

static bool is_double_equal(void *a, void *b)
{
    return (a && b) ? *(double *) a == *(double *) b : a == b;
}

int queue_double_op(queue_t **q, int data, size_t *idx, op_t op)
{
    return queue_data_op_ptr(q, &data, sizeof(double), DOUBLE, idx, op, (cb_t) {NULL, print_double, is_double_equal, NULL});
}

double queue_double_get(queue_t *q, size_t *idx, op_t op)
{
    void  *ptr = queue_data_get(q, sizeof(double), DOUBLE, idx, op);
    double val = (!ptr) ? DBL_MAX : *((double *) ptr);
    free(ptr);

    return val;
}

void *copy_string(void *ptr)
{
    if (!ptr)
    {
        return NULL;
    }

    size_t len = strlen((char *) ptr);
    char  *cpy = (char *) calloc(len + 1, sizeof(char));
    snprintf(cpy, len + 1, "%s", (char *) ptr);

    return (void *) cpy;
}

static void print_string(void *ptr)
{
    if (ptr)
    {
        printf("%s\n", (char *) ptr);
    }
}

static bool is_str_equal(void *a, void *b)
{
    if (!a || !b)
    {
        return a == b;
    }

    return strcmp((char *) a, (char *) b) == 0;
}

int queue_string_op(queue_t **q, char *data, size_t *idx, op_t op)
{
    return queue_data_op_args(q, &data, sizeof(char *), STRING, idx, copy_string, data, op, (cb_t) {copy_string, print_string, is_str_equal, NULL});
}

char *queue_string_get(queue_t *q, size_t *idx, op_t op)
{
    return (char *) queue_data_get(q, sizeof(char *), STRING, idx, op);
}

static void print_tuple(void *ptr)
{
    tuple_t *tup = (tuple_t *) ptr;

    if (tup)
    {
        printf("(%f,%f)\n", tup->_p_0, tup->_p_1);
    }
}

static bool is_tup_equal(void *a, void *b)
{
    return (a && b) ? is_tuple_equal(*(tuple_t *) a, *(tuple_t *) b) : a == b;
}

int queue_tuple_op(queue_t **q, tuple_t tup, size_t *idx, op_t op)
{
    return queue_data_op_ptr(q, &tup, sizeof(tuple_t), TUPLE, idx, op, (cb_t) {NULL, print_tuple, is_tup_equal, NULL});
}

tuple_t queue_tuple_get(queue_t *q, size_t *idx, op_t op)
{
    void   *ptr = queue_data_get(q, sizeof(tuple_t), TUPLE, idx, op);
    tuple_t tup = (!ptr) ? new_tuple() : *((tuple_t *) ptr);
    free(ptr);

    return tup;
}