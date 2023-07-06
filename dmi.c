#include <stdio.h>
#include <stdlib.h>
#include "dmi.h"

#define DMIMAX 100

typedef enum dmi_type_s
{
	DMIT_ONE,
	DMIT_TWO,
	DMIT_MAX
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

static void* dmi_init(int dmitmax)
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

static void display_dmi(int type)
{
        dmi_t *dmi = &dmis[type];
        mi_t *head = dmi->milist;
        int cnt = dmi->micnt;
        printf("type: %d, cnt: %d\n", type, dmi->micnt);
        while (cnt--)
        {
                printf("size: %d, b: %p\n", head->size, head->b);
                head = head->next;
        }
        return;
}

void* dmi_malloc(int type, size_t size)
{
        // sanity checks on type and size
        void *b = malloc(size);
        if (!b)
        {
                perror("malloc failure");
                goto exit_fail_dmi_malloc;
        }
        dmi_t *dmi = &dmis[type];
        mi_t *mi = calloc(1, sizeof(mi_t));
        if (!mi)
        {
                perror("calloc failure");
                goto exit_fail_dmi_malloc;
        }
        mi->b = b;
        mi->size = size;
        mi->next = NULL;

        if (_dmi_new(dmi, mi))
        {
                printf("failed to add new dynamic memory instance\n");
                return NULL;
        }
        return b;

exit_fail_dmi_malloc:
        if (b) free(b);
        if (mi) free(mi);
        return NULL;
}

int main(void)
{
	dmi_t *dmi = dmi_init(10);
	if (!dmi)
	{
		printf("Error initializing dmi\n");
		return 1;
	}
	/*
	dmi_new(1, 10, sizeof(int), NULL);
	dmi_new(1, 20, sizeof(int), NULL);
	dmi_new(1, 30, sizeof(int), NULL);
	display_dmi(1);
	dmi_new(2, 10, sizeof(int), NULL);
	dmi_new(3, 20, sizeof(int), NULL);
	dmi_new(4, 30, sizeof(int), NULL);
	display_dmi(2);
	display_dmi(3);
	display_dmi(4);
	*/
	void *b = dmi_malloc(2, 100); 
	display_dmi(2);
	return 0;
}
