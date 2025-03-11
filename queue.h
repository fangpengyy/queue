#ifndef __CQUEUE_H__
#define __CQUEUE_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <time.h>
#include <stdlib.h>


#define NS_NOW() \
   struct timespec start, end; \
   clock_gettime(CLOCK_MONOTONIC, &start)


#define NS_COST(flag) \
   clock_gettime(CLOCK_MONOTONIC, &end);\
   unsigned long ns = ( end.tv_sec - start.tv_sec ) * 1000000000LL + ( end.tv_nsec - start.tv_nsec ); \
   printf("%s cost ns %lu\n", #flag, ns)



void memory_barrier();

typedef int  bool;

#define CQUE_COUNT   1024

typedef struct CQueue_t
{
    void* _arr[CQUE_COUNT];
    int _read_head;
    int _read_tail;

    int _write_head;
    int _write_tail;
} CQueue;


void init_queue(CQueue* queue);

int get_queue_size();

bool put(CQueue* queue, void *ptr);

void* get(CQueue* queue);

#endif

