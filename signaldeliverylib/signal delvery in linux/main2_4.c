/*
 * =====================================================================================
 *
 *       Filename:  main2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/04/2014 08:42:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Prem swaroop k (), premswaroop@asu.edu
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>  /* Semaphore */
#include <sys/sem.h>
#include <sched.h>
#include<errno.h>
#include <time.h>
#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
sig_atomic_t terminate  =0;
int count =0;
pthread_t mythread1;
static void sigiohdl (int sig, siginfo_t *siginfo, void *context)
{
    terminate ++;  
}
void *threadfunc1(void *parm)
{
  while(1)
  {
    sleep(5);
    count++;
  }
}
int main()
{
    struct sigaction act;

    printf("This is parent the FIFO sched pri min:%d,max%d\n",(int)sched_get_priority_min(SCHED_FIFO),(int)sched_get_priority_max(SCHED_FIFO));
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &sigiohdl;
    act.sa_flags = SA_RESTART;
    if (sigaction(SIGUSR1, &act,NULL) < 0) {
             perror ("sigaction");
             return 1;
    }
    pthread_create(&mythread1, NULL, threadfunc1, NULL);
    sleep(20);
    while(terminate <= 3)
    {
       sleep(2);
       pthread_kill(mythread1,SIGUSR1);
    }
    printf("\n Terminating main process:count:%d\n",count);
    return 1;  
}
