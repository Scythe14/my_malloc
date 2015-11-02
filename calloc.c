#include <string.h>
#include <pthread.h>
#include "malloc.h"

extern pthread_mutex_t		g_mutex;

void				*calloc(size_t nmemb, size_t size)
{
  void				*tmp;

  tmp = malloc(nmemb * size);
  pthread_mutex_lock(&g_mutex);
  if (!tmp)
    return (unlock_thread());
  memset(tmp, 0, nmemb * size);
  pthread_mutex_unlock(&g_mutex);
  return (tmp);
}
