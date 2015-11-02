#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "malloc.h"

pthread_mutex_t		g_mutex = PTHREAD_MUTEX_INITIALIZER;
t_info			g_info;

void			*expand_memory(size_t size)
{
  size_t		i;
  void			*tmp;

  i = 0;
  while (g_info.number * (size_t)PAGE < g_info.size + size + (size_t)STRUCT_SIZE)
    {
      ++i;
      ++g_info.number;
    }
  if (i)
    {
      tmp = sbrk(i * PAGE);
      g_info.brkpoint = tmp + PAGE * i;
      return (tmp);
    }
  return (NULL);
}

static t_alloc		*create_node_to_allocate(size_t size)
{
  t_alloc		*alloc;

  if (expand_memory(size) == FAILED)
    return (NULL);
  alloc = g_info.offset;
  alloc->next = NULL;
  alloc->prev = NULL;
  alloc->p_align = NULL;
  alloc->n_align = NULL;
  alloc->magic = MAGIC;
  alloc->size = size;
  return (alloc);
}

static void		*init_list(size_t size)
{
  void			*tmp;
  t_alloc		*alloc;

  if ((tmp = split_memory(size)))
    return (tmp);
  if (!(alloc = create_node_to_allocate(size)))
    return (NULL);
  alloc->p_align = g_info.last;
  if (g_info.last)
    g_info.last->n_align = alloc;
  if (!g_info.first)
    g_info.first = alloc;
  g_info.last = alloc;
  g_info.offset += (STRUCT_SIZE + size);
  g_info.size += (STRUCT_SIZE + size);
  return ((void *)alloc + STRUCT_SIZE);
}

void			*unlock_thread()
{
  pthread_mutex_unlock(&g_mutex);
  return (NULL);
}

void			*malloc(size_t size)
{
  void			*tmp;

  new_range();
  pthread_mutex_lock(&g_mutex);
  if ((long int)size < 0)
    return (unlock_thread());
  while (size % ALIGNMENT || !size)
    ++size;
  if (!g_info.alloc)
    {
       if ((g_info.hash = sbrk(HASH)) == FAILED)
	 return (unlock_thread());
       memset(g_info.hash, 0, HASH);
       if ((g_info.offset = expand_memory(size)) == FAILED)
	 return (unlock_thread());
       if (!(tmp = init_list(size)))
	 return (unlock_thread());
       g_info.alloc = tmp - STRUCT_SIZE;
       pthread_mutex_unlock(&g_mutex);
       return (tmp);
    }
  if (!(tmp = init_list(size)))
    return (unlock_thread());
  pthread_mutex_unlock(&g_mutex);
  return (tmp);
}
