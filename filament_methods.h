#include "filament.h"

void filament::try_polymerize()
{
        //LHS is +/-tip pos, RHS is BOX_LENGTH or 0
        //pol=+1, straightforward
        //pol=-1, if tip gets past 0, pos is negative, and -1*negative is not < 0

//        fprintf(stderr, "kt %g, kt-mt %g, t %g, k+ %g\n", kt_position[0], kt_position[0] - tip_pos, tension, calc_polym());

        if((polarization*tip_pos < 0.5*(BOX_LENGTH + polarization*BOX_LENGTH)) || INFINITE_BOX)
        if(unif_rand() < calc_polym())
        {
          tip_pos += polarization*MONO_DIAM;
        }
}

void filament::try_depolymerize()
{
        bool pos_pol_cond = (((polarization > 0) && (tip_pos > 0)) || NEGATIVE_ALLOWED);
        bool neg_pol_cond = ((polarization < 0) && (tip_pos < BOX_LENGTH));

//        fprintf(stderr, "kt %g, kt-mt %g, t %g, k- %g\n", kt_position[0], kt_position[0] - tip_pos, tension, calc_depolym());

        if(pos_pol_cond || neg_pol_cond)
        if(unif_rand() < calc_depolym())
                tip_pos -= polarization*MONO_DIAM;
}

void filament::try_switch(double time_in)
{
/*
if(growing)
	        fprintf(stderr, "kt %g, kt-mt %g, t %g, kc %g\n", kt_position[0], kt_position[0] - tip_pos, tension, calc_switch());
else
        	fprintf(stderr, "kt %g, kt-mt %g, t %g, kr %g\n", kt_position[0], kt_position[0] - tip_pos, tension, calc_switch());
*/


        if(unif_rand() < calc_switch())
        {
	        switch_mode(time_in);
	}
}

void filament::try_attdet(double time_in)
{
        if(unif_rand() < calc_attdet())
                attdet(time_in);
}

double filament::calc_polym() 
{
        double base_rate, exp_factor;
        if(growing)
                base_rate = growth_polym_rate;
        else
                base_rate = shrinkage_polym_rate;
        if(attached || (tension < 0.))
                exp_factor = exp(tension*POLYM_EXP_FACTOR);
        else
                exp_factor = 1.;

	current_polym = base_rate * exp_factor;
        //fprintf(stderr, "p rate %g, %i\n", base_rate * exp_factor, (int) get_attached());
        return current_polym;
}


double filament::calc_depolym() 
{
        double base_rate, exp_factor;
        if(growing)
                base_rate = growth_depolym_rate;
        else
                base_rate = shrinkage_depolym_rate;
        if(attached || (tension < 0.))
                exp_factor = exp(tension*DEPOLYM_EXP_FACTOR);
        else
                exp_factor = 1.;


	current_depolym = base_rate * exp_factor;
        //fprintf(stderr, "d rate %g, %i\n", base_rate * exp_factor,(int) get_attached());
        return current_depolym;
}

double filament::calc_switch() 
{
        double base_rate, exp_factor;
	double current_rate;
        if(growing)
        {
                base_rate = catastrophe_rate;
                if(attached || (tension < 0.))
                        exp_factor = exp(tension*CATASTROPHE_EXP_FACTOR);
                else
                        exp_factor = 1.;

		current_cat = base_rate * exp_factor;
		current_rate = current_cat;
        }
        else
        {
                base_rate = rescue_rate;
                if(attached || (tension < 0.))
                        exp_factor = exp(tension*RESCUE_EXP_FACTOR);
                else
                        exp_factor = 1.;

		current_res = base_rate * exp_factor;
		current_rate = current_res;
        }
        //fprintf(stderr, "s rate %g, %i\n", base_rate * exp_factor,(int) get_attached());
        return current_rate;
}


double filament::calc_attdet() const
{
        double base_rate, exp_factor;
        if(attached || (tension < 0.))
        {
                if(growing)
                {
                        base_rate = growth_det_rate;
                        exp_factor = exp(tension*GDET_EXP_FACTOR);
                }
                else
                {
                        base_rate = shrinkage_det_rate;
                        exp_factor = exp(tension*SDET_EXP_FACTOR);
                }
        }
        else
        {
                if(growing)
                {
                        if(tension / KT_MT_SPRING + KT_MT_SPRING_LENGTH < ATTRANGE)//+ktmt spring length if att range refers to distance from kineto
                        {
                                base_rate = growth_att_rate;
                                exp_factor = exp(tension*ATT_EXP_FACTOR);
                        }
                        else
                        {
                                base_rate = 0.;
                                exp_factor = 1.;
                        }
                }
                else
                {
                        if(tension / KT_MT_SPRING +KT_MT_SPRING_LENGTH < ATTRANGE)
                        {
                                base_rate = shrinkage_att_rate;
                                exp_factor = exp(tension*ATT_EXP_FACTOR);
                        }
                        else
                        {
                                base_rate = 0.;
                                exp_factor = 1.;
                        }
                }
        }

        //fprintf(stderr, "ad rate %g, %i\n", base_rate * exp_factor,(int) get_attached());
        return (base_rate*exp_factor);
}

