#include "filament.h"
#include "linked_list.h"
#include "common.h"

#define LARGE_TENSION_THRESH (KT_MT_SPRING*MONO_DIAM)

void forces_kts(unsigned long long step)
{
        int ii, jj;
	//mtcontrib is the contribution of MTs to kinetochore motion
        double mt_contrib_left = 0.;
        double mt_contrib_right = 0.;
	double mt_contrib_left_merotelic = 0.;
        double kt_kt_contrib = 0.;//kt kt contrib is the contribution of ktkt ints to kinetochore 0's motion 
        double temp1, temp2, temp3;


        ii = left_near_list.get_first_fil();
        while(ii != -1)
        {
		temp1 = 0.;
//Calculate "tension" regardless of whether or not attached/compressed because in principle, attachment rate can depend on what the tension will be
/********more importantly, tension is used to determine whether or not filaments are within attachment range***************/
                        temp1 = -1.*KT_MT_SPRING*(kt_position[0] - left_filaments[ii].get_tip_pos() - KT_MT_SPRING_LENGTH);

/*//120618: haven't used strong spring in a long time. leave it out for now
                        #if !AUTO_START_CATASTROPHE
                        if(kt_position[0] - left_filaments[ii].get_tip_pos() < KT_MT_STRONG_SPRING_LENGTH)
                                temp1 += -1.*KT_MT_STRONG_SPRING*(kt_position[0] - left_filaments[ii].get_tip_pos() - KT_MT_STRONG_SPRING_LENGTH);
                        #endif*/
                //to interact with kt, filaments must be attached or filaments must be pushing on kinetochore
                if(left_filaments[ii].get_attached() || (left_filaments[ii].get_tip_pos() + KT_MT_SPRING_LENGTH > kt_position[0]))
		{
                       //temp1 = -1.*KT_MT_SPRING*(kt_position[0] - left_filaments[ii].get_tip_pos() - KT_MT_SPRING_LENGTH);
                       mt_contrib_left += temp1;
			if(temp1 < 0.)//tension on filaments, kt is pulled
			{
			   if(step*dt > 1.5*TIME_DELAY)
			   {
			    left_filaments[ii].increase_time_tension(dt);
			    if(temp1 < -1.*LARGE_TENSION_THRESH)
				left_filaments[ii].increase_time_large_tension(dt);

			    if(left_filaments[ii].get_tension() < 0.) //this algorithm may need improvement..
				left_filaments[ii].increase_num_force_switches();
			   }
                        avg_tension_count++;
                        avg_tension += (temp1 - avg_tension)/avg_tension_count;
			}
			else//temp1 > 0, compression on filaments, kt is pushed
			{
                           if(step*dt > 1.5*TIME_DELAY)
                           {
			    left_filaments[ii].increase_time_compression(dt);
			    if(temp1 > LARGE_TENSION_THRESH)
				left_filaments[ii].increase_time_large_compression(dt);

                            if(left_filaments[ii].get_tension() > 0.)
                                left_filaments[ii].increase_num_force_switches();
			   }
                        avg_compression_count++;
                        avg_compression += (temp1 - avg_compression)/avg_compression_count;

			}
/*//120628 -- keep track of tension and compression separately... commented this out
			//this keeps track of all forces (not just tension, despite name of variable)
                        avg_tension_count++;
			avg_tension += (temp1 - avg_tension)/avg_tension_count;*/
		}// if(left_filaments[ii].get_attached() || (left_filaments[ii].get_tip_pos() + KT_MT_SPRING_LENGTH > kt_position[0]))

                //set tension regardless. attachment is handled internally. need "tension" to determine attachment rate of detached filaments.
                left_filaments[ii].set_tension(-1.*temp1);

	    ii = left_near_list.get_next_fil(ii);
        }//while loop over near filaments



if(NUMBER_OF_KTS > 1)
{
	ii = right_near_list.get_first_fil();
	while(ii != -1)
	{
		temp2 = 0;
                        temp2 = KT_MT_SPRING*(right_filaments[ii].get_tip_pos() - kt_position[1] - KT_MT_SPRING_LENGTH);
/*omit for now. 120702
#if !AUTO_START_CATASTROPHE
                         if(right_filaments[ii].get_tip_pos() - kt_position[1] < KT_MT_STRONG_SPRING_LENGTH)
                                temp2 += KT_MT_STRONG_SPRING*(right_filaments[ii].get_tip_pos() - kt_position[1] - KT_MT_STRONG_SPRING_LENGTH);
#endif
*/
                        //to interact with kt, filaments must be attached or filaments must be pushing on kinetochore
                        if(right_filaments[ii].get_attached() || (right_filaments[ii].get_tip_pos() - KT_MT_SPRING_LENGTH < kt_position[1]))
                        {
			        mt_contrib_right += temp2;

			  if(temp2 > 0.)//tension
			  {
			   if(step*dt > 1.5*TIME_DELAY)
			   {
				right_filaments[ii].increase_time_tension(dt);
				if(temp2 > LARGE_TENSION_THRESH)
					right_filaments[ii].increase_time_large_tension(dt);
				if(right_filaments[ii].get_tension() < 0.)//sign is correct because tension is the variable used to determine rates.
					right_filaments[ii].increase_num_force_switches();
			   }
	                        avg_tension2_count++;
        	                avg_tension2 += (temp2 - avg_tension2)/avg_tension2_count;
                	  }
                          else
			  {
                           if(step*dt > 1.5*TIME_DELAY)
                           {
                                right_filaments[ii].increase_time_compression(dt);
				if(temp2 < -1.*LARGE_TENSION_THRESH)
					right_filaments[ii].increase_time_compression(dt);
				
				if(right_filaments[ii].get_tension() > 0.)
					right_filaments[ii].increase_num_force_switches();
			   }
        	                avg_compression2_count++;
	                        avg_compression2 += (temp2 - avg_compression2)/avg_compression2_count;
			  }
			}
                        right_filaments[ii].set_tension(temp2);

	   ii = right_near_list.get_next_fil(ii);
	}//while loop for right filaments





if(MEROTELIC)
{
ii = left_merotelic_near_list.get_first_fil();
while(ii != -1)
{
	temp3 = 0.;

//note: i have the following in the above loops
//                        temp2 = KT_MT_SPRING*(right_filaments[ii].get_tip_pos() - kt_position[1] - KT_MT_SPRING_LENGTH);
//                        temp1 = -1.*KT_MT_SPRING*(kt_position[0] - left_filaments[ii].get_tip_pos() - KT_MT_SPRING_LENGTH);
	temp3 = -1.*KT_MT_SPRING*(kt_position[1] - left_merotelic_filaments[ii].get_tip_pos() - KT_MT_SPRING_LENGTH);
//fprintf(stderr, "fil %i attached=%i\n", ii, (int) left_merotelic_filaments[ii].get_attached());
//fprintf(stderr, "position %12.10f\n", left_merotelic_filaments[ii].get_tip_pos());


        if(left_merotelic_filaments[ii].get_attached() || (left_merotelic_filaments[ii].get_tip_pos() + KT_MT_SPRING_LENGTH > kt_position[1]))
	{
	   mt_contrib_left_merotelic += temp3;
	   if(temp3 < 0.)
	   {
		if(step*dt > 1.5*TIME_DELAY)
		{
		  left_merotelic_filaments[ii].increase_time_tension(dt);
		  if(temp3 < -1.*LARGE_TENSION_THRESH)
			left_merotelic_filaments[ii].increase_time_large_tension(dt);
		  if(left_merotelic_filaments[ii].get_tension() < 0.)
			left_merotelic_filaments[ii].increase_num_force_switches();
		}
		avg_tension_m_count++;
		avg_tension_m += (temp3 - avg_tension_m)/avg_tension_m_count;
	   }//temp3<0.
	   else//temp3>0.
	   {
		if(step*dt > 1.5*TIME_DELAY)
		{
                   left_merotelic_filaments[ii].increase_time_compression(dt);
                   if(temp3 > LARGE_TENSION_THRESH)
                     left_merotelic_filaments[ii].increase_time_large_compression(dt);
                   if(left_merotelic_filaments[ii].get_tension() > 0.)
                     left_merotelic_filaments[ii].increase_num_force_switches();
		}
                avg_compression_m_count++;
                avg_compression_m += (temp3 - avg_compression_m)/avg_compression_m_count;
	   }//temp3>0.
	}//attached || compressed

	left_merotelic_filaments[ii].set_tension(-1.*temp3);
	ii = left_merotelic_near_list.get_next_fil(ii);
}//while loop over left_merotelic
}//if(merotelic)




//an attractive and repulsive spring
          kt_kt_contrib = KT_KT_SPRING*(kt_position[1] - kt_position[0] - KT_KT_SPRING_LENGTH);
          if(kt_kt_contrib < 0.)
          {
            kt_kt_contrib = 0.;
//a repulsive only spring
            if(kt_position[1] - kt_position[0] - KT_KT_REPEL_SPRING_LENGTH < 0.)
                kt_kt_contrib = KT_KT_REPEL_SPRING*(kt_position[1] - kt_position[0] - KT_KT_REPEL_SPRING_LENGTH);
          }
}//numkts > 1


/*
if(max_force < fabs(dt/KT_DRAG*(mt_contrib_left + kt_kt_contrib)))
{
        if(max_force < 1.)
          if(fabs(dt/KT_DRAG*(mt_contrib_left + kt_kt_contrib)) > 1.)
                fprintf(stderr, "step %i large force %g\n", step, fabs(dt/KT_DRAG*(mt_contrib_left + kt_kt_contrib)));

        max_force = fabs(dt/KT_DRAG*(mt_contrib_left + kt_kt_contrib));

        #if DETAIL
        fprintf(stderr, "new max force at step %i, f=%g\n", step, dt/KT_DRAG*(mt_contrib_left + kt_kt_contrib));
        #endif
}
*/

/*
for(int iik=0; iik < left_merotelic_filaments.size(); iik++)
fprintf(stderr, "mtip %i pos %12.10f\n", iik, left_merotelic_filaments[iik].get_tip_pos());
fprintf(stderr, "kt1= %12.10f, left_mero_ contrib= %12.10f, right= %12.10f, ktkt=%12.10f\n", kt_position[1], mt_contrib_left_merotelic, mt_contrib_right, -1.*kt_kt_contrib);
*/
        kt_position[0] += dt/KT_DRAG*(mt_contrib_left + kt_kt_contrib);
        if(NUMBER_OF_KTS > 1)
	{
		if(!MEROTELIC)
                 kt_position[1] += dt/KT_DRAG*(mt_contrib_right - kt_kt_contrib);
		else
		 kt_position[1] += dt/KT_DRAG*(mt_contrib_right + mt_contrib_left_merotelic - kt_kt_contrib);
	}
        else if(step*dt > TIME_DELAY)//no load if double kinetochore 120703
        {
	        kt_position[0] += dt/KT_DRAG*LOAD;
	}


#if KT_RAND_MOTION
kt_rand_motion();
#endif

if(!NEGATIVE_ALLOWED || !INFINITE_BOX)
	check_kt_bc();

}






