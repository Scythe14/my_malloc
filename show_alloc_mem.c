#include <stdio.h>
#include "malloc.h"

extern t_info	g_info;

void		show_alloc_mem(void)
{
  t_alloc	*list;

  if (!(list = g_info.first))
    return ;
  printf("Break : %p\n", sbrk(0));
  while (list)
    {
      if (list->magic == (size_t)MAGIC)
	printf("0x%lX - 0x%lX, %ld octets\n",
	       (size_t)list,
	       (size_t)list + list->size,
	       list->size);
      list = list->n_align;
    }
}
