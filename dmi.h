#pragma once

/* single memory instance */
typedef struct mi_s
{
	int size;
	void *b;
	struct mi_s *next;
} mi_t;

/* base pointer of a 'type' to a linked list of memory instance */
typedef struct dmi_s
{
	int micnt;
	mi_t *milist;
} dmi_t;
