#include "common.h"
#include "RNG_taus.h"

class filament
{
        protected:
//base rates
                double growth_polym_rate;
                double growth_depolym_rate;
                double shrinkage_polym_rate;
                double shrinkage_depolym_rate;
                double rescue_rate;
                double catastrophe_rate;
                double growth_att_rate;
                double shrinkage_att_rate;
                double growth_det_rate;
                double shrinkage_det_rate;
//current rates, so they can be grabbed easily after calculation
		double current_polym;
		double current_depolym;
		double current_cat;
		double current_res;

                bool growing;
                bool attached;
                double tip_pos;
//              double prev_tip_pos;
                double tension;
//              double prev_tension;
                double polarization;//tells us which way + ends point

		//Filament statistics
		unsigned long long num_switches;
		double time_growing;
		double time_tension;
		double time_compression;
		double time_large_tension;
		double time_large_compression; 
		double time_of_last_switch;
		double time_of_last_attdet;
		double time_attached;
		
		unsigned long long num_force_switches;//number of times force on filament switches between tension/compression (or 0)

        public:
                filament(double start, double direction)
                {
                        if(unif_rand() < 0.5)
                                growing = true;
                        else
                                growing = false;
                        #if (FIL_START_RAND_LENGTH || FIL_START_0)
                        attached = false;
                        #else
                        if(unif_rand() < 1.5)
                                attached = true;
                        else
                                attached = false;
                        #endif
                        growth_polym_rate = GPOLYM*dt;
                        growth_depolym_rate = GDEPOLYM*dt;
                        shrinkage_polym_rate = SPOLYM*dt;
                        shrinkage_depolym_rate = SDEPOLYM*dt;
                        rescue_rate = RESCUE*dt;
                        catastrophe_rate = CATASTROPHE*dt;
                        growth_att_rate = GATT*dt;
                        shrinkage_att_rate = SATT*dt;
                        growth_det_rate = GDET*dt;
                        shrinkage_det_rate = SDET*dt;

			current_polym = 0.;
			current_depolym = 0.;

                        tension = 0.;
//                      prev_tension = prev_tension;
                        tip_pos = start;
//                      prev_tip_pos = tip_pos;
                        polarization = direction;
			
			reset_nums_and_times(0.);
                }
                void reset_rates()
                {
                        growth_polym_rate = GPOLYM*dt;
                        growth_depolym_rate = GDEPOLYM*dt;
                        shrinkage_polym_rate = SPOLYM*dt;
                        shrinkage_depolym_rate = SDEPOLYM*dt;
                        rescue_rate = RESCUE*dt;
                        catastrophe_rate = CATASTROPHE*dt;
                        growth_att_rate = GATT*dt;
                        shrinkage_att_rate = SATT*dt;
                        growth_det_rate = GDET*dt;
                        shrinkage_det_rate = SDET*dt;
                }

		void coarse_grain_rates(int coarse_graining)
		{
			double factor = (double) coarse_graining;
                        growth_polym_rate *= factor;
                        growth_depolym_rate *= factor;
                        shrinkage_polym_rate *= factor;
                        shrinkage_depolym_rate *= factor;
                        rescue_rate *= factor;
                        catastrophe_rate *= factor;
                        growth_att_rate *= factor;
                        shrinkage_att_rate *= factor;
                        growth_det_rate *= factor;
                        shrinkage_det_rate *= factor;
		}


		void set_catastrophe_rate(double input){catastrophe_rate = input*dt;}
		void set_growth_det_rate(double input){growth_det_rate = input*dt;}
		void set_shrinkage_det_rate(double input){shrinkage_det_rate = input*dt;}

		void phosphorylate_dam1()
		{
                	set_catastrophe_rate(CATASTROPHE*2.5);
                	set_growth_det_rate(GDET*10.);
			set_shrinkage_det_rate(GDET*10.*1.6);
		}

                double calc_polym();
                double calc_depolym();
                double calc_switch();
                double calc_attdet() const;
                bool get_growing() const {return growing;}
                bool get_attached() const {return attached;}
                void set_growing(bool input){growing = input;}
                void set_attached(bool input){attached = input;}
                double get_tip_pos() const {return tip_pos;}
		void change_tip_pos(double amount){tip_pos = tip_pos + amount;}
//              double get_prev_tip_pos() const {return prev_tip_pos;}
		double get_polarization() const {return polarization;}
                double get_tension() const {return tension;}
                void set_tension(double t_in){tension = t_in;}//*polarization
//              double get_prev_tension const (){return prev_tension;}
                void try_polymerize();
                void try_depolymerize();
                void try_switch(double time_in);
                void try_attdet(double time_in);
                void switch_mode(double time_in)
		{
		  if(growing)//if currently growing, we're about to switch to shrinking
		  {
			increase_time_growing(time_in);
			current_depolym = calc_depolym();
	                growing = !growing;
			current_res = calc_switch();
		  }
		  else
		  {
			current_polym = calc_polym();
                        growing = !growing;
                        current_cat = calc_switch();
		  }
		  time_of_last_switch = time_in;
		  num_switches++;
		}
                void attdet(double time_in)
		{
			if(attached)
			  increase_time_attached(time_in);
			time_of_last_attdet = time_in;
			
			if(polarization > 0.)
			 num_attached_left -= (int) (2*((double)((int)attached) - 0.5));
			else
			 num_attached_right -= (int) (2*((double)((int)attached) - 0.5));

			attached = !attached;
		}
                void start_catastrophe(){growing = false;}
                void detach()
		{
		 attached = false; 
		 if(polarization > 0.)
		  num_attached_left--;
		 else
		  num_attached_right--;
		}
                void attach()
		{
		 attached = true; 
		 if(polarization > 0.)
		  num_attached_left++;
		 else
		  num_attached_right++;
		}

		void increase_time_growing(double time_in){time_growing += time_in - time_of_last_switch;}
		void increase_time_attached(double time_in){time_attached += time_in - time_of_last_attdet;}
		void increase_time_tension(double time_increment){time_tension += time_increment;}
		void increase_time_compression(double time_increment){time_compression += time_increment;}
		void increase_time_large_tension(double time_increment){time_large_tension += time_increment;}
		void increase_time_large_compression(double time_increment){time_large_compression += time_increment;}

		void increase_num_force_switches(){num_force_switches++;}

		unsigned long long get_num_switches() const {return num_switches;}
		double get_time_growing() const {return time_growing;}
		double get_time_attached() const {return time_attached;}
		double get_time_tension() const {return time_tension;}
		double get_time_compression() const {return time_compression;}
		double get_time_large_tension() const {return time_large_tension;}
		double get_time_large_compression() const {return time_large_compression;}
		unsigned long long get_num_force_switches() const {return num_force_switches;}
		
		double get_current_polym() const {return current_polym;}
		double get_current_depolym() const {return current_depolym;}
                double get_current_res() const {return current_res;}
                double get_current_cat() const {return current_cat;}

		void reset_nums_and_times(double current_time)
		{
                        num_switches = 0;
                        time_growing = 0.;
                        time_tension = 0.;
                        time_compression = 0.;
                        time_large_tension = 0.;
                        time_large_compression = 0.;
                        time_of_last_switch = current_time;
                        time_of_last_attdet = current_time;
                        time_attached = 0.;
                        num_force_switches = 0;
		}	

};


extern vector<filament> left_filaments;
extern vector<filament> right_filaments;
extern vector<filament> left_merotelic_filaments;

