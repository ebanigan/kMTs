#include "common.h"
#include "filament.h"


void write_restart(unsigned long long step)
{
FILE *restartfile;
char restartname[96];
int ii;

sprintf(restartname, "restart/restart%4.4i", rseed);
restartfile = fopen(restartname, "w");

for(ii = 0; ii < NUMBER_OF_KTS; ii++)
{
        fprintf(restartfile, "%g\n", kt_position[ii]);
}

for(ii = 0; ii < NUMBER_OF_FILAMENTS_PER_KT; ii++)
{
        fprintf(restartfile, "%i %g %i %i\n", ii, left_filaments[ii].get_tip_pos(), (int)(left_filaments[ii].get_growing()), (int)(left_filaments[ii].get_attached()));
}

if(NUMBER_OF_KTS > 1)
for(ii = 0; ii < NUMBER_OF_FILAMENTS_PER_KT; ii++)
{
        fprintf(restartfile, "%i %g %i %i\n", ii, right_filaments[ii].get_tip_pos(), (int)(right_filaments[ii].get_growing()), (int)(right_filaments[ii].get_attached()));
}

fflush(restartfile);
fclose(restartfile);

}






void read_restart()
{
FILE *restartfile;
char restartname[96];
int ii;
double position;
int growing_int, attached_int;
bool growing_bool, attached_bool;
int temp_id;

sprintf(restartname, "restart/restart%4.4i", rseed);
restartfile = fopen(restartname, "r");

set_common_variables();


for(ii = 0; ii < NUMBER_OF_KTS; ii++)
{
        fscanf(restartfile, "%lg", &position);
        kt_position.push_back(position);
}
for(ii = 0; ii < NUMBER_OF_FILAMENTS_PER_KT; ii++)
{
        fscanf(restartfile, "%i %lg %i %i", &temp_id, &position, &growing_int, &attached_int);

	//fprintf(stderr, "%i %g %i %i \n", temp_id, position, growing_int, attached_int);

        filament temp_obj1(position, 1.);
        left_filaments.push_back(temp_obj1);
        if(growing_int == 1)
                growing_bool = true;
        else
                growing_bool = false;
        if(attached_int == 1)
                attached_bool = true;
        else
                attached_bool = false;
        left_filaments[ii].set_growing(growing_bool);
        left_filaments[ii].set_attached(attached_bool);
}
fflush(restartfile);
fclose(restartfile);

for(ii = 0; ii < NUMBER_OF_FILAMENTS_PER_KT; ii++)
{
                if(left_filaments[ii].get_attached())
                        num_attached_left++;
                if(NUMBER_OF_KTS > 1)
                 if(right_filaments[ii].get_attached())
                        num_attached_right++;
}

construct_linked_lists();
}

