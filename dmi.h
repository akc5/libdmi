#pragma once

#include <stdio.h>
#include <stdlib.h>

/* single memory instance */
typedef struct mi_s
{
	int size;
	void *b;
	struct mi_s *next;
} mi_t;

typedef struct dmi_s
{
	int micnt;
	mi_t *milist;
} dmi_t;
