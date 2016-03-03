/*
 * =====================================================================================
 *
 *       Filename:  main1.c
 *
 *    Description:  test setjmp and lngjmp
 *
 *        Version:  1.0
 *        Created:  12/04/2014 02:39:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Prem swaroop k (), premswaroop@asu.edu
 *   Organization:  
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<linux/input.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>  /* Semaphore */
#include <sys/sem.h>
#include <sched.h>
#include<errno.h>
#include <time.h>
#include <setjmp.h>
#include <sys/time.h>

#define MOUSE_DEV "/dev/input/event5"
static jmp_buf buf;
struct timeval start;
struct timeval end;
pthread_t main_thrd;
////////////////////////////////////////////////////////
// Mouse thread which gets the left/right click and sets
// the corresponding filter
////////////////////////////////////////////////////////
void *mouse_thread()
{
    int mouse_fd;
    unsigned int ret,cnt=0;
    struct input_event mouse_event;
    printf("mouse thread\n");
    mouse_fd = open(MOUSE_DEV,O_RDONLY);
    while (1)
    {
        read(mouse_fd, &mouse_event, sizeof(mouse_event));
        if (mouse_event.type == EV_KEY && mouse_event.code == BTN_LEFT && mouse_event.value == 0)
            printf("LEFT_CLICK\n");
        else if (mouse_event.type == EV_KEY && mouse_event.code == BTN_RIGHT && mouse_event.value == 0)
         {
            if(cnt == 0)
            {
              cnt++;
              gettimeofday(&start, NULL);
            }
            else if(cnt ==1)
            {
              gettimeofday(&end, NULL);
              if(((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))<= 1000000)
              {
                printf("RIGHT_CLICK_DOUBLE\n");
                pthread_kill(main_thrd,SIGUSR1);
                cnt =0;
              }
              else
              {
               // printf("RIGHT_CLICK:%ld \n",((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
                cnt =1;
                gettimeofday(&start, NULL);
              }
            }
         }
    }
    printf("\n exiting program as you pressed right click%dtimes\n",cnt);
    close(mouse_fd);

}
static void sigiohdl (int sig, siginfo_t *siginfo, void *context)
{
        printf ("\nSIGNAL_rcvd[thrd]: %d, \n ",sig);
        longjmp(buf,1); 
}

int main()
{
    unsigned int cnt=0;
    pthread_t mouse_thrd;
    struct sigaction act;
    memset (&act, '\0', sizeof(act));
    act.sa_sigaction = &sigiohdl;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &act,NULL) < 0) {
             perror ("sigaction");
             return 1;
    }
    main_thrd = pthread_self() ;
    pthread_create(&mouse_thrd, NULL, mouse_thread, NULL);
    while (! setjmp(buf))
    {
       cnt++;
       sleep(1);
    }
    printf("\n exiting program as you pressed right click 2times[%d]\n",cnt);
    return 0;
}
