#include <stdio.h>
#include "dmi.h"

int main(void)
{
	dmi_t *dmi = dmi_init(10);
	if (!dmi)
	{
		fprintf(stderr, "Error initializing dmi\n");
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
	for (int i = 0; i < 1000; i++)
	{
		volatile void *b = dmi_calloc(1, 10, 10);
		if (!b)
				printf("error\n");
	}
	display_dmi(1, stdout);
	return 0;
}
