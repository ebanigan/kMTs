#include "common.h"

void print_distrib_data()
{
int ii;
/*if(!(NEGATIVE_ALLOWED || INFINITE_BOX))
{*/
        char distribname[96];
        FILE *distribfile;

        char distribname_all[96];
        FILE *distribfile_all;

        char distribname_det[96];
        FILE *distribfile_det;

        sprintf(distribname, "output/mtdistrib%4.4i", rseed);
        distribfile = fopen(distribname, "w");

        sprintf(distribname_all, "output/mtdistrib_all%4.4i", rseed);
        distribfile_all = fopen(distribname_all, "w");

        sprintf(distribname_det, "output/mtdistrib_det%4.4i", rseed);
        distribfile_det = fopen(distribname_det, "w");

        for(ii = 0; ii < 2*2*BOX_LENGTH; ii++)
        {
        if((0.5*(ii+1)-2000.>-100) && (0.5*(ii+1)-2000. < 100))
        {

//I know N_attached, so I can get a number distribution of attached filaments
                mtdistrib[ii] *= 1./(1.0*mtdistrib_count);
                fprintf(distribfile, "%g %g\n", 0.5*(ii+1)-2000., mtdistrib[ii]);

//Then, by subtracting attached MT num distrib from the all MT num distrib, I can get the detached MT num distrib
                mtdistrib_all[ii] *= 1./(NUMBER_OF_FILAMENTS_PER_KT*mtdistrib_count_all);
                fprintf(distribfile_all, "%g %g\n", 0.5*(ii+1)-2000., mtdistrib_all[ii]);

//in this way I can see all attached and all compressed filaments and all nearby filaments...

                mtdistrib_det[ii] *= 1./(1.0*mtdistrib_count_det);
                fprintf(distribfile_det, "%g %g\n", 0.5*(ii+1)-2000., mtdistrib_det[ii]);
        }
        }

        fflush(distribfile);
        fclose(distribfile);
        fflush(distribfile_all);
        fclose(distribfile_all);
        fflush(distribfile_det);
        fclose(distribfile_det);
/*}*/

}
