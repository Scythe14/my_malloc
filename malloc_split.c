#include "malloc.h"

extern t_info		g_info;

static void		*new_elem_to_alloc(t_alloc *alloc, size_t size, t_alloc *tmp)
{
  t_alloc		*new;
  size_t		old_size;

  old_size = tmp->size;
  alloc = tmp;
  set_elem(alloc);
  alloc->size = size;
  new = (void *)alloc + STRUCT_SIZE + alloc->size;
  new->size = old_size - STRUCT_SIZE - alloc->size;
  new->next = NULL;
  new->prev = NULL;
  new->n_align = alloc->n_align;
  if (new->n_align)
    new->n_align->p_align = new;
  alloc->n_align = new;
  new->p_align = alloc;
  new_space_for_elem(new);
  if (g_info.last == alloc)
    g_info.last = new;
  return (alloc + 1);
}

void			*split_memory(size_t size)
{
  t_alloc		*alloc;
  t_alloc		*tmp;

  alloc = NULL;
  tmp = find_best_fit(size);
  if (!tmp)
    return (NULL);
  else if (tmp->size == size)
    {
      alloc = tmp;
      set_elem(alloc);
      return (alloc + 1);
    }
  else if (tmp->size < size || tmp->size < size + STRUCT_SIZE + ALIGNMENT)
    return (NULL);
  return (new_elem_to_alloc(alloc, size, tmp));
}

void			new_range()
{
  if (sbrk(0) != g_info.brkpoint && g_info.brkpoint)
    {
      g_info.brkpoint = sbrk(0);
      g_info.offset = g_info.brkpoint;
      g_info.size = 0;
      g_info.number = 0;
    }
}
