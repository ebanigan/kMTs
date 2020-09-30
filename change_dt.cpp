#include "common.h"
#include "filament.h"
#include "linked_list.h"

void change_dt(unsigned long long *step)
{
	int jj, ii;
	double k_times_n = (pow(KT_MT_SPRING, 1.11) * num_attached_left + fabs(LOAD) + (NUMBER_OF_KTS-1)*KT_KT_SPRING)/ KT_DRAG;
	if(NUMBER_OF_KTS > 1)
	  k_times_n *= 2.;
	double old_dt = dt;
	double current_time = *step*dt;
	bool dt_changed = false;

	if(k_times_n < 600)
	{
	     if(cl_dt < .999*1.e-3)
	     {
		cl_dt = 1.e-3;
		dt_changed = true;
		min_dt = true;
	     }
	}
	else if((k_times_n < 1220) && (KT_MT_SPRING < 75.))
	{
	     if(cl_dt < .999*5.e-4)
	     {
		cl_dt = 5.e-4;
                dt_changed = true;
	     }
	}
	else if(k_times_n < 2250)
	{
	    if(cl_dt < .999*1.e-4)
	    {
		cl_dt = 1.e-4;
                dt_changed = true;
	    }
	}
	else if(k_times_n < 12000)
	{
	    if(cl_dt < .999*5.e-5)
	    {
		cl_dt = 5.e-5;
                dt_changed = true;
	    }
	}
	else if(k_times_n < 85000)
	{
	    if(cl_dt < .999*2.5e-5)
	    {
		cl_dt = 2.5e-5;
                dt_changed = true;
	    }
	}

if(dt_changed)
{
	cl_numskip = (int)(cl_numskip*(old_dt / cl_dt));
	*step = current_time/cl_dt;
	


/*        ii = left_near_list.get_first_fil();
        while(ii != -1)
        {
                left_filaments[ii].reset_rates();
                ii = left_near_list.get_next_fil(ii);
        }
*/
	
	for(ii = 0; ii < left_filaments.size(); ii++)
	  left_filaments[ii].reset_rates();

        ii = left_far_list.get_first_fil();
        while(ii != -1)
        {
                left_filaments[ii].coarse_grain_rates(COARSE_DT);
                ii = left_far_list.get_next_fil(ii);
        }

		
	if(NUMBER_OF_KTS > 1)
	{
		for(ii = 0; ii < right_filaments.size(); ii++)
			right_filaments[ii].reset_rates();

        	ii = right_far_list.get_first_fil();
       		while(ii != -1)
        	{
              		right_filaments[ii].coarse_grain_rates(COARSE_DT);
                	ii = right_far_list.get_next_fil(ii);
        	}
	}

	sqrt_dt_over_zeta = sqrt(2. * KbT * dt / KT_DRAG);

	fprintf(stderr, "t= %g, k_times_n = %g, switching dt from %g to %g\n", *step*dt, k_times_n, old_dt, dt);
}


}

