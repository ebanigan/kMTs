//#include "RNG_taus.cpp"
#include "common.h"
#include "linked_list.h"
#include "filament_methods.h"
//#include "RNG_taus_methods.h"


linked_list left_near_list;
linked_list left_far_list;

linked_list right_near_list;
linked_list right_far_list;

linked_list left_merotelic_near_list;
linked_list left_merotelic_far_list;

RNG_taus RNUM;

int cl_numkts;
int cl_rseed, cl_numfil, cl_numskip, cl_number_of_merotelic_filaments;
double cl_load, cl_pexp, cl_dexp, cl_rexp, cl_cexp, cl_time, cl_ktmtspring, cl_ktktspring;
double cl_attrange, cl_gdet, cl_gatt, cl_gpolym, cl_sdepolym, cl_cat, cl_res;
double cl_sdetfactor, cl_gdetexp, cl_sdetexp;
bool cl_delayed_phosphorylation;
double cl_poleward_flux_velocity;
bool cl_merotelic;

int num_attached_left, num_attached_right, num_attached_left_merotelic;
double avg_tension, avg_compression, avg_tension2, avg_compression2, avg_tension_m, avg_compression_m;
unsigned long long avg_tension_count, avg_compression_count, avg_tension2_count, avg_compression2_count, avg_tension_m_count, avg_compression_m_count;
double avg_tip_pos;
unsigned long long avg_tip_pos_count;

double avg_tension_polym, avg_tension_depolym, avg_compression_polym, avg_compression_depolym;
double avg_tension_res, avg_tension_cat, avg_compression_res, avg_compression_cat;
unsigned long long avg_tension_polym_count, avg_tension_depolym_count, avg_compression_polym_count, avg_compression_depolym_count;
//unsigned long long avg_tension_res_count, avg_tension_cat_count, avg_compression_res_count, avg_compression_cat_count;

double cl_dt;
bool cl_inf_box, cl_neg_allowed;
double cl_time_delay;
double cl_drag;

vector<double> mtdistrib;
vector<double> mtdistrib_all;
vector<double> mtdistrib_det;
unsigned long long mtdistrib_count;
unsigned long long mtdistrib_count_all;
unsigned long long mtdistrib_count_det;

unsigned long long total_time_steps;
unsigned long long mtstats_steps; 

vector<filament> left_filaments;
vector<filament> right_filaments;
vector<filament> left_merotelic_filaments;
vector<double> kt_position;
int iseed0;
double sqrt_dt_over_zeta;

FILE *outfile;
char outname[72];
FILE *mtoutfile;
char mtoutname[72];
FILE *attachedfile;
char attachedname[72];
FILE *tensionfile;
char tensionname[72];
FILE *distribfile;
char distribname[72];

double max_force;


bool min_dt;

