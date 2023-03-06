#include "queue.h"

struct node
{
    void    *ptr;
    size_t   siz;
    size_t    id;
    node_t *next;

    copy_fun   copy_fun;
    print_fun print_fun; 
    comp_fun   comp_fun;
    free_fun   free_fun;
};

struct queue
{
    node_t *head;
    node_t *tail;
    size_t   len;
};

static inline void *_queue_ptr_alloc(void *ptr, size_t siz, copy_fun copy)
{
    void *tmp = NULL;

    if (copy)
    {
        tmp = copy(ptr);
    }
    else if (ptr != NULL) 
    {
        tmp = malloc(siz);
        memcpy(tmp, ptr, siz);
    }

    return tmp;
}

static inline void _queue_ptr_free(node_t *head)
{
    void **to_free = &(head->ptr);

    if (head->free_fun == NULL)
    {
        free(*to_free);
    }
    else
    {
        head->free_fun(to_free);
    }
}

queue_t *new_queue(void)
{
    return (queue_t *) calloc(1, sizeof(queue_t));
}

queue_t *copy_queue(queue_t *q)
{
    if (!q)
    {
        return NULL;
    }

    queue_t *copy = new_queue();
    
    if (!copy)
    {
        return NULL;
    }

    node_t *head = q->head;

    while (head)
    {
        enqueue_ptr(copy, head->siz, head->id, head->ptr, (cb_t){.copy = head->copy_fun, .print = head->print_fun, .comp = head->comp_fun, .free = head->free_fun});
        head = head->next;
    }

    return copy;
}

q_stat_t enqueue_ptr(queue_t *q, size_t siz, size_t id, void *ptr, cb_t cb)
{
    if (!q)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    node_t *node = (node_t *) calloc(1, sizeof(node_t));

    if (!node)
    {
        return QUEUE_ALLOC_ERROR;
    }
    
    *node = (node_t) {.ptr = _queue_ptr_alloc(ptr, siz, cb.copy), .siz = siz, .id = id, .next = NULL, .copy_fun = cb.copy, .print_fun = cb.print, .comp_fun = cb.comp, .free_fun = cb.free};

    if (!q->head)
    {
        q->head = node;
        q->tail = q->head;
    }
    else
    {
        q->tail->next = node;
        q->tail = q->tail->next;
    }

    q->len += 1;
    return QUEUE_NO_ERROR;
}

q_stat_t enqueue_args(queue_t *q, size_t siz, size_t id, new_fun new, void *new_args, cb_t cb)
{
    if (!q || !new)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    node_t *node = (node_t *) calloc(1, sizeof(node_t));

    if (!node)
    {
        return QUEUE_ALLOC_ERROR;
    }

    *node = (node_t) {.ptr = new(new_args), .siz = siz, .id = id, .next = NULL, .copy_fun = cb.copy, .print_fun = cb.print, .comp_fun = cb.comp, .free_fun = cb.free};

    if (!q->head)
    {
        q->head = node;
        q->tail = q->head;
    }
    else
    {
        q->tail->next = node;
        q->tail = q->tail->next;
    }

    q->len += 1;
    return QUEUE_NO_ERROR;
}

q_stat_t insert_ptr(queue_t *q, size_t siz, size_t id, void *ptr, cb_t cb, size_t idx)
{
    if (!q || idx >= q->len)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    node_t *node = (node_t *) calloc(1, sizeof(node_t));

    if (!node)
    {
        return QUEUE_ALLOC_ERROR;
    }

    node_t *head = q->head;

    for (size_t i = 0; 
         i + 1 < idx; 
         i++)
    {
        head = head->next;
    }

    *node = (node_t) {.ptr = _queue_ptr_alloc(ptr, siz, cb.copy), .siz = siz, .id = id, .next = NULL, .copy_fun = cb.copy, .print_fun = cb.print, .comp_fun = cb.comp, .free_fun = cb.free};

    if (idx == 0)
    {
        node->next = head;
        q->head = node;
    }
    else
    {
        node->next = head->next;
        head->next = node;
    }

    q->len += 1;
    return QUEUE_NO_ERROR;
}

q_stat_t insert_args(queue_t *q, size_t siz, size_t id, new_fun new, void *new_args, cb_t cb, size_t idx)
{
    if (!q || !new || idx >= q->len)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    node_t *node = (node_t *) calloc(1, sizeof(node_t));

    if (!node)
    {
        return QUEUE_ALLOC_ERROR;
    }

    node_t *head = q->head;

    for (size_t i = 0; 
         i + 1 < idx; 
         i++)
    {
        head = head->next;
    }

    *node = (node_t) {.ptr = new(new_args), .siz = siz, .id = id, .next = NULL, .copy_fun = cb.copy, .print_fun = cb.print, .comp_fun = cb.comp, .free_fun = cb.free};

    if (idx == 0)
    {
        node->next = head;
        q->head = node;
    }
    else
    {
        node->next = head->next;
        head->next = node;
    }

    q->len += 1;
    return QUEUE_NO_ERROR;
}

