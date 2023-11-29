#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "dmi.h"
#include <jansson.h>
#include <signal.h>

/*
 * Iterate over json config
 *
 * */

typedef struct __attribute__((__packed__)) data_s 
{
	int a;
	int b;
	char c;
} data_t;

/* callback to compare data */
int data_cmp(void *d1, void *d2)
{
	printf("Comparing data:\n");
	return memcmp(d1, d2, sizeof(data_t));	
}

void sighup_cb(int arg)
{
	printf("SIGHUP called\n");
}

int main(void)
{
	dmi_type_t type = DMIT_RELOADABLE;
	dmi_t *dmi = dmi_init(DMIT_MAX);
	if (!dmi)
	{
		fprintf(stderr, "Error initializing dmi\n");
		return 1;
	}
	printf("DMI size: %ld\n", sizeof(data_t));
	for (int i = 0; i < 100; i++)
	{
		volatile void *b = dmi_calloc(type, 10, sizeof(data_t));
		if (!b)
		{
				perror("calloc\n");
				return 1;
		}
	}
	display_dmi(type, stdout);
	/* free all instances of type 1 */
	printf("Free dmi instances for type %d\n\n", type);
	dmi_free(type, NULL, NULL);
	display_dmi(type, stdout);
	return 0;
}