void kt_rand_motion()
{
        double randnum = unif_rand();
        
        if(randnum < 0.25)
        {
                kt_position[0] += sqrt_dt_over_zeta;
                if(NUMBER_OF_KTS > 1)
                kt_position[1] += sqrt_dt_over_zeta;
        }
        else if(randnum < 0.5)
        {
                kt_position[0] += sqrt_dt_over_zeta;
                if(NUMBER_OF_KTS > 1)
                kt_position[1] -= sqrt_dt_over_zeta;
        }
        else if(randnum < 0.75)
        {
                kt_position[0] -= sqrt_dt_over_zeta;
                if(NUMBER_OF_KTS > 1)
                kt_position[1] += sqrt_dt_over_zeta;
        }
        else
        {
                kt_position[0] -= sqrt_dt_over_zeta;
                if(NUMBER_OF_KTS > 1)
                kt_position[1] -= sqrt_dt_over_zeta;
        }
}




void check_kt_bc()
{
        if(kt_position[0] < 0.)
	{
              if(!NEGATIVE_ALLOWED)
                kt_position[0] = 0.;
	}
        else if(kt_position[0] > BOX_LENGTH)
	{
              if(!INFINITE_BOX)
                kt_position[0] = BOX_LENGTH;
	}

        if(NUMBER_OF_KTS > 1)
        {
                if(kt_position[1] < 0.)
		{
                      if(!NEGATIVE_ALLOWED)
                        kt_position[1] = 0.;
		}
                else if(kt_position[1] > BOX_LENGTH)
		{
                      if(!INFINITE_BOX)
                        kt_position[1] = BOX_LENGTH;
		}
        }
}
