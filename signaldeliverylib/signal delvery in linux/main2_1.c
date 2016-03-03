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
unsigned int count1 =0;
pthread_t mythread;
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
void *threadfunc(void *parm)
{
  pthread_t             self = pthread_self();
  
  printf("Thread1 0x%ld entered\n", (long)self);
  while(count1 <= 7)
  {
    run(1000);
    count1++;
    if((count1 ==4))
    {
      pthread_kill(self,SIGUSR1);
    }
  }
  printf("Thread1 0x%ld Exiting(count1:%d):%d\n", (long)self,count1,terminate);
  return NULL;
}
int main()
{
    struct sigaction act;
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &sigiohdl;
    act.sa_flags = SA_RESTART;
    if (sigaction(SIGUSR1, &act,NULL) < 0) {
             perror ("sigaction");
             return 1;
    }
    pthread_create(&mythread, NULL, threadfunc, NULL);
    pthread_join(mythread, NULL);
    printf("\n Terminating main process:%d\n",terminate);
    return 1;  
}
