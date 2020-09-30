#include "common.h"
#include "filament.h"
#include "linked_list.h"

void initialize()
{
        int ii;

set_common_variables();

        for(ii = 0; ii < NUMBER_OF_FILAMENTS_PER_KT; ii++)
        {
                #if FIL_START_RAND_LENGTH
                if(NUMBER_OF_KTS < 2)
                {
                  filament temp_obj1((int)((INITIAL_FILAMENT_LENGTH+1)*unif_rand()), 1.);
                  left_filaments.push_back(temp_obj1);
                }
                else
                {
                 filament temp_obj1((int)(unif_rand()*(0.5*(BOX_LENGTH-KT_KT_SPRING_LENGTH)+1)), 1.);
                 left_filaments.push_back(temp_obj1);

                 double rightfil_randompart = unif_rand()*(BOX_LENGTH-(int)(0.5*(BOX_LENGTH-KT_KT_SPRING_LENGTH))+1);
                 filament temp_obj2((int)(0.5*(BOX_LENGTH+KT_KT_SPRING_LENGTH))+(int)rightfil_randompart, -1.);
                 right_filaments.push_back(temp_obj2);
                }
                #elif FIL_START_0
                if(NUMBER_OF_KTS < 2)
                {
                        filament temp_obj1(0, 1.);
                        left_filaments.push_back(temp_obj1);
                }
                else
                {
                        fprintf(stderr, "Don't use FIL_START_0 option for double kt system. Exiting.\n");
                        exit(1);
                }
                #else // this is the usual starting condition
                if(NUMBER_OF_KTS < 2)
                {
                 filament temp_obj1(INITIAL_FILAMENT_LENGTH, 1.);
                 left_filaments.push_back(temp_obj1);
                }
                else
                {
		 #if (!(INFINITE_BOX && NEGATIVE_ALLOWED)) 
                 filament temp_obj1((int)(0.5*(BOX_LENGTH-KT_KT_SPRING_LENGTH-2.*KT_MT_SPRING_LENGTH)), 1.);
                 filament temp_obj2((int)(0.5*(BOX_LENGTH+KT_KT_SPRING_LENGTH+2.*KT_MT_SPRING_LENGTH)), -1.);
		 #else
		 filament temp_obj1((int)(-0.5.*KT_KT_SPRING_LENGTH-KT_MT_SPRING_LENGTH), 1.);
		 filament temp_obj2((int)(0.5*KT_KT_SPRING_LENGTH+KT_MT_SPRING_LENGTH), -1.);
		 #endif
                 left_filaments.push_back(temp_obj1);
                 right_filaments.push_back(temp_obj2);

		 if(MEROTELIC)
		 {
			if(ii < NUMBER_OF_MEROTELIC_FILAMENTS)
			{
			  filament temp_obj3((int)(0.5*(BOX_LENGTH+KT_KT_SPRING_LENGTH-2.*KT_MT_SPRING_LENGTH)), 1.);
			  left_merotelic_filaments.push_back(temp_obj3);
			}
		 }
                }
                #endif
                


                if(left_filaments[ii].get_attached())
                        num_attached_left++;
		if(NUMBER_OF_KTS > 1)
		{
		 if(right_filaments[ii].get_attached())
			num_attached_right++;
		 if(MEROTELIC)
		  if(ii < NUMBER_OF_MEROTELIC_FILAMENTS)
		   if(left_merotelic_filaments[ii].get_attached())
		     num_attached_left_merotelic++;
		}
        }
        #if FIL_START_0
        kt_position.push_back(0.);
        #else
        kt_position.push_back(INITIAL_FILAMENT_LENGTH);
        #endif
        #if !FIL_START_RAND_LENGTH
        kt_position[0]+= KT_MT_SPRING_LENGTH;
        #endif

        if(NUMBER_OF_KTS > 1)
        {
		#if (!(INFINITE_BOX && NEGATIVE_ALLOWED))
                kt_position[0] = (int)(0.5*(BOX_LENGTH-KT_KT_SPRING_LENGTH));
                kt_position.push_back((int)(0.5*(BOX_LENGTH+KT_KT_SPRING_LENGTH)));
		#else
		kt_position[0] = (int)(-0.5.*KT_KT_SPRING_LENGTH);
		kt_position.push_back((int)(0.5*KT_KT_SPRING_LENGTH));
		#endif
        }




        construct_linked_lists();


}//end of initialize()






