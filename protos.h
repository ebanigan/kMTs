extern void initialize();
extern void initialize_comm_line();
extern void construct_linked_lists();
extern void set_common_variables();
extern void options_menu();


extern void check_finite(unsigned long long step);

extern void change_dt(unsigned long long *step);

extern void evolve_mts(unsigned long long step);
extern void forces_kts(unsigned long long step);
extern void kt_rand_motion();
extern void check_kt_bc();
extern void update_linked_lists();

extern void phosphorylate();

extern void tally_filament_distrib(unsigned long long step);

extern void write_restart(unsigned long long step);
extern void read_restart();

//extern double ranf0();

extern void print_distrib_data();
extern void print_mt_stats();
extern void print_distrib_vs_time(unsigned long long sim_step);
