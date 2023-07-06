#pragma once

/*
 * mi_t: a single dynamic memory instance
 * @ size: size of the instance
 * @ b: pointer to memory blob/slab/block
 * @ next: pointer to next instance of the same type
 **/
typedef struct mi_s
{
	int size;
	void *b;
	struct mi_s *next;
} mi_t;

/*
 * dmi_t: dynamic memory instances of a type
 * @ micnt: number of dynamic memory instances added to this list.
 * @ milist: based pointer to the list.
 **/
typedef struct dmi_s
{
	int micnt;
	mi_t *milist;
} dmi_t;

/*
 * Return a pointer to allocated memory.
 * @ type: Type of dynamic memory instance.
 * @ size: size of memory requested.
 **/
void* dmi_malloc(int type, size_t size);
