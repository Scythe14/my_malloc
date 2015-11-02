#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include "malloc.h"

extern pthread_mutex_t	g_mutex;

static int		how_many_digits(int value)
{
  int			result;

  result = 0;
  while (value != 0)
    {
      value /= 10;
      ++result;
    }
  return (result);
}

void			print_errors(int param)
{
  char			filename[5 + how_many_digits(getpid()) + 6];
  char			buffer[1024];
  int			fd_file;
  int			ret;

  printf("**** Invalid free *****\n");
  sprintf(filename, "/proc/%d/maps", getpid());
  if ((fd_file = open(filename, O_RDONLY)) < 0)
    abort() ;
  memset(buffer, 0, 1024);
  while ((ret = read(fd_file, buffer, 1024)) > 0)
    {
      write(2, buffer, ret);
      memset(buffer, 0, 1024);
    }
  if (param == UNLOCK)
    pthread_mutex_unlock(&g_mutex);
  abort();
}
