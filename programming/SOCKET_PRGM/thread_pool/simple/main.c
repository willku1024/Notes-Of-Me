#include <unistd.h>
#include <stdio.h>
#include "threadpool.h"
#include "threadpool.c"

/* 线程池中的线程，模拟处理业务 */
void *process(void *arg)
{
    int num = *(int *)arg;
    // printf("thread 0x%x working on task %d\n ", (unsigned int)pthread_self(), num);
    printf("callback: print %d\n", num);
    // printf("thread 0x%x finish the task %d\n ", (unsigned int)pthread_self(), num);

    return NULL;
}

int main(void)
{
    /*threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);*/

    threadpool_t *thp = threadpool_create(3,100,100);   /*创建线程池，池里最小3个线程，最大100，队列最大100*/
    printf("pool inited");

    //int *num = (int *)malloc(sizeof(int)*20);
    int num[20], i;
    for (i = 0; i < 20; i++) {
        num[i]=i;
        threadpool_add(thp, process, (void*)&num[i]);   /* 向线程池中添加任务 */
    }
	
    sleep(10);
    threadpool_destroy(thp);

    return 0;
}
