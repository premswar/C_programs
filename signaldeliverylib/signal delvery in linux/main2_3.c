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
sig_atomic_t terminate  =0;
sem_t semLock;
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
    sem_wait(&semLock);
    run(2000);
  printf("Thread1 0x%ld Exiting:%d\n", (long)self,terminate);
  return NULL;
}
int main()
{
    struct sigaction act;
    sem_init(&semLock, 0,0);
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &sigiohdl;
    act.sa_flags = SA_RESTART;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &act,NULL) < 0) {
             perror ("sigaction");
             return 1;
    }
    pthread_create(&mythread, NULL, threadfunc, NULL);
    while(terminate <= 1)
    {
      run(1000);
      pthread_kill(mythread,SIGUSR1);
    }   
    //pthread_join(mythread, NULL);
    printf("\n Terminating main process:%d\n",terminate);
    return 1;  
}
