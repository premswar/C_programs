/*
 * =====================================================================================
 *
 *       Filename:  mysig.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/30/2014 06:43:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Prem swaroop k (), premswaroop@asu.edu
 *   Organization:  
 *
 * =====================================================================================
 */

#include <pthread.h>
#include <stdio.h>
#include <sys/signal.h>
#include <errno.h>
#include <string.h>
#include "sig_lib.h"

#define NUMTHREADS 3
void sighand(int sig, siginfo_t *siginfo, void *context);
sig_atomic_t send_child  =3;
pthread_t               threads[NUMTHREADS+1];
unsigned int TnoFrmId(long self)
{
  int i;
  for(i= 1;i<=NUMTHREADS;i++)
  {
    if((long)threads[i]==self)
        return i;
  }
  return 0;
}
void *threadfunc(void *parm)
{
  pthread_t             self = pthread_self();
  int                   rc;

  printf("Thread 0x%ld entered\n", (long)self);
  errno = 0;
  while(send_child )
  {
    rc = sleep(1);
  }
  mythread_exit(NULL);
  return NULL;
}

int main()
{
  int                     rc;
  int                     i;
  struct sigaction        actions;

  printf("MAIN process :Thread 0 is main process\n");
  memset(&actions, 0, sizeof(actions));
  sigemptyset(&actions.sa_mask);
  actions.sa_flags = SA_SIGINFO;
  actions.sa_sigaction = &sighand;

  rc = mythread_sigaction(SIGIO,&actions,NULL);

  for(i=1; i<=NUMTHREADS; i++) {
    rc = mythread_create(&threads[i], NULL, threadfunc, NULL);
    printf("created Thread :%d [%ld] \n",i,(long)threads[i]);
  }  
  while(send_child)
  {
    sleep(1);
  }
  for(i=1; i<=NUMTHREADS; ++i) {
    rc = mythread_join(threads[i], NULL);
  }
  printf("Main completed\n");
  return 0;
}

void sighand(int sig, siginfo_t *siginfo, void *context)
{
  pthread_t             self = pthread_self();
  unsigned int          tid;
  
  tid = TnoFrmId((long)self);
  printf("SIG[%d]Thread %d in signal handler\n",sig,tid);
  if(tid == 0)
  {
    send_child --;
  }
  return;
}
