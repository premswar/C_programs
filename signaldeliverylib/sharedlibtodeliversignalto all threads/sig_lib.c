/*
 * =====================================================================================
 *
 *       Filename:  sig_lib.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/30/2014 05:24:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Prem swaroop k (), premswaroop@asu.edu
 *   Organization:  
 *
 * =====================================================================================
 */

#define _MULTI_THREADED
#include <pthread.h>
#include <stdio.h>
#include <sys/signal.h>
#include <errno.h>
#include <string.h>

#define MAXTHREADS 30
pthread_t mythreads[MAXTHREADS+1]={0};
typedef void (*action)(int, siginfo_t *, void *);
action thrd_action;
static unsigned int top =1;
unsigned int getTnoFrmId(long self)
{
  int i;
  for(i= 1;i<=MAXTHREADS;i++)
  {
    if((long)mythreads[i]==self)
        return i;
  }
  return 0;
}
static void sighdl (int sig, siginfo_t *siginfo, void *context)
{
    int i;

    //printf("\nThe thread for current process is %ld and parent is:%ld\n",(long)pthread_self(),(long)mythreads[0]);
    if((long)pthread_self() == (long)mythreads[0])/* send to all threads as it is rcvd in main thrd */
    {
      //printf("entered parent thread\n");
      for(i=1; i<=MAXTHREADS; i++) 
      {
        if(mythreads[i] == 0)
        {
          break;
        }
        pthread_kill(mythreads[i], sig);
      }
    }
    (void) thrd_action(sig,siginfo,context);
    return;
}

int mythread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg)
{
   int                     rc;
   pthread_t temp_thrd;
   if(top <= 1)
   {
     /* first time so store process id in thread0 */
     mythreads[0] = pthread_self();
     //printf("\nThe thread for main process is %ld\n",(long)mythreads[0]);
   }
   else if(top > MAXTHREADS)
   { 
     printf("\nError cannot create thread: Reached maxno threads \n");
     return -1;
   } 
   //printf("\nThe mythread_create creating tid:%d %ld\n",top,(long)mythreads[0]);
   rc = pthread_create(&temp_thrd, attr, start_routine, arg);
   mythreads[top] =  temp_thrd;
   *thread = temp_thrd;
   //printf("\nThe mythread_create created thrdid:%ld %ld\n",(long)mythreads[top],(long)mythreads[0]);
   top++;
   return rc;
}
int mythread_sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact)
{
   struct sigaction        actions;
   memset(&actions, 0, sizeof(actions));
   memcpy(&actions,act,sizeof(actions));
   actions.sa_sigaction = &sighdl;
   thrd_action = act->sa_sigaction;
   return sigaction(SIGIO,&actions,oldact); 
}
void mythread_exit(void *retval)
{
   unsigned int tno,i;
   tno = getTnoFrmId((long)pthread_self());
   for(i = tno;i<top;i++)
   {
    mythreads[i] = mythreads[i+1];
   }
   mythreads[top] =0;
   top--;
   pthread_exit(retval);
   return;
}
int  mythread_join(pthread_t thread, void **retval)
{
  return pthread_join(thread, retval);
}