void construct_linked_lists()
{
int ii;

//linked_list tempobj(NUMBER_OF_FILAMENTS_PER_KT);

//left_near_list = tempobj;
//left_far_list = tempobj;

left_near_list.initialize_lists(NUMBER_OF_FILAMENTS_PER_KT);
left_far_list.initialize_lists(NUMBER_OF_FILAMENTS_PER_KT);

if(NUMBER_OF_KTS > 1)
{
right_near_list.initialize_lists(NUMBER_OF_FILAMENTS_PER_KT);
right_far_list.initialize_lists(NUMBER_OF_FILAMENTS_PER_KT);

if(MEROTELIC)
{
left_merotelic_near_list.initialize_lists(NUMBER_OF_MEROTELIC_FILAMENTS);
left_merotelic_far_list.initialize_lists(NUMBER_OF_MEROTELIC_FILAMENTS);
}

}

//all filaments start on the near list
for(ii = 0; ii < NUMBER_OF_FILAMENTS_PER_KT; ii++)
{
	left_near_list.add_fil(ii);
	if(NUMBER_OF_KTS > 1)
	{	
	  right_near_list.add_fil(ii);
	  if(MEROTELIC)
	   if(ii < NUMBER_OF_MEROTELIC_FILAMENTS)
		left_merotelic_near_list.add_fil(ii);
	}
}

//fprintf(stderr, "cons ll sizes %i %i\n", left_near_list.get_size(), left_far_list.get_size());

}


void set_common_variables()
{
int ii;

        iseed0 = rseed+258;

max_force =  0.;
num_attached_left = 0;
num_attached_right = 0;
avg_tension = 0.;
avg_tension_count = 0;
avg_compression = 0.;
avg_compression_count = 0.;
avg_tension2 = 0.;
avg_tension2_count = 0;
avg_compression2 = 0.;
avg_compression2_count = 0.;
avg_tension_m = 0.;
avg_tension_m_count = 0;
avg_compression_m = 0.;
avg_compression_m_count = 0.;
avg_tip_pos = 0.;
avg_tip_pos_count = 0;

avg_tension_polym = 0.;
avg_tension_depolym = 0.;
avg_compression_polym = 0.;
avg_compression_depolym = 0.;
avg_tension_res = 0.;
avg_tension_cat = 0.;
avg_compression_res = 0.;
avg_compression_cat = 0.;

avg_tension_polym_count = 0;
avg_tension_depolym_count = 0;
avg_compression_polym_count = 0;
avg_compression_depolym_count = 0;

total_time_steps = 0;
mtstats_steps = 0;

        if(dt>.999e-3)
          min_dt = true;
        else
          min_dt = false;

        RNUM.set(iseed0);
//fprintf(stderr, "%g %g %g\n", unif_rand(), unif_rand(), unif_rand()); exit(1);
        mtdistrib_count_all = 0;
        mtdistrib_count = 0;
        mtdistrib_count_det = 0;
        for(ii = 0; ii < 2*2*BOX_LENGTH; ii++)
        {
          mtdistrib.push_back(0);
          mtdistrib_all.push_back(0);
          mtdistrib_det.push_back(0);
        }

        for(ii = 0; ii < rseed; ii++)
           double randnumber_initialize = unif_rand();

        sqrt_dt_over_zeta = sqrt(2. * KbT * dt / KT_DRAG);

	char mkscratch[128];
	sprintf(mkscratch, "mkdir /var/tmp/scratch/");
	system(mkscratch);

	char fvkt[128];
	sprintf(fvkt, "mkdir /var/tmp/scratch/fvkt/");
	system(fvkt);

        sprintf(outname, "/var/tmp/scratch/fvkt/ktpos%4.4i", rseed);
        outfile = fopen(outname, "w");

        /*sprintf(mtoutname, "output/mtpos%4.4i", rseed);
        mtoutfile = fopen(mtoutname, "w");
*/
        sprintf(attachedname, "/var/tmp/scratch/fvkt/attached%4.4i", rseed);
        attachedfile = fopen(attachedname, "w");

        sprintf(tensionname, "/var/tmp/scratch/fvkt/tension%4.4i", rseed);
        tensionfile = fopen(tensionname, "w");

	sprintf(distribname, "/var/tmp/scratch/fvkt/distrib_stats%4.4i", rseed);
	distribfile = fopen(distribname, "w");

}





void initialize_comm_line()
{
cl_gpolym = 0.07;
cl_sdepolym = 2.0;
cl_dt = 1.e-5;
cl_numskip = (long) (5*inv_dt);
cl_time = 10000.;
cl_ktmtspring = 10.;
cl_ktktspring = cl_ktmtspring;
cl_rseed = 2;
cl_numfil = 50;
cl_number_of_merotelic_filaments = 0;
cl_pexp = 0.05;
cl_dexp = -0.1;
cl_rexp = 0.2;
cl_cexp = -0.3;
cl_load = 0;
cl_numkts = 1;
cl_attrange = 20.;
cl_gdet = 0.001;
cl_gatt = 0.02;
cl_inf_box = false;
cl_neg_allowed = false;
cl_time_delay = 1e3;//50e3;
cl_drag = 1.;
cl_cat = 3.e-4;
cl_res = 2.e-3;
cl_sdetfactor=1.;
cl_gdetexp=0.;
cl_sdetexp=0.;
cl_delayed_phosphorylation = false;
cl_poleward_flux_velocity = 0.;
cl_merotelic = false;
}
