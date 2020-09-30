#include "common.h"
#include "filament.h"

void print_distrib_vs_time(unsigned long long sim_step)
{
        double g_mt_pos_avg = 0.;
        double s_mt_pos_avg = 0.;
        double g_mt_pos_var = 0.;
        double s_mt_pos_var = 0.;
        double g_tot_conc = 0.;
        double s_tot_conc = 0.;

	double g_att_conc = 0.;
	double s_att_conc = 0.;
	double right_g_att_conc = 0.;
	double right_s_att_conc = 0.;

        double right_g_mt_pos_avg = 0.;
        double right_s_mt_pos_avg = 0.;
        double right_g_mt_pos_var = 0.;
        double right_s_mt_pos_var = 0.;
        double right_g_tot_conc = 0.;
        double right_s_tot_conc = 0.;

        int ii;
        int max_ii = NUMBER_OF_FILAMENTS_PER_KT;

        for(ii = 0; ii < max_ii; ii++)
        {
		if(left_filaments[ii].get_growing())
		{
                	g_mt_pos_avg += (left_filaments[ii].get_tip_pos() - kt_position[0]);
	                g_mt_pos_var += (left_filaments[ii].get_tip_pos() - kt_position[0])*(left_filaments[ii].get_tip_pos() - kt_position[0]);
	                g_tot_conc += 1.0;
	
			if(left_filaments[ii].get_attached())
				g_att_conc += 1.0;		

		}
		else
		{
	                s_mt_pos_avg += (left_filaments[ii].get_tip_pos() - kt_position[0]);
                	s_mt_pos_var += (left_filaments[ii].get_tip_pos() - kt_position[0])*(left_filaments[ii].get_tip_pos() - kt_position[0]);
	                s_tot_conc += 1.0;

			if(left_filaments[ii].get_attached())
				s_att_conc += 1.0;
		}
                if(NUMBER_OF_KTS > 1)
		{
		  if(right_filaments[ii].get_growing())
		  {
                        right_g_mt_pos_avg += (right_filaments[ii].get_tip_pos() - kt_position[1]);
                        right_g_tot_conc += 1.0;
                        right_g_mt_pos_var += (right_filaments[ii].get_tip_pos() - kt_position[1])*(right_filaments[ii].get_tip_pos() - kt_position[1]);
			if(right_filaments[ii].get_attached())
				right_g_att_conc += 1.0;

		  }
		  else
		  {
                        right_s_mt_pos_avg += (right_filaments[ii].get_tip_pos() - kt_position[1]);
                        right_s_tot_conc += 1.0;
                        right_s_mt_pos_var += (right_filaments[ii].get_tip_pos() - kt_position[1])*(right_filaments[ii].get_tip_pos() - kt_position[1]);
			if(right_filaments[ii].get_attached())
				right_s_att_conc += 1.0;
		  }
                }
        }

        g_mt_pos_avg *= 1./g_tot_conc;
        s_mt_pos_avg *= 1./s_tot_conc;
        g_mt_pos_var *= 1./g_tot_conc;
        s_mt_pos_var *= 1./s_tot_conc;
        g_mt_pos_var -= g_mt_pos_avg*g_mt_pos_avg;
        s_mt_pos_var -= s_mt_pos_avg*s_mt_pos_avg;

	if(!finite(g_mt_pos_avg)) g_mt_pos_avg = 0.;
        if(!finite(s_mt_pos_avg)) s_mt_pos_avg = 0.;
        if(!finite(g_mt_pos_var)) g_mt_pos_var = 0.;
        if(!finite(s_mt_pos_var)) s_mt_pos_var = 0.;

        if(NUMBER_OF_KTS > 1)
        {
	right_g_mt_pos_avg *= 1./right_g_tot_conc;
        right_s_mt_pos_avg *= 1./right_s_tot_conc;
        right_g_mt_pos_var *= 1./right_g_tot_conc;
        right_s_mt_pos_var *= 1./right_s_tot_conc;
        right_g_mt_pos_var -= right_g_mt_pos_avg*right_g_mt_pos_avg;
        right_s_mt_pos_var -= right_s_mt_pos_avg*right_s_mt_pos_avg;

        if(!finite(right_g_mt_pos_avg)) right_g_mt_pos_avg = 0.;
        if(!finite(right_s_mt_pos_avg)) right_s_mt_pos_avg = 0.;
        if(!finite(right_g_mt_pos_var)) right_g_mt_pos_var = 0.;
        if(!finite(right_s_mt_pos_var)) right_s_mt_pos_var = 0.;
        }


        fprintf(distribfile, "%g %g %g %g %g %g %g", sim_step*dt, g_mt_pos_avg, s_mt_pos_avg, g_mt_pos_var, s_mt_pos_var, g_att_conc, s_att_conc);
       if(NUMBER_OF_KTS > 1)
       {
        fprintf(distribfile, " %g %g %g %g %g %g", right_g_mt_pos_avg, right_s_mt_pos_avg, right_g_mt_pos_var, right_s_mt_pos_var, right_g_att_conc, right_s_att_conc);
       }
        fprintf(distribfile, "\n");


fflush(distribfile);

}//end of func
