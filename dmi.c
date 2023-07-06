#include "dmi.h"

#define DMIMAX 100

typedef enum dmi_type_s
{
	DMIT_ONE,
	DMIT_TWO,
	DMIT_MAX
} dmi_type_t;

int main(void)
{
	dmi_t *dmi = dmi_init(10);
	if (!dmi)
	{
		printf("Error initializing dmi\n");
		return 1;
	}
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
	return 0;
}
