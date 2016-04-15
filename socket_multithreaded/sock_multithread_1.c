#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define MAX_MSG_SIZE 64
int sockId;

int
socket_create (const char *filename)
{
  struct sockaddr_un name;
  int sock;
  size_t size;

  /* Create the socket. */
  sock = socket (PF_LOCAL, SOCK_DGRAM, 0);
  if (sock < 0)
    {
      perror ("socket");
      exit (EXIT_FAILURE);
    }

  /* Bind a name to the socket. */
  name.sun_family = AF_LOCAL;
  strncpy (name.sun_path, filename, sizeof (name.sun_path));
  name.sun_path[sizeof (name.sun_path) - 1] = '\0';

  /* The size of the address is
     the offset of the start of the filename,
     plus its length (not including the terminating null byte).
     Alternatively you can just do:
     size = SUN_LEN (&name);
   */
  size = (offsetof (struct sockaddr_un, sun_path) + strlen (name.sun_path));
  unlink (name.sun_path);
  if (bind (sock, (struct sockaddr *) &name, size) < 0)
    {
      perror ("bind");
      exit (EXIT_FAILURE);
    }

  return sock;
}
void *
recvThrd (void *targ)
{

  int *thrd_id;
  int s,i;
  struct sockaddr_un remote;
  int rem_size;
  char rxMsg[MAX_MSG_SIZE];
  int len, seqNum;

  thrd_id = (int *) targ;
  s = pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
  if (s != 0)
    perror ("pthread_setcancelstate");

  while (1)
    {
      rem_size = sizeof (remote);
      /* do stuff */
      if ((len =
	   recvfrom (sockId, (void *) &rxMsg, MAX_MSG_SIZE, 0,
		     (struct sockaddr *) &remote, &rem_size)) < 0)
	{
	  if (errno == EINTR)
	    continue;
	  else
	    {
	      perror ("recvfrom");
	      exit (1);
	    }
	}
        printf("\n RCVD MSG FROM %s MSG[%s] \n",remote.sun_path,rxMsg);
      }
 }

int main()
{
  int rc,len=0;
  pthread_t thrdId;
  int rlen, nbytes;
  struct sockaddr_un remote;
  char txMsg[MAX_MSG_SIZE];
 
   sockId = socket_create ("/tmp/premtest_1");
  // create a receiver thread so that link can listen
  rc = pthread_create (&thrdId, NULL, recvThrd,
		       (void *) &thrdId);
  if (rc)
    {
      exit (-1);
    }

  remote.sun_family = AF_UNIX;
  strcpy (remote.sun_path, "/tmp/premtest_2");
  rlen = strlen (remote.sun_path) + sizeof (remote.sun_family);

  while(1)
  {
     scanf("%s", txMsg);
     len = strlen(txMsg);
     if (txMsg == "exit")
     {
         break;
     }
    
     if ((nbytes = sendto (sockId, txMsg, len, 0,
  			  (struct sockaddr *) &remote, rlen)) < 0)
     {
        perror ("sendto (channel)");
        exit (1);
     }
  }
  if(pthread_cancel(thrdId) !=0)
     perror("pthread_cancel");
  close(sockId); 
}