q_stat_t modify_ptr(queue_t *q, size_t siz, size_t id, void *ptr, cb_t cb, size_t idx)
{
    if (!q || idx >= q->len)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    node_t *head = q->head;

    for (size_t i = 0; 
         i < idx; 
         i++)
    {
        head = head->next;
    }

    _queue_ptr_free(head);

    head->ptr = _queue_ptr_alloc(ptr, siz, cb.copy);
    head->siz = siz;
    head->id  = id;

    head->copy_fun  = cb.copy;
    head->print_fun = cb.print;
    head->comp_fun  = cb.comp;
    head->free_fun  = cb.free;

    return (ptr != NULL && head->ptr == NULL) ? QUEUE_ALLOC_ERROR : QUEUE_NO_ERROR;
}

q_stat_t modify_args(queue_t *q, size_t siz, size_t id, new_fun new, void *new_args, cb_t cb, size_t idx)
{
    if (!q || !new || idx >= q->len)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    node_t *head = q->head;

    for (size_t i = 0; 
         i < idx; 
         i++)
    {
        head = head->next;
    }

    _queue_ptr_free(head);

    head->ptr = new(new_args);
    head->siz = siz;
    head->id  = id;

    head->copy_fun  = cb.copy;
    head->print_fun = cb.print;
    head->comp_fun  = cb.comp;
    head->free_fun  = cb.free;

    return QUEUE_NO_ERROR;
}

q_stat_t dequeue(queue_t *q)
{
    if (!q || !q->head)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    node_t *head = q->head;
    _queue_ptr_free(head);

    head = head->next;
    free(q->head);

    q->head = head;
    q->len -= 1;

    return QUEUE_NO_ERROR;
}

q_stat_t rem(queue_t *q, size_t idx)
{
    if (!q || !q->head || idx >= q->len)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    q->len -= 1;

    node_t *head = q->head;
    node_t *tail = NULL;

    for (size_t i = 0; 
         i < idx; 
         i++)
    {
        tail = head;
        head = head->next;
    }

    _queue_ptr_free(head);

    if (!tail)
    {
        q->head = head->next;
    }
    else
    {
        tail->next = head->next;
    }

    free(head);
    return QUEUE_NO_ERROR;
}

q_stat_t free_queue(queue_t **q)
{
    if (!q || !*q)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    while ((*q)->head)
    {
        dequeue(*q);
    }

    free(*q);
    *q = NULL;

    return QUEUE_NO_ERROR;
}

q_stat_t print_queue(queue_t *q)
{
    if (!q)
    {
        return QUEUE_NULL_ARG_ERROR;
    }

    node_t *head = q->head;
    size_t  i = 0;

    while (head)
    {   
        if (head->print_fun)
        {
            printf("Element %ld: ", i++);
            head->print_fun(head->ptr);
        } 
        else
        {
            printf("Element %ld: No print function was called...\n", i++);
        }

        head = head->next;
    }

    return QUEUE_NO_ERROR;
}

size_t len_queue(queue_t *q)
{
    return (!q) ? SIZE_MAX : q->len;
}

// The void pointer that is returned by this function has been allocated either
// through the copy callback function or malloc. 

void *get(queue_t *q, size_t siz, size_t id, size_t idx)
{
    if (!q || idx >= q->len)
    {
        return NULL;
    }

    node_t *head = q->head;

    for (size_t i = 0; 
         i < idx; 
         i++)
    {
        head = head->next;
    }

    if (siz != head->siz || id != head->id)
    {
        return NULL;
    }

    return _queue_ptr_alloc(head->ptr, head->siz, head->copy_fun);
}

// Peek at the front of the queue. 

void *peek_front(queue_t *q, size_t siz, size_t id)
{
    if (!q)
    {
        return NULL;
    }

    node_t *head = q->head;

    if (siz != head->siz || id != head->id)
    {
        return NULL;
    }

    return _queue_ptr_alloc(head->ptr, head->siz, head->copy_fun);
}

// Peak at the end of the queue.

void *peek_back(queue_t *q, size_t siz, size_t id)
{
    if (!q)
    {
        return NULL;
    }

    node_t *tail = q->tail;

    if (siz != tail->siz || id != tail->id)
    {
        return NULL;
    }

    return _queue_ptr_alloc(tail->ptr, tail->siz, tail->copy_fun);
}

int is_equal(queue_t *a, queue_t *b)
{
    // This function returns three possibilities: -1, 0, 1.
    // This is not a "space-ship" operation.
    // 1 and 0 are self-explanatory. -1 is reserved for reporting ambiguity.

    if (!a || !b)
    {
        return (a == b);
    }

    if (a->len != b->len)
    {
        return 0;
    }

    node_t *h1 = a->head;
    node_t *h2 = b->head;

    bool ambig = false;
    bool   ret = true;

    for (size_t i = 0; 
         i < a->len; 
         i++)
    {
        if ((h1->siz != h2->siz) || 
            (h1->id  != h2->id))
        {
            ret = false;
            break;
        }
        
        if (!h1->comp_fun || !h2->comp_fun)
        {
            // Despite everything else being similar, if there is no callback function
            // for comparison, then the value being pointed to by node's ptr structure
            // variable cannot be determined. In other words, it is ambiguous.

            ambig = true;
        }
        else if ((h1->comp_fun == h2->comp_fun) && 
                 (h1->comp_fun(h1->ptr, h2->ptr) == false))
        {
            ret = false;
            break;
        }

        h1 = h1->next;
        h2 = h2->next;
    }

    return (ambig) ? -1 : ret;
}