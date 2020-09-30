#include "common.h"

void check_finite(unsigned long long step)
{
                if(!finite(kt_position[0]))
                {
                        fprintf(stderr, "kt 1 not finite, step %llu\n", step);
                        exit(1);
                }
                if(NUMBER_OF_KTS > 1)
                if(!finite(kt_position[1]))
                {
                        fprintf(stderr, "kt 2 not finite, step %llu\n", step);
                        exit(1);
                }
}

