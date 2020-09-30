#include "common.h"
#include "linked_list.h"
#include "filament.h"

void update_linked_lists()
{
//add/delete from near list
//add/delete corresponding filaments from far list
//reset kinetics of those added to near list
//coarse grain kinetics of those added to far list

int ii,jj;

ii = left_near_list.get_first_fil();
while(ii != -1)
{
	jj = left_near_list.get_next_fil(ii);

	if((kt_position[0] - left_filaments[ii].get_tip_pos() > DISTANT_THRESHOLD) && !left_filaments[ii].get_attached())
	{
//	fprintf(stderr, "deleting from near list\n");
		left_near_list.delete_fil(ii);
		left_far_list.add_fil(ii);
		left_filaments[ii].coarse_grain_rates(COARSE_DT);
	}
        ii = jj;
}


ii = left_far_list.get_first_fil();
while(ii != -1)
{
        jj = left_far_list.get_next_fil(ii);

        if(kt_position[0] - left_filaments[ii].get_tip_pos() < DISTANT_THRESHOLD)
        {               
//	fprintf(stderr, "deleting from far list\n");
                left_far_list.delete_fil(ii);
                left_near_list.add_fil(ii);
                left_filaments[ii].reset_rates();
        }
	ii = jj;
}


if(NUMBER_OF_KTS > 1)
{
ii = right_near_list.get_first_fil();
while(ii != -1)
{
        jj = right_near_list.get_next_fil(ii);

        if((-kt_position[1] + right_filaments[ii].get_tip_pos() > DISTANT_THRESHOLD) && !right_filaments[ii].get_attached())
        {
//      fprintf(stderr, "deleting from near list\n");
                right_near_list.delete_fil(ii);
                right_far_list.add_fil(ii);
                right_filaments[ii].coarse_grain_rates(COARSE_DT);
        }
        ii = jj;
}


ii = right_far_list.get_first_fil();
while(ii != -1)
{
        jj = right_far_list.get_next_fil(ii);

        if(-kt_position[1] + right_filaments[ii].get_tip_pos() < DISTANT_THRESHOLD)
        {
//      fprintf(stderr, "deleting from far list\n");
                right_far_list.delete_fil(ii);
                right_near_list.add_fil(ii);
                right_filaments[ii].reset_rates();
        }
        ii = jj;
}


if(MEROTELIC)
{
ii = left_merotelic_near_list.get_first_fil();
while(ii != -1)
{
        jj = left_merotelic_near_list.get_next_fil(ii);

        if((kt_position[1] - left_merotelic_filaments[ii].get_tip_pos() > DISTANT_THRESHOLD) && !left_merotelic_filaments[ii].get_attached())
        {
                left_merotelic_near_list.delete_fil(ii);
                left_merotelic_far_list.add_fil(ii);
                left_merotelic_filaments[ii].coarse_grain_rates(COARSE_DT);
        }
        ii = jj;
}

ii = left_merotelic_far_list.get_first_fil();
while(ii != -1)
{
        jj = left_merotelic_far_list.get_next_fil(ii);

        if(kt_position[1] - left_merotelic_filaments[ii].get_tip_pos() < DISTANT_THRESHOLD)
        {
                left_merotelic_far_list.delete_fil(ii);
                left_merotelic_near_list.add_fil(ii);
                left_merotelic_filaments[ii].reset_rates();
        }
        ii = jj;
}
}//merotelic

}// numkts > 1


}


