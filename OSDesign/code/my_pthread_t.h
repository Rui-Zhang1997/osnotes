// File:	my_pthread_t.h
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server: 
#ifndef MY_PTHREAD_T_H
#define MY_PTHREAD_T_H

#define _GNU_SOURCE

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

const int KB_SIZE = 1024;
const int MEM_SZ = 10 * KB_SIZE;
const int THRD_CNT = 256;
const int CTX_CNT = 256;
typedef uint my_pthread_t;

typedef struct ccontext {
    ucontext_t* ctx;
    struct ccontext* next;
    struct ccontext* prev;
    int cid;
    int priority;
    void* R;
} ccontext;

// queues lower number = higher priority

typedef struct queueList {
    ccontext* head;
    ccontext* tail;
} queueList;
typedef struct mlpq {
    queueList* queue0;
    queueList* queue1;
    queueList* queue2;
    queueList* queue3;
    queueList* curr; // points to the currently executing queueList
    /*ccontext queue0[CTX_CNT];
    ccontext queue1[CTX_CNT];
    ccontext queue2[CTX_CNT];
    ccontext queue3[CTX_CNT];
    ccontext* head_ptr; // points to the queue
    */
} mlpq;

// relates tid to a context
typedef struct threadControlBlock {
    ccontext context_list[THRD_CNT];
    int p; // points to next available tid
} tcb; 

/* mutex struct definition */
typedef struct my_pthread_mutex_t {
	/* add something here */
} my_pthread_mutex_t;

/* define your data structures here: */

// Feel free to add your own auxiliary data structures


/* Function Declarations: */

/* create a new thread */
int my_pthread_create(my_pthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg);

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield();

/* terminate a thread */
void my_pthread_exit(void *value_ptr);

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr);

/* initial the mutex lock */
int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

/* aquire the mutex lock */
int my_pthread_mutex_lock(my_pthread_mutex_t *mutex);

/* release the mutex lock */
int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex);

/* destroy the mutex */
int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex);

#endif
