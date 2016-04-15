/*
 * =====================================================================================
 *
 *       Filename:  sig_lib.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/30/2014 06:42:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Prem swaroop k (), premswaroop@asu.edu
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef SIG_LIB_H
  #define SIG_LIB_H
int mythread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
int mythread_sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact);
void mythread_exit(void *retval);
int  mythread_join(pthread_t thread, void **retval);

#endif
