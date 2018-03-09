// File:	my_pthread.c
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server:

#include <ucontext_t.h>
#include "my_pthread_t.h"

void fn_wrapper(void*(fn)(void*), void* args, my_pthread_t tid) {
    ccontext* ctx = tcb->context_list[tid];
    ctx->res = (*fn)(args);
}

ucontext_t* create_context(my_pthread_t tid, void*(fn)(void*), void* args) {
    ucontext_t* T = (ucontext_t*)calloc(1, sizeof(ucontext_t));
    T->uc_link = 0;
    T->uc_stack.ss_sp = malloc(MEM_SZ);
    T->uc_stack.ss_size = MEM_SZ;
    T->uc_stack.ss_flags = 0;
    makecontext(T, (void*)&fn_wrapper, 3, fn, args, tid);
    return T;
}

int get_available_tid() {
    if (tcb->p < THRD_CNT) {
        tcb->p += 1; // automatically moves p forward by 1
        return tcb->p - 1; // returns the previous value of p
    }
    // finds available value of p and set p to i + 1 while returning i
    int i;
    for (i = 0; i < THRD_CNT; i++) {
        if (tcb->context_list[i] == NULL) {
            tcb->p = i + 1;
            return i;
        }
    }
    return -1; // no more threads available
}

// helpers
void link_contexts(ccontext* c1, ccontext* c2) {
    c1->next = c2;
    c2->prev = c1;
}

// TCB managers
void insert_to_tcb(ccontext* ctx) {
    tcb->context_list[ctx->cid] = ctx; // inserts to context_list
}

void insert_to_queueN(queueList* q, ccontext* ctx) {
    if (q->head == NULL) {
        q->head = ctx;
        q->tail = ctx;
    } else {
        link_contexts(q->tail, ctx);
        q->tail = ctx;
    }
}

void insert_to_queue0(ccontext* ctx) {
    insert_to_queueN(mlpq->queue0, ctx);
}

void remove_nth_context(ccontext* ctx) {
    queueList* q;
    switch (ctx->priority) {
        case 0:
            q = mlpq->queue0;
            break;
        case 1:
            q = mlpq->queue1;
            break;
        case 2:
            q = mlpq->queue2;
            break;
        case 3:
            q = mlpq->queue3;
            break;
    }
    if (q->head == q->tail) {
        q->head = NULL;
        q->tail = NULL;
    } else {
        if (q->head == ctx) {
            q->head->next->prev = NULL;
            q->head = q->head->next;
        } else if (q->tail == ctx) {
            q->tail->prev->next = NULL;
            q->tail = q->tail->prev;
        } else {
            link_contexts(ctx->prev, ctx->next);
        }
    }
}

// Context Managers
void move_to_next_context() {
    
}

// PRIMARY FUNCTIONS
/* create a new thread */
int my_pthread_create(my_pthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {
    ccontext ctx = (ccontext*)calloc(1, sizeof(ccontext));
    *thread = get_available_tid(); // gets the next available tid
    ctx->ctx = create_context(*thread, function, arg);
    ctx->next = NULL;
    ctx->prev = NULL;
    ctx->priority = 0;
    ctx->cid = *thread; // assigns it the same value as TID
    insert_to_tcb(ctx);
    insert_to_queue0(ctx);
	return 0;
};

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield() {
    
	return 0;
};

/* terminate a thread */
void my_pthread_exit(void *value_ptr) {
    
};

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr) {
	return 0;
};

/* initial the mutex lock */
int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
	return 0;
};

/* aquire the mutex lock */
int my_pthread_mutex_lock(my_pthread_mutex_t *mutex) {
	return 0;
};

/* release the mutex lock */
int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex) {
	return 0;
};

/* destroy the mutex */
int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex) {
	return 0;
};

