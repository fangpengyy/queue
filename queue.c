#include "queue.h"


void memory_barrier() {
    __asm__ __volatile__ ("": : :"memory");
}

void init_queue(CQueue* queue)
{
    memset( &queue->_arr, 0, CQUE_COUNT * sizeof(void*) );

    queue->_read_head = 0;
    queue->_read_tail = 0;

    queue->_write_head = 0;
    queue->_write_tail = 0;
}

int get_queue_size()
{
    return CQUE_COUNT;
}

bool put(CQueue* queue, void *ptr)
{
   int head = 0;	
   int next = 0;
   int read_tail = 0;
   bool ret;
   NS_NOW();

   head = queue->_write_head;
   next = ( head + 1 )% CQUE_COUNT;

   read_tail = queue->_read_tail;
   if ( next == read_tail )
   {
       return 0;
   }

   ret = __sync_bool_compare_and_swap( &queue->_write_head, head, next );

   if ( ret ) 
   {
      queue->_arr[head] = ptr;

      memory_barrier();

      do
      {
          ret = __sync_bool_compare_and_swap( &queue->_write_tail, head, next );
      } while ( !ret );

      NS_COST(put);
      return 1;
   }
  
   return 0;
}

void* get(CQueue* queue)
{
   int head = 0;	
   int next = 0;
   int write_tail = 0;
   bool ret;
   void *ptr = NULL;

   NS_NOW();

   head = queue->_read_head;
   next =  ( head + 1 ) % CQUE_COUNT;

   write_tail = queue->_write_tail;
   if ( head == write_tail )
   {
       return NULL;
   }

   ret = __sync_bool_compare_and_swap(&queue->_read_head, head, next);

   if ( ret ) 
   {
       ptr =  queue->_arr[head];
       
       do
       {
           ret = __sync_bool_compare_and_swap(&queue->_read_tail, head, next);
       } while ( !ret );

       NS_COST(get); 
       return ptr;
   }
   
   return NULL;
}

