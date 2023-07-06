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

int dmitmax;
dmi_t *dmis;

static void* _dmi_init(const int f_dmitmax)
{
	dmitmax = f_dmitmax;
	if (!dmitmax || (dmitmax < 0))
		return NULL;
	dmis = calloc(dmitmax, sizeof(dmi_t));	
	return dmis;
}

static void* dmi_init(int dmitmax)
{
	return _dmi_init(dmitmax);
}

typedef int (*dmi_new_sanity_check)(int type, int cnt, int size);

/*
 * @dmi: base pointer of instance type
 * @mi: memory instance to be added
 * */
static int _dmi_new(dmi_t *dmi, mi_t *mi)
{
	mi_t *tmp;
	mi_t *head = dmi->milist;
	if (!dmi->milist)
	{
		dmi->milist = mi;
		dmi->micnt++;
		return 0;
	}
	while (head->next)
	{
		head = head->next;
	}
	head->next = mi;
	dmi->micnt++;
	return 0;
}

static int dmi_new(int type, int cnt, size_t size, dmi_new_sanity_check sccb)
{
	if (!dmis)
	{
		printf("DMI not initialized\n");
		return 1;
	}
	if (sccb && sccb(type, cnt, size))
	{
		printf("Sanity check failed\n");	
		return 1;
	}
	int idx = type-1;
	dmi_t *dmi = &dmis[idx];
	printf("adding mi at index: %d\n", idx);
	mi_t *mi = calloc(1, sizeof(mi_t));
	void *b = calloc(cnt, size);
	mi->size = cnt * size;
	mi->b = b;
	mi->next = NULL;

	return _dmi_new(dmi, mi);
}

static void display_dmi(int type)
{
	dmi_t *dmi = &dmis[type-1];
	mi_t *head = dmi->milist;
	int cnt = dmi->micnt;
	printf("type: %d, cnt: %d\n", type-1, dmi->micnt);
	while (cnt--)
	{
		printf("size: %d, b: %p\n", head->size, head->b);
		head = head->next;
	}
	return;
}
