#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dmi.h"

#define DMIMAX 100

typedef enum dmi_type_s
{
	DMIT_RELOADABLE,
	DMIT_NON_RELOADABLE,
	DMIT_MAX = DMIT_NON_RELOADABLE
} dmi_type_t;

/* maximum number of dmi types supported */
static int dmitmax;
/* array of dmi types */
static dmi_t *dmis;

static void* _dmi_init(const int f_dmitmax)
{
        dmitmax = f_dmitmax;
        if (dmitmax <= 0)
		{
			printf("Invalid max dmi type value. Init failure.\n");
			return NULL;
		}
        dmis = calloc(dmitmax, sizeof(dmi_t));
		if (!dmis)
			perror("calloc");
        return dmis;
}

/**
 * Initialize maximum number of dynamic memory instances that
 * need to be recored
 */
void* dmi_init(int dmitmax)
{
        return _dmi_init(dmitmax);
}

/**
 * Add the new memory instance to the linked list of its type
 * @dmi: base pointer of instance type
 * @mi: memory instance to be added
 */
static int _dmi_new(dmi_t *dmi, mi_t *mi)
{
        if (!dmi->milist)
        {
                dmi->milist = mi;
                dmi->micnt++;
				dmi->last = dmi->milist;
                return 0;
        }
		if (dmi->last)
		{
			dmi->last->next = mi;
			dmi->micnt++;
			dmi->last = dmi->last->next;
			return 0;
		}
		printf("Error occured while recording new dmi\n");
        return 1;
}

void display_dmi(int type, FILE *fp)
{
        dmi_t *dmi = &dmis[type];
        mi_t *head = dmi->milist;
        int cnt = dmi->micnt;
		printf("Displaying dmis at %p, cnt: %d\n", dmi, dmi->micnt);
        while (cnt--)
        {
				fprintf(fp, "type: %d, size: %d, b: %p\n", type, head->size, head->b);
                head = head->next;
        }
		printf("Type: %d, total alloc count: %lu, total alloc size: %lu\n", type, dmi->mi_alloc_count, dmi->mi_alloc_tsz);
		printf("Type: %d, total free count: %lu, total free size: %lu\n", type, dmi->mi_free_count, dmi->mi_free_tsz);
        return;
}

/**
 * Allocate a new memory instance, populate it with required values and record it
 */
static int _dmi_record(int type, size_t size, void *b)
{
	dmi_t *dmi = &dmis[type];
	mi_t *mi = calloc(1, sizeof(mi_t));
	if (!mi)
	{
		perror("_dmi_record calloc failure");
		return 1;
	}
	mi->b = b;
	mi->size = size;
	mi->next = NULL;
	if (_dmi_new(dmi, mi))
	{
		free(mi);
		mi = NULL;
		return 1;
	}
	dmi->mi_alloc_count++;
	dmi->mi_alloc_tsz += size;
	return 0;
}

void* dmi_malloc(int type, size_t size)
{
	/* TODO: sanity checks on type and size */
	void *b = malloc(size);
	if (!b)
	{
		perror("malloc failure");
		goto exit_fail_dmi_malloc;
	}

	if (_dmi_record(type, size, b))
		goto exit_fail_dmi_malloc;
	return b;

exit_fail_dmi_malloc:
	if (b) free(b);
	b = NULL;
	return b;
}

void* dmi_calloc(int type, int cnt, int size)
{
	/* TODO: sanity checks on type and size */
	void *b = calloc(cnt, size);
	if (!b)
	{
		perror("calloc failure");
		goto exit_fail_dmi_calloc;
	}
	if (_dmi_record(type, size * cnt, b))
		goto exit_fail_dmi_calloc;
	return b;

exit_fail_dmi_calloc:
	if (b) free(b);
	b = NULL;
	return b;
}

static void _dmi_free(int type)
{
	int i;
	dmi_t *dmi = &dmis[type];
	mi_t *head = dmi->milist; 
	int micnt = dmi->micnt;
	/* free direction - first to last */
	while (head)
	{
		mi_t *next = head->next;
		free(head->b);
		dmi->mi_free_count++;
		dmi->mi_free_tsz += head->size;
		head->size = 0;
		head->next = NULL;
		free(head);
		if (!next) break;
		head = next;
	}
	dmi->micnt -= micnt;
}

void dmi_free(int type)
{
	_dmi_free(type);
}
