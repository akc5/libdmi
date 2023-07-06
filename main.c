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
	void *b = dmi_malloc(2, 100); 
	display_dmi(2, stdout);
	b = dmi_calloc(3, 10, 300); 
	display_dmi(3, stdout);
	printf("b: %p\n", b);
	return 0;
}
