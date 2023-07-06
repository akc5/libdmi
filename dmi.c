#include <stdio.h>
#include <stdlib.h>
#include "dmi.h"

#define DMIMAX 100

typedef enum dmi_type_s
{
	DMIT_RELOADABLE,
	DMIT_NON_RELOADABLE,
	DMIT_MAX = DMIT_NON_RELOADABLE
} dmi_type_t;

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

void* dmi_init(int dmitmax)
{
        return _dmi_init(dmitmax);
}

/* Not used anymore, was needed for dmi_new()
typedef int (*dmi_new_sanity_check)(int type, int cnt, int size);
*/

/*
 * @dmi: base pointer of instance type
 * @mi: memory instance to be added
 * */
static int _dmi_new(dmi_t *dmi, mi_t *mi)
{
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
/*
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
        int idx = type;
        dmi_t *dmi = &dmis[idx];
        printf("adding mi at index: %d\n", idx);
        mi_t *mi = calloc(1, sizeof(mi_t));
        void *b = calloc(cnt, size);
        mi->size = cnt * size;
        mi->b = b;
        mi->next = NULL;

        return _dmi_new(dmi, mi);
}
*/

void display_dmi(int type, FILE *fp)
{
        dmi_t *dmi = &dmis[type];
        mi_t *head = dmi->milist;
        int cnt = dmi->micnt;
        while (cnt--)
        {
				fprintf(fp, "type: %d, size: %d, b: %p\n", type, head->size, head->b);
                head = head->next;
        }
        return;
}

static int _dmi_alloc(int type, size_t size, void *b)
{
	dmi_t *dmi = &dmis[type];
	mi_t *mi = calloc(1, sizeof(mi_t));
	if (!mi)
	{
		perror("_dmi_alloc calloc failure");
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

	if (_dmi_alloc(type, size, b))
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
	if (_dmi_alloc(type, size * cnt, b))
		goto exit_fail_dmi_calloc;
	return b;

exit_fail_dmi_calloc:
	if (b) free(b);
	b = NULL;
	return b;
}

