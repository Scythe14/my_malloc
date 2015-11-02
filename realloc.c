#include <string.h>
#include <pthread.h>
#include "malloc.h"

extern pthread_mutex_t	g_mutex;
extern t_info		g_info;

static void		*resize_elem(t_alloc *alloc, size_t size, void *ptr)
{
  if (expand_memory(size - alloc->size) == FAILED)
    return (NULL);
  g_info.offset += (size - alloc->size);
  g_info.size += (size - alloc->size);
  alloc->size += (size - alloc->size);
  pthread_mutex_unlock(&g_mutex);
  return (ptr);
}

void			*realloc(void *ptr, size_t size)
{
  void			*new;
  t_alloc		*alloc;

  if (!ptr || !size)
    return (malloc(size));
  pthread_mutex_lock(&g_mutex);
  while (size % ALIGNMENT)
    ++size;
  alloc = ptr - STRUCT_SIZE;
  if (alloc->magic != (size_t)MAGIC)
    print_errors(UNLOCK);
  else if (alloc == g_info.last)
    return (resize_elem(alloc, size, ptr));
  new_space_for_elem(alloc);
  pthread_mutex_unlock(&g_mutex);
  if (!(new = malloc(size)))
    return (NULL);
  return (memmove(new, ptr, size));
}
