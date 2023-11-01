#include <stdio.h>
#include <string.h>
#include "dmi.h"

typedef struct data_s
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

int main(void)
{
	dmi_t *dmi = dmi_init(10);
	if (!dmi)
	{
		fprintf(stderr, "Error initializing dmi\n");
		return 1;
	}
	for (int i = 0; i < 100; i++)
	{
		volatile void *b = dmi_calloc(1, 10, sizeof(data_t));
		if (!b)
				printf("error\n");
	}
	printf("check 1\n");
	/* free all instances of type 1 */
	dmi_free(1, NULL, NULL);
	printf("check 2\n");
	data_t d = {1,2,'C'};
	data_t d1 = {12, 2,'C'};
	data_t d2 = {5, 5, 'D'};
	int cnt = 0;
	volatile void *b = dmi_calloc(1, 10, sizeof(data_t));
	if (!b)
			printf("error\n");
	memcpy((void *)b, &d, sizeof(data_t));
	volatile void *b1 = dmi_calloc(1, 10, sizeof(data_t));
	if (!b1)
			printf("error\n");
	memcpy((void *)b1, &d1, sizeof(data_t));
	volatile void *b2 = dmi_calloc(1, 10, sizeof(data_t));
	if (!b2)
			printf("error\n");
	memcpy((void *)b2, &d2, sizeof(data_t));
	/*
	for (int i = 0; i < 100; i++)
	{
		volatile void *b = dmi_calloc(1, 10, sizeof(data_t));
		if (!b)
				printf("error\n");
		if (!cnt)
		{
			memcpy((void *)b, &d1, sizeof(data_t));
			cnt++;
		}
		else
			memcpy((void *)b, &d, sizeof(data_t));
	}
	*/
	//printf("check 3, cnt: %d\n", cnt);
	/* populate some of these instances */
	/* free a specific instance of type 1 */
	dmi_free(1, NULL, NULL);
	//dmi_free(1, &d2, data_cmp);
	printf("check 4\n");
	display_dmi(1, stdout);
	printf("check 5\n");
	return 0;
}
