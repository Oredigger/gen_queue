#ifndef CALLBACK_HELPER_H
#define CALLBACK_HELPER_H

#include "queue.h"
#include "tuple.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum data_type data_type_t;

enum data_type
{
    INT    = 0,
    DOUBLE = 1,
    STRING = 2,
    TUPLE  = 3,
    OTHER  = 4
};

typedef enum op op_t;

enum op
{
    ENQUEUE    = 0,
    INSERT     = 1,
    MODIFY     = 2,
    GET        = 3,
    PEEK_FRONT = 4,
    PEEK_BACK  = 5
};

int  queue_int_op(queue_t **q, int data, size_t *idx, op_t op);
int queue_int_get(queue_t  *q, size_t *idx, op_t op);

int    queue_double_op(queue_t **q, int data, size_t *idx, op_t op);
double queue_double_get(queue_t *q, size_t *idx, op_t op);

int    queue_string_op(queue_t **q, char *data, size_t *idx, op_t op);
char *queue_string_get(queue_t  *q, size_t *idx, op_t op);

int      queue_tuple_op(queue_t **q, tuple_t tup, size_t *idx, op_t op);
tuple_t queue_tuple_get(queue_t  *q, size_t *idx, op_t op);

#ifdef __cplusplus
}
#endif

#endif