#ifndef QUEUE_H
#define QUEUE_H

#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct node  node_t;
typedef struct queue queue_t;
typedef struct cb    cb_t;

struct cb
{
    void  *copy;
    void *print;
    void  *comp;
    void  *free;
};

typedef enum q_stat q_stat_t;

enum q_stat
{
    QUEUE_NO_ERROR,
    QUEUE_NULL_ARG_ERROR,
    QUEUE_ALLOC_ERROR,
    QUEUE_UNKNOWN_ERROR
};

typedef void  (*free_fun)(void **);
typedef void *(*copy_fun)(void *);
typedef void  *(*new_fun)(void *);
typedef bool  (*comp_fun)(void *, void *);
typedef void (*print_fun)(void *);

queue_t    *new_queue(void);
queue_t   *copy_queue(queue_t  *q);
q_stat_t  enqueue_ptr(queue_t  *q, size_t siz, size_t id, void *ptr, cb_t cb);
q_stat_t enqueue_args(queue_t  *q, size_t siz, size_t id, new_fun new, void *new_args, cb_t cb);
q_stat_t   insert_ptr(queue_t  *q, size_t siz, size_t id, void *ptr, cb_t cb, size_t idx);
q_stat_t  insert_args(queue_t  *q, size_t siz, size_t id, new_fun new, void *new_args, cb_t cb, size_t idx);
q_stat_t   modify_ptr(queue_t  *q, size_t siz, size_t id, void *ptr, cb_t cb, size_t idx);
q_stat_t  modify_args(queue_t  *q, size_t siz, size_t id, new_fun new, void *new_args, cb_t cb, size_t idx);
q_stat_t      dequeue(queue_t  *q);
q_stat_t          rem(queue_t  *q, size_t idx);
q_stat_t   free_queue(queue_t **q);
q_stat_t  print_queue(queue_t  *q);
size_t      len_queue(queue_t  *q);
void             *get(queue_t  *q, size_t siz, size_t id, size_t idx);
void      *peek_front(queue_t  *q, size_t siz, size_t id);
void       *peek_back(queue_t  *q, size_t siz, size_t id);
int          is_equal(queue_t  *a, queue_t *b);

#ifdef __cplusplus
}
#endif

#endif