#include "filament.h"

void phosphorylate()
{
	int ii;

	for(ii = 0; ii < NUMBER_OF_FILAMENTS_PER_KT; ii++)
	{
		left_filaments[ii].phosphorylate_dam1();
		right_filaments[ii].phosphorylate_dam1();
	}

}

