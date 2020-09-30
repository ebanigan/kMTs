#include "common.h"
#include "filament.h"

void tally_filament_distrib(unsigned long long step)
{
if(step > TIME_DELAY)
{
  int ii;
  double distance;
  int bin;
  for(ii = 0; ii < NUMBER_OF_FILAMENTS_PER_KT; ii++)
  {
    distance = left_filaments[ii].get_tip_pos() - kt_position[0];
    if(distance > 0.)
      bin = -1+(int)(2*(distance + BOX_LENGTH + 0.25));
    else
      bin = (int)(2*(BOX_LENGTH + distance - 0.25));


  if((bin < 0) || (bin >= (int) BOX_LENGTH * 2 * 2))
     continue;

    mtdistrib_all[bin] += 1.0;

  if(left_filaments[ii].get_attached())
  {
    mtdistrib[bin] += 1.0;
    mtdistrib_count += 1;
  }
  else
  {
    mtdistrib_det[bin] += 1;
    mtdistrib_count_det += 1.0;
  }


  }
    mtdistrib_count_all += 1;


}//timedelay
}


