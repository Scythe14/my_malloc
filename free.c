#include <pthread.h>
#include "malloc.h"

extern pthread_mutex_t	g_mutex;
extern t_info		g_info;

t_alloc			*find_best_fit(size_t index)
{
  index = RESIZE(index);
  if (g_info.hash[index / ALIGNMENT - 1])
    return ((t_alloc *)g_info.hash[index / ALIGNMENT - 1]);
  index += STRUCT_SIZE + ALIGNMENT;
  index = RESIZE(index);
  while (index / ALIGNMENT - 1 < HASH / ALIGNMENT - 1
	 && !g_info.hash[index / ALIGNMENT - 1])
    index += ALIGNMENT;
  return ((t_alloc *)g_info.hash[index / ALIGNMENT - 1]);
}

void			new_space_for_elem(t_alloc *alloc)
{
  t_alloc		*tmp;

  alloc->magic = (size_t)~MAGIC;
  if (!g_info.hash[RESIZE(alloc->size) / ALIGNMENT - 1])
    {
      g_info.hash[RESIZE(alloc->size) / ALIGNMENT - 1] = (size_t)alloc;
      alloc->next = NULL;
      alloc->prev = NULL;
    }
  else
    {
      tmp = (t_alloc *)g_info.hash[RESIZE(alloc->size) / ALIGNMENT - 1];
      alloc->next = tmp;
      tmp->prev = alloc;
      alloc->prev = NULL;
      g_info.hash[RESIZE(alloc->size) / ALIGNMENT - 1] = (size_t)alloc;
    }
}

void			set_elem(t_alloc *alloc)
{
  if (alloc->prev)
    alloc->prev->next = alloc->next;
  else
    g_info.hash[RESIZE(alloc->size) / ALIGNMENT - 1] = (size_t)alloc->next;
  if (alloc->next)
    alloc->next->prev = alloc->prev;
  alloc->magic = (size_t)MAGIC;
  alloc->next = NULL;
  alloc->prev = NULL;
}

int			fusion_part(t_alloc *alloc)
{
  t_alloc		*next_elem;

  if (alloc)
    return (-1);
  next_elem = alloc->n_align;
  if (!next_elem || next_elem->magic == (size_t)MAGIC)
    return (-1);
  set_elem(next_elem);
  set_elem(alloc);
  alloc->n_align = next_elem->n_align;
  if (alloc->n_align)
    alloc->n_align->p_align = alloc;
  alloc->size += STRUCT_SIZE + next_elem->size;
  new_space_for_elem(alloc);
  return (0);
}

void			free(void *ptr)
{
  t_alloc		*alloc;

  if (!ptr)
    return ;
  pthread_mutex_lock(&g_mutex);
  alloc = (t_alloc *)(ptr - STRUCT_SIZE);
  if (alloc->magic != MAGIC)
    print_errors(UNLOCK);
  if (fusion_part(alloc))
    new_space_for_elem(alloc);
  if (alloc->p_align && alloc->p_align->magic == (size_t)~MAGIC)
    fusion_part(alloc->p_align);
  pthread_mutex_unlock(&g_mutex);
}
