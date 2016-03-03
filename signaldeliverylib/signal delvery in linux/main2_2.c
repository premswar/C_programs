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
unsigned int count1 =0,count2=0;
pthread_t mythread1,mythread2,main_thrd;
void run(unsigned long j)
{
  unsigned long i,k;
  unsigned long long tmp =0;
  for(i = 1;i<=j; i++)
  {
    for(k =0;k<16384;k++)
    {
      tmp++;
    }
  }
}
static void sigiohdl (int sig, siginfo_t *siginfo, void *context)
{
   terminate ++;  
}
void *threadfunc1(void *parm)
{
  pthread_t             self = pthread_self();
  int                   rc;
  struct sched_param param;
  param.sched_priority = 30;
  rc = pthread_setschedparam(pthread_self(),SCHED_FIFO, &param);
               if (rc != 0)
                   handle_error_en(rc, "pthread_setschedparam");

  printf("Thread1 0x%ld entered\n", (long)self);
  while(count1 <= 7)
  {
    run(1000);
    count1++;
    if((count1 ==4))
    {
      printf("sending signal to Thread2 0x%ld \n", (long)mythread2);
      pthread_kill(mythread2,SIGUSR1);
    }
  }
  printf("Thread1 0x%ld Exiting(count1:%d):%d\n", (long)self,count1,terminate);
  return NULL;
}
void *threadfunc2(void *parm)
{
  pthread_t             self = pthread_self();
  int                   rc;
  struct sched_param param;
  param.sched_priority = 20;
  rc = pthread_setschedparam(pthread_self(),SCHED_FIFO, &param);
               if (rc != 0)
                   handle_error_en(rc, "pthread_setschedparam");

  printf("Thread2 0x%ld entered\n", (long)self);
  while(count2<=7)
  {
    run(1000);
    count2++;
  }
  printf("Thread2 0x%ld Exiting(count1:%d):%d\n", (long)self,count2,terminate);
  return NULL;
}

int main()
{
    struct sigaction act;
    struct sched_param param;
    param.sched_priority = 10;
    printf("This is parent the FIFO sched pri min:%d,max%d\n",(int)sched_get_priority_min(SCHED_FIFO),(int)sched_get_priority_max(SCHED_FIFO));
    sched_setscheduler(0,SCHED_FIFO,&param);
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &sigiohdl;
    act.sa_flags = SA_RESTART;
    if (sigaction(SIGUSR1, &act,NULL) < 0) {
             perror ("sigaction");
             return 1;
    }
    main_thrd = pthread_self() ;
    pthread_create(&mythread1, NULL, threadfunc1, NULL);
    pthread_create(&mythread2, NULL, threadfunc2, NULL);
    sleep(10);
    while(terminate < 1)
    {
       run(2000);
    }
    printf("\n Terminating main process\n");
    return 1;  
}