int main(int argc, char **argv)
{
initialize_comm_line();

int option;
//o, all caps remaining except FGMRSP
while( (option = getopt(argc, argv, "s:l:f:w:x:y:z:t:k:p:n:r:u:d:a:b:g:m:h:q:e:c:j:v:F:S:G:R:P:M:i:")) != -1)
{
 switch(option)
 {
  case 'n':
    cl_numkts = atoi(optarg); break;
  case 's':
    cl_rseed = atoi(optarg); break;
  case 'l':
    cl_load = (double) atof(optarg); break;
  case 'f':
    cl_numfil = atoi(optarg); break;
  case 'w':
    cl_pexp = (double) atof(optarg); break;
  case 'x':
    cl_dexp = (double) atof(optarg); break;
  case 'y':
    cl_rexp = (double) atof(optarg); break;
  case 'z':
    cl_cexp = (double) atof(optarg); break;
  case 't':
    cl_time = (double) atof(optarg); break;
  case 'k':
    cl_numskip = atoi(optarg); break;
  case 'p':
    cl_ktmtspring = (double) atof(optarg); break;
  case 'v':
    cl_ktktspring = (double) atof(optarg); break;
  case 'r':
    cl_attrange = (double) atof(optarg); break;
  case 'u':
    cl_gdet = (double) atof(optarg); break;
  case 'd':
    cl_dt = (double) atof(optarg); break;
  case 'a':
    cl_gatt = (double) atof(optarg); break;
  case 'b':
    if(atoi(optarg) == 1)
      cl_inf_box = true;
    fprintf(stderr, "inf %i\n", cl_inf_box);
    break;
  case 'm':
    if(atoi(optarg) == 1)
      cl_neg_allowed = true;
    fprintf(stderr, "neg %i\n", cl_neg_allowed);
    break;
  case 'g':
     cl_gpolym = (double)atof(optarg); break;
  case 'e':
     cl_sdepolym = (double)atof(optarg); break;
  case 'c':
     cl_cat=(double)atof(optarg); break;
  case 'j':
     cl_res=(double)atof(optarg); break;
  case 'h':
     cl_time_delay = (double)atof(optarg); break;
  case 'q': 
     cl_drag = (double)atof(optarg); break;
  case 'F':
	cl_sdetfactor = (double)atof(optarg); 
	fprintf(stderr, "detfactor is %g\n", cl_sdetfactor);
	break;
  case 'G':
	cl_gdetexp = (double)atof(optarg); break;
  case 'S':
	cl_sdetexp = (double)atof(optarg); break;
  case 'R':
	if(atoi(optarg) == 1) cl_delayed_phosphorylation = true;
	break;
  case 'P':
	cl_poleward_flux_velocity = atof(optarg); break;
  case 'M':
	if(atoi(optarg) == 1) cl_merotelic = true; 
	break;
  case 'i':
	cl_number_of_merotelic_filaments = atoi(optarg);
	break;
  case '?':
    cerr <<  "Unknown option character '" << optopt << "'. ...exiting.\n";
    options_menu();
    return 1;
  default:
        abort();
 }
}
fprintf(stderr, "dt %g seed %i load %g fil %i w %g x %g y %g z %g det rate %g att rate %g springconst %g att range %g inf box %i neg %i polymrate %g depoly %g time delay %g ktdrag %g ktktspr %g sdetfactor %g detexps(G,S) %g %g\n", cl_dt, cl_rseed, cl_load, cl_numfil, cl_pexp, cl_dexp, cl_rexp, cl_cexp, cl_gdet, cl_gatt, cl_ktmtspring, cl_attrange, (int)cl_inf_box, (int)cl_neg_allowed, cl_gpolym, cl_sdepolym, cl_time_delay, cl_drag, cl_ktktspring, cl_sdetfactor, cl_gdetexp, cl_sdetexp);

	unsigned long long ii;
	long jj;
	int print_size = 0;
	
	if(RESTART)
	  read_restart();
	else
	  initialize();

fprintf(stderr, "out of init\n");

#if !DETAIL
        if(left_filaments.size() < 16)
#endif
           print_size = left_filaments.size();
#if !DETAIL
        else
           print_size = 15;
#endif

	bool times_not_yet_reset = true;


	for(ii = 0; ii*dt < TIME+.9*dt; ii++, total_time_steps++)
	{

                        if(DELAYED_PHOSPHORYLATION)
                        if(ii*dt > PHOSPHORYLATION_TIME)
                                phosphorylate();

		if(ii*dt > 1.5*TIME_DELAY)
		{
			mtstats_steps++;	
			if(times_not_yet_reset && (NUMBER_OF_KTS < 2))
			{
			  for(jj = 0; jj < left_filaments.size(); jj++)
				left_filaments[jj].reset_nums_and_times(ii*dt);
			  times_not_yet_reset = false;
			}
		}

		if(ii%NUMSKIP == 0)
		{

                        fprintf(attachedfile, "%g %i", ii*dt, num_attached_left);
			if(NUMBER_OF_KTS > 1)
			{
				fprintf(attachedfile, " %i", num_attached_right);
				if(MEROTELIC)
				  fprintf(attachedfile, " %i", num_attached_left_merotelic);
			}
			fprintf(attachedfile, "\n");

                        print_distrib_vs_time(ii);

                        fprintf(tensionfile, "%g %g %g %g %g", ii*dt, avg_tension, avg_compression, (double)(avg_tension_count)/NUMSKIP, (double)(avg_compression_count)/NUMSKIP);
                        if(NUMBER_OF_KTS > 1)
			{
                        	fprintf(tensionfile, " %g %g %g %g", avg_tension2, avg_compression2, (double)(avg_tension2_count)/NUMSKIP, (double)(avg_compression2_count)/NUMSKIP);
                        avg_tension2 = 0.;
                        avg_tension2_count = 0;
                        avg_compression2 = 0.;
                        avg_compression2_count = 0;
			   if(MEROTELIC)
			   {
				fprintf(tensionfile, " %g %g %g %g", avg_tension_m, avg_compression_m, (double)(avg_tension_m_count)/NUMSKIP, (double)(avg_compression_m_count)/NUMSKIP);
				avg_tension_m = 0.;
				avg_tension_m_count = 0;
				avg_compression_m = 0.;
				avg_compression_m_count = 0;
			   }
			}

			fprintf(tensionfile, "\n");

                        avg_tension = 0.;
                        avg_tension_count = 0;
                        avg_compression = 0.; 
                        avg_compression_count = 0;

	
                        if((ii%(500*NUMSKIP) == 0) && (ii != 0))
                        {
			  if(!min_dt)
                           change_dt(&ii);
			}
			else if(ii == NUMSKIP)
			{
			  if(!min_dt)
			    change_dt(&ii);
			}
		}

//		if(!(INFINITE_BOX || NEGATIVE_ALLOWED))
//		{
/*		
		if(num_attached_left == 0)
		{
		  fprintf(stderr, "Left detachment at t=%g. Exiting.\n", ii*dt);
//		  fprintf(attachedfile, "Left detachment at t=%g. Exiting.\n", ii*dt);
		  exit(1);
		}
		if(NUMBER_OF_KTS > 1)
		 if(num_attached_right == 0)
		 {
                   fprintf(stderr, "Right detachment at t=%g. Exiting.\n", ii*dt);
		   fprintf(attachedfile, "Right detachment at t=%g. Exiting.\n", ii*dt);
                   exit(1);
		 }
*/
//                }

                forces_kts(ii);
		evolve_mts(ii);

//fprintf(stderr, "%llu\n", ii);
	
		if(ii%(2*NUMSKIP) == 0)
		{

			check_finite(ii);
//fprintf(stderr, "hi\n");
			update_linked_lists();
//fprintf(stderr, "success\n");
//fprintf(stderr, "%g\n", ii*dt);
		}
		if(ii%NUMSKIP == 0)
		{
//fprintf(stderr, "%g\n", ii*dt);


////////////switch dts!!!



			if(ii%(1000*NUMSKIP) == 0)
			{
fprintf(stderr, "%g\n", ii*dt);
			 fflush(attachedfile);
			 fflush(tensionfile);
			}

			fprintf(outfile, "%g %12.10g", ii*dt, kt_position[0]);
			if(NUMBER_OF_KTS > 1)
				fprintf(outfile, " %12.10g", kt_position[1]);
			fprintf(outfile, "\n");

//                        fprintf(mtoutfile, "%g %g\n", ii*dt, avg_tip_pos);
			avg_tip_pos = 0.;
			avg_tip_pos_count = 0;

                        if(ii%(10*NUMSKIP) == 0)
			{
				fflush(outfile);
			//	fflush(mtoutfile);
			}


//			fprintf(mtoutfile, "%g", ii*dt);

//			double max_tip = 0.;
			for(jj = 0; jj < print_size; jj++)
			{
//				fprintf(mtoutfile, " %g", left_filaments[jj].get_tip_pos());
//				if(left_filaments[jj].get_tip_pos() > max_tip)
//					max_tip = left_filaments[jj].get_tip_pos();
			}
//			for(jj = print_size; jj< left_filaments.size(); jj++)
//                                if(left_filaments[jj].get_tip_pos() > max_tip)
//                                        max_tip = left_filaments[jj].get_tip_pos();
//			fprintf(mtoutfile, " %g", max_tip);

//			if(NUMBER_OF_KTS > 1)
//			for(jj = 0; jj < print_size; jj++)
//                                fprintf(mtoutfile, " %g", right_filaments[jj].get_tip_pos());




/*
			if(NUMBER_OF_FILAMENTS_PER_KT < 4)
			{
			for(jj = 0; jj < left_filaments.size(); jj++)//March
				fprintf(mtoutfile, " %i", (int) left_filaments[jj].get_growing());
			for(jj = 0; jj < left_filaments.size(); jj++)
				fprintf(mtoutfile, " %i", (int) left_filaments[jj].get_attached());
			}
*/
//			fprintf(mtoutfile, "\n");
		//	if(!(NEGATIVE_ALLOWED || INFINITE_BOX))
			if(ii*dt > 1.5*TIME_DELAY)
			tally_filament_distrib(ii);
		}
	
		if(ii%(5000*NUMSKIP) == 0)
			write_restart(ii);
	}

	fclose(outfile);
//	fclose(mtoutfile);
	fclose(attachedfile);
	fclose(tensionfile);


	fclose(distribfile);
//print data...

print_distrib_data();

if(TIME > 1.5*TIME_DELAY)
{
//print_distrib_data();
//print_mt_stats();
}

print_mt_stats();

	char mvcomm[128];
	sprintf(mvcomm, "mv /var/tmp/scratch/fvkt/*%4.4i /data1/ebanigan/merotelic/output/", rseed);
	system(mvcomm);

	write_restart(ii);
	fprintf(stderr, "Trial %i completed\n", rseed);
	return 0;
}

