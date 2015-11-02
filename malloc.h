#ifndef MALLOC_H_
# define MALLOC_H_

# include <unistd.h>

# define ALIGNMENT sizeof(void *)
# define PAGE getpagesize() * 2
# define HASH getpagesize()
# define MAGIC 0xFF00FF00
# define FAILED (void *)-1
# define RESIZE(size) ((size >= (size_t)HASH) ? ((size_t)HASH) : (size))
# define UNLOCK 1

void			*malloc(size_t size);
void			*expand_memory(size_t size);
void			show_alloc_mem(void);
void			print_errors(int);
void			*unlock_thread();
void			new_range();

typedef struct		s_alloc
{
  size_t		magic;
  size_t		size;
  struct s_alloc	*next;
  struct s_alloc	*prev;
  struct s_alloc	*p_align;
  struct s_alloc	*n_align;
}			t_alloc;

typedef struct		s_info
{
  t_alloc		*alloc;
  void			*offset;
  size_t		size;
  size_t		number;
  size_t		*hash;
  t_alloc		*last;
  t_alloc		*first;
  void                  *brkpoint;
}			t_info;

void			*split_memory(size_t size);
void			set_elem(t_alloc *alloc);
void			new_space_for_elem(t_alloc *alloc);
t_alloc			*find_best_fit(size_t index);

# define STRUCT_SIZE sizeof(t_alloc)

#endif /* !MALLOC_H_ */
