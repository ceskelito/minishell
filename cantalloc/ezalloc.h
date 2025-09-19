#ifndef EZALLOC_H
# define EZALLOC_H

# include <stdlib.h>

# define NEW	0
# define CLEAN	1
# define FREE	2

/* garbage collector node */
typedef struct s_alloc
{
	void			*ptr;
	struct s_alloc	*next;
}	t_alloc;

/* Allocates size bytes of memory with malloc() and saves
** the pointer in a static linked list, before returning it.
** Returns NULL if malloc fails. */
void	*ezalloc(size_t size);

/* Allocates size * count bytes of memory with malloc(), fills
** everything with 0s and saves the pointer in a static linked list,
** before returning it. Returns NULL if malloc fails.*/
void	*ezcalloc(size_t size, size_t count);

/* Frees the memory of ptr, and delete the relative node from the list */
void	ezfree(void	*ptr);

/* Frees all the memory previously allocated by ezalloc and ezcalloc. */
void	ezcleanup(void);

#endif
