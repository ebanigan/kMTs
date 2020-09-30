#include "common.h"
#include "filament.h"

void print_mt_stats()
{
int ii;
FILE *mtstatsfile;
char mtstatsname[72];

double inv_TIME = 1./(TIME-1.5*TIME_DELAY);
if(NUMBER_OF_KTS > 1)
	inv_TIME = 1./TIME;

double inv_steps = 1./total_time_steps;

sprintf(mtstatsname, "/var/tmp/scratch/fvkt/mtstats%4.4i", rseed);
mtstatsfile = fopen(mtstatsname, "w");


	fprintf(mtstatsfile, "%g %g %g %g %g %g %g %g ", avg_tension_polym, avg_tension_depolym, avg_tension_res, avg_tension_cat, avg_compression_polym, avg_compression_depolym, avg_compression_res, avg_compression_cat); 
	fprintf(mtstatsfile, "%g %g %g %g\n", (double) avg_tension_polym_count * inv_steps, (double) avg_tension_depolym_count * inv_steps, (double) avg_compression_polym_count * inv_steps, (double) avg_compression_depolym_count * inv_steps);  

for(ii = 0; ii < left_filaments.size(); ii++)
{
	fprintf(mtstatsfile, "%g %g %g %g %g %g %g %g\n", (double) left_filaments[ii].get_num_switches() * inv_TIME, left_filaments[ii].get_time_growing() * inv_TIME, left_filaments[ii].get_time_attached() * inv_TIME, left_filaments[ii].get_time_tension() * inv_TIME, left_filaments[ii].get_time_compression() * inv_TIME, left_filaments[ii].get_time_large_tension() * inv_TIME, left_filaments[ii].get_time_large_compression() * inv_TIME, (double) left_filaments[ii].get_num_force_switches() * inv_TIME);
}


if(NUMBER_OF_KTS > 1)
{
for(ii = 0; ii < right_filaments.size(); ii++)
{
        fprintf(mtstatsfile, "%g %g %g %g %g %g %g %g\n", (double) right_filaments[ii].get_num_switches() * inv_TIME, right_filaments[ii].get_time_growing() * inv_TIME, right_filaments[ii].get_time_attached() * inv_TIME, right_filaments[ii].get_time_tension() * inv_TIME, right_filaments[ii].get_time_compression() * inv_TIME, right_filaments[ii].get_time_large_tension() * inv_TIME, right_filaments[ii].get_time_large_compression() * inv_TIME, (double) right_filaments[ii].get_num_force_switches() * inv_TIME);
}

}



fflush(mtstatsfile);
fclose(mtstatsfile);


}
