#include "params.h" 
#include "protos.h" 

extern int cl_numkts;
extern int cl_rseed, cl_numfil, cl_numskip, cl_number_of_merotelic_filaments;
extern double cl_load, cl_pexp, cl_dexp, cl_rexp, cl_cexp, cl_time, cl_ktmtspring, cl_ktktspring;
extern double cl_attrange, cl_gdet, cl_gatt, cl_gpolym, cl_sdepolym, cl_cat, cl_res;
extern double cl_sdetfactor, cl_gdetexp, cl_sdetexp;
extern bool cl_delayed_phosphorylation;
extern double cl_poleward_flux_velocity;
extern bool cl_merotelic;

extern int num_attached_left, num_attached_right, num_attached_left_merotelic;
extern double avg_tension, avg_compression, avg_tension2, avg_compression2, avg_tension_m, avg_compression_m;
extern unsigned long long avg_tension_count, avg_compression_count, avg_tension2_count, avg_compression2_count, avg_tension_m_count, avg_compression_m_count;
extern double avg_tip_pos;
extern unsigned long long avg_tip_pos_count;

extern double avg_tension_polym, avg_tension_depolym, avg_compression_polym, avg_compression_depolym;
extern double avg_tension_res, avg_tension_cat, avg_compression_res, avg_compression_cat;
extern unsigned long long avg_tension_polym_count, avg_tension_depolym_count, avg_compression_polym_count, avg_compression_depolym_count;
//extern unsigned long long avg_tension_res_count, avg_tension_cat_count, avg_compression_res_count, avg_compression_cat_count;
//don't need - these are identical to other counts

extern double cl_dt;
extern bool cl_inf_box, cl_neg_allowed;
extern double cl_time_delay;
extern double cl_drag;

extern vector<double> mtdistrib;
extern vector<double> mtdistrib_all;
extern vector<double> mtdistrib_det;
extern unsigned long long mtdistrib_count;
extern unsigned long long mtdistrib_count_all;
extern unsigned long long mtdistrib_count_det;


extern vector<double> kt_position;
extern int iseed0;
extern double sqrt_dt_over_zeta;

extern FILE *outfile;
extern char outname[72];
extern FILE *mtoutfile;
extern char mtoutname[72];
extern FILE *attachedfile;
extern char attachedname[72];
extern FILE *tensionfile;
extern char tensionname[72];
extern FILE *distribfile;
extern char distribname[72];

extern unsigned long long total_time_steps;
extern unsigned long long mtstats_steps;

extern double max_force;

extern bool min_dt;

