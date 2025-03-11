#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <stdlib.h>
#include <pthread.h>

#include "queue.h"


static CQueue queue;

void * proc_write(void* ptr)
{
     int i = 0;

     unsigned long base = (unsigned long)(ptr);

     int step = CQUE_COUNT + CQUE_COUNT / 2 + 10;

     unsigned long j = base * step;
     unsigned long n = j + step;

     pthread_t thid = pthread_self();

     while( j < n )
     {     
	 unsigned long* p = malloc( sizeof(unsigned long) );
	 *p = j++;

	 NS_NOW();
	 while ( ! put(&queue, p) ) 
         {

	 }
	 NS_COST(put);

	// printf("th %lu index %d put %lu\n", thid, i, *p);

	 i++;
	 if (i == CQUE_COUNT )
            i = 0;		 
     }
     

     return 0;
}


void* proc_read(void* ptr)
{
    do
    {	
       //NS_NOW();	    
       void* p = get(&queue);

       //NS_COST(get);

       if ( p ) 
       {
       //   printf("get %d\n", *(int*)p );

       }
       else {
        //  printf("get null\n");
       }

    } while (1);
}


int main()
{
    	
    init_queue(&queue);

    pthread_t pr2;
    pthread_t pr;
    pthread_t pw, pw2;


    pthread_create( &pr, NULL, proc_read, NULL);

   // pthread_create( &pr2, NULL, proc_read, NULL);

    pthread_create( &pw, NULL, proc_write, (void*) 0);

//    pthread_create( &pw2, NULL, proc_write, (void*)1 );

    pthread_join( pw, NULL );
    printf("pw exit\n");

  //  pthread_join( pw2, NULL );
   // printf("pw2 exit\n");

    pthread_join( pr, NULL );


   // pthread_join( pr2, NULL );

    return 0;
}
