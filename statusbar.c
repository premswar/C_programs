#include<stdio.h>
#include <unistd.h>

void printstatus ();

int
main ()
{
  int count = 0;
  printf ("Will load in 10 Sec \nLoading ");
  for (count; count < 10; ++count)
    {
      printf (". ");
      fflush (stdout);
      sleep (1);
    }
  printf ("\nDone\n");
  printstatus ();
  return 0;
}

void
printstatus ()
{
  float progress = 0.0;
  int barWidth = 70, pos, i;
  while (progress <= 1.0)
    {
      printf ("[");
      pos = barWidth * progress;
      for (i = 0; i < barWidth; ++i)
	{
	  if (i < pos)
	    printf ("=");
	  else if (i == pos)
	    printf (">");
	  else
	    printf (" ");
	}
      printf ("] %d%% \r", (int) (progress * 100.0));
      fflush (stdout);
      sleep (1);
      progress += 0.1;		// for demonstration only
    }
  printf ("\n");
}

/*
 * Stand alone API to use in any external function
 * to Display the status
 * USAGE: 
        if(i%10 == 0)
         printstatus(i, parts); 
*/
#if 0 
void
printstatus (int crnt, int total)
{
  int progress = 0, factr;
  int barWidth = 70, pos, i;
  factr = total / 10;
  progress = crnt / factr;
  if (progress <= 10.0)
    {
      printf ("[");
      pos = 0.1 * barWidth * progress;
      for (i = 0; i < barWidth; ++i)
	{
	  if (i < pos)
	    printf ("=");
	  else if (i == pos)
	    printf (">");
	  else
	    printf (" ");
	}
      printf ("] %d%% \r", (int) (progress * 10));
      fflush (stdout);
      //sleep(1);
      //progress += 0.16; // for demonstration only
    }
  //printf("\n");
}
#endif
