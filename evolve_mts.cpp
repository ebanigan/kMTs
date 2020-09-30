#include "common.h"
#include "filament.h"
#include "linked_list.h"


void evolve_mts(unsigned long long step)
{
int ii,jj;
double current_time = step*dt;

//fprintf(stderr, "em ll sizes %i %i\n", left_near_list.get_size(), left_far_list.get_size());
//exit(1);
//near list
	ii = left_near_list.get_first_fil();
//	fprintf(stderr, "%i\n", ii);
	while(ii != -1)
	{

		if(step%COARSE_DT == 0)
		{
                 avg_tip_pos_count++;
		 avg_tip_pos += (left_filaments[ii].get_tip_pos() - avg_tip_pos)/avg_tip_pos_count; 
		}
	
                if(left_filaments[ii].get_growing())
		{
			//fprintf(stderr, "%i ", ii);
                        left_filaments[ii].try_polymerize();
	                left_filaments[ii].try_switch(current_time);
			
                        if(step*dt > 1.5*TIME_DELAY)
                        {

			 if(left_filaments[ii].get_tension() > 0.)
			 {	
                          avg_tension_polym_count++;
                          avg_tension_polym += (left_filaments[ii].get_current_polym()*inv_dt - avg_tension_polym) / avg_tension_polym_count;
                          avg_tension_cat += (left_filaments[ii].get_current_cat()*inv_dt - avg_tension_cat) / avg_tension_polym_count;
			 }
			 else
			 {
                          avg_compression_polym_count++;
                          avg_compression_polym += (left_filaments[ii].get_current_polym()*inv_dt - avg_compression_polym) / avg_compression_polym_count;
                          avg_compression_cat += (left_filaments[ii].get_current_cat()*inv_dt - avg_compression_cat) / avg_compression_polym_count;
			 }
			}
		}
                else
		{
                        //fprintf(stderr, "%i ", ii);
                        left_filaments[ii].try_depolymerize();
	                left_filaments[ii].try_switch(current_time);

                        if(step*dt > 1.5*TIME_DELAY)
                        {

                         if(left_filaments[ii].get_tension() > 0.)
                         {
                          avg_tension_depolym_count++;
                          avg_tension_depolym += (left_filaments[ii].get_current_depolym()*inv_dt - avg_tension_depolym)/ avg_tension_depolym_count;
                          avg_tension_res += (left_filaments[ii].get_current_res()*inv_dt - avg_tension_res) / avg_tension_depolym_count;
			 }
			 else
			 {
                          avg_compression_depolym_count++;
                          avg_compression_depolym += (left_filaments[ii].get_current_depolym()*inv_dt - avg_compression_depolym) / avg_compression_depolym_count;
                          avg_compression_res += (left_filaments[ii].get_current_res()*inv_dt - avg_compression_res) / avg_compression_depolym_count;
			 }
			}
		}

                left_filaments[ii].try_attdet(current_time);

		#if AUTO_START_CATASTROPHE
                if(left_filaments[ii].get_tip_pos() > kt_position[0]-MONO_DIAM)
                        left_filaments[ii].start_catastrophe();
		#endif

		if((left_filaments[ii].get_tip_pos() > 0.)|| NEGATIVE_ALLOWED)
		   left_filaments[ii].change_tip_pos(-1.*left_filaments[ii].get_polarization()*POLEWARD_FLUX_VELOCITY*dt);
		
		ii = left_near_list.get_next_fil(ii);
        //fprintf(stderr, "%i\n", ii);
	}

	if(NUMBER_OF_KTS > 1)
	{
	 ii = right_near_list.get_first_fil();
         while(ii != -1)
         {
/*
                if(step%COARSE_DT == 0)
                {
                 avg_tip_pos_count++;
                 avg_tip_pos += (right_filaments[ii].get_tip_pos() - avg_tip_pos)/avg_tip_pos_count;
                }
*/

                if(right_filaments[ii].get_growing())
                {
                        right_filaments[ii].try_polymerize();
                }
                else
                {
                        right_filaments[ii].try_depolymerize();
                }
                right_filaments[ii].try_switch(current_time);
                right_filaments[ii].try_attdet(current_time);

                #if AUTO_START_CATASTROPHE
                if(right_filaments[ii].get_tip_pos() < kt_position[1]-MONO_DIAM)
                        right_filaments[ii].start_catastrophe();
                #endif

                if((right_filaments[ii].get_tip_pos() < BOX_LENGTH)|| INFINITE_BOX)
                   right_filaments[ii].change_tip_pos(-1.*right_filaments[ii].get_polarization()*POLEWARD_FLUX_VELOCITY*dt);

                ii = right_near_list.get_next_fil(ii);
         }

	  if(MEROTELIC)
	  {
	    ii = left_merotelic_near_list.get_first_fil();
	    while(ii != -1)
	    {
		if(left_merotelic_filaments[ii].get_growing())
		{
		   left_merotelic_filaments[ii].try_polymerize();
		}
		else
		{
		   left_merotelic_filaments[ii].try_depolymerize();
		}
		left_merotelic_filaments[ii].try_switch(current_time);
		left_merotelic_filaments[ii].try_attdet(current_time);
                #if AUTO_START_CATASTROPHE
//???
                #endif
		if((left_merotelic_filaments[ii].get_tip_pos() > 0.) || NEGATIVE_ALLOWED)
		{
		  left_merotelic_filaments[ii].change_tip_pos(-1.*left_merotelic_filaments[ii].get_polarization()*POLEWARD_FLUX_VELOCITY*dt);
	//	fprintf(stderr, "fluxed %i\n", ii);
		}

		ii = left_merotelic_near_list.get_next_fil(ii);
	    }//while loop for left merotelic	
	  }//merotelic

	}//if( NUMKTS > 1



//far list has same stuff except for att/det dynamics and "auto catastrophe"
//also, kinetics are coarse grained and are checked less frequently
	if(step%COARSE_DT == 0)
	{
	 ii = left_far_list.get_first_fil();

	 while(ii != -1)
	 {

                 avg_tip_pos_count++;
                 avg_tip_pos += (left_filaments[ii].get_tip_pos() - avg_tip_pos)/avg_tip_pos_count;


                if(left_filaments[ii].get_growing())
                        left_filaments[ii].try_polymerize();
                else
                        left_filaments[ii].try_depolymerize();
                left_filaments[ii].try_switch(current_time);

                if((left_filaments[ii].get_tip_pos() > 0.)|| NEGATIVE_ALLOWED)
                   left_filaments[ii].change_tip_pos(-1.*left_filaments[ii].get_polarization()*POLEWARD_FLUX_VELOCITY*COARSE_DT*dt);

                ii = left_far_list.get_next_fil(ii);

	 }

	 if(NUMBER_OF_KTS > 1)
	 {
          ii = right_far_list.get_first_fil();

          while(ii != -1)
          {
//                 avg_tip_pos_count++;
//                 avg_tip_pos += (left_filaments[ii].get_tip_pos() - avg_tip_pos)/avg_tip_pos_count;

                if(right_filaments[ii].get_growing())
                        right_filaments[ii].try_polymerize();
                else
                        right_filaments[ii].try_depolymerize();
                right_filaments[ii].try_switch(current_time);


                if((right_filaments[ii].get_tip_pos() < BOX_LENGTH)|| INFINITE_BOX)
                   right_filaments[ii].change_tip_pos(-1.*right_filaments[ii].get_polarization()*POLEWARD_FLUX_VELOCITY*COARSE_DT*dt);

                ii = right_far_list.get_next_fil(ii);
          }//right while loop

	  if(MEROTELIC)
	  {
		ii = left_merotelic_far_list.get_first_fil();
		while(ii != -1)
		{
		  if(left_merotelic_filaments[ii].get_growing())
		      left_merotelic_filaments[ii].try_polymerize();
		  else
		      left_merotelic_filaments[ii].try_depolymerize();
		  left_merotelic_filaments[ii].try_switch(current_time);
		
		  if((left_merotelic_filaments[ii].get_tip_pos() > 0.) || NEGATIVE_ALLOWED)
		     left_merotelic_filaments[ii].change_tip_pos(-1.*left_merotelic_filaments[ii].get_polarization()*POLEWARD_FLUX_VELOCITY*COARSE_DT*dt);

		ii = left_merotelic_far_list.get_next_fil(ii);
		}//while..
	  }//merotelic
	 }//numkts>1
	}



/*
                if(NUMBER_OF_KTS > 1)
                {

//This needs to be modified in the two kinetochore case
                if(right_filaments[ii].get_growing())
                        right_filaments[ii].try_polymerize();
                else
                        right_filaments[ii].try_depolymerize();
                right_filaments[ii].try_switch();
                right_filaments[ii].try_attdet();
                }
#if AUTO_START_CATASTROPHE
                if(NUMBER_OF_KTS > 1)
                if(right_filaments[ii].get_tip_pos() < kt_position[1]-MONO_DIAM)
                        right_filaments[ii].start_catastrophe();
#endif
//fprintf(stderr, "%i %i %g %g %g %g %g %g\n", step, ii, left_filaments[ii].calc_polym(), right_filaments[ii].calc_polym(), left_filaments[ii].get_tip_pos(), right_filaments[ii].get_tip_pos(), kt_position[0], kt_position[1]);

        }

*/


}//end of evolve_mts()

