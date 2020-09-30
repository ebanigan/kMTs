#include<iostream>
#include<vector>
#include<math.h>
#include<stdio.h>
//#include "RNG_taus.h"

//#include "common.h"//includes params.h
//#include "protos.h"

using namespace std;
#define DETAIL false

#define RESTART false

#define rseed cl_rseed 
#define NUMBER_OF_KTS cl_numkts
#define NUMBER_OF_FILAMENTS_PER_KT cl_numfil
#define INITIAL_FILAMENT_LENGTH (1000.*MONO_DIAM)//(0.5*(BOX_LENGTH-KT_KT_SPRING_LENGTH-2.*KT_MT_SPRING_LENGTH))//assume 1 mono/dimer/subunit/whatever = 10nm... half spindle = 5micron
#define FIL_START_RAND_LENGTH false//make sure to flip filstart0 concurrently
#define FIL_START_0 false
#define TIME cl_time//50.//500000.
#define dt cl_dt
#define inv_dt (1./dt)
#define NUMSTEPS (TIME/dt)
#define NUMSKIP cl_numskip//10//10000
#define COARSE_DT 50//not really a time step.  actually a number of time steps to skip between coarse grained kinetic steps

#define DISTANT_THRESHOLD 150.//100. 

#define PHOSPHORYLATION_TIME 5000.
#define DELAYED_PHOSPHORYLATION cl_delayed_phosphorylation

#define MEROTELIC cl_merotelic
#define NUMBER_OF_MEROTELIC_FILAMENTS cl_number_of_merotelic_filaments

#define GPOLYM cl_gpolym//0.07//growth polym rate // 7 nm/s
#define SPOLYM 0.//shrinkage polym rate
#define GDEPOLYM 0.// 
#define SDEPOLYM cl_sdepolym// 200 nm/s
#define RESCUE cl_res//2.0e-3// 90/hr
#define CATASTROPHE cl_cat//3.0e-4//catastrophe rate // 8/hr
#define GATT cl_gatt//0.02
#define SATT GATT
#define GDET cl_gdet//1e-3 or 1e-5
#define SDET (cl_sdetfactor*GDET)
#define POLYM_EXP_FACTOR cl_pexp//5.e-2//(3.3e-1*MONO_DIAM/KbT) //rate up to 60nm/s with force of 16pN
#define DEPOLYM_EXP_FACTOR cl_dexp//(-1.e-1)//(-7.5e-3*MONO_DIAM/KbT) //rate down to 20 nm/s with force of 7.5pN
#define RESCUE_EXP_FACTOR cl_rexp//2e-1//(1.4e-5*MONO_DIAM/KbT) //rate up to 600/hr w/ force of 10pN
#define CATASTROPHE_EXP_FACTOR cl_cexp//(-3e-1)//(-3.1e-5*MONO_DIAM/KbT) //rate down to 0.1/hr w/ force of 9.5pN
#define ATT_EXP_FACTOR 0.
#define GDET_EXP_FACTOR cl_gdetexp
#define SDET_EXP_FACTOR cl_sdetexp

#define ATTRANGE cl_attrange

#define POLEWARD_FLUX_VELOCITY cl_poleward_flux_velocity

#define LOAD cl_load
#define TIME_DELAY cl_time_delay//(50e3)

#define KT_MT_SPRING cl_ktmtspring//(20.*KbT/MONO_DIAM/MONO_DIAM)
#define KT_MT_STRONG_SPRING (0.*KbT/MONO_DIAM/MONO_DIAM)//
#define KT_KT_REPEL_SPRING KT_KT_SPRING//(20.*KbT/MONO_DIAM/MONO_DIAM)
#define KT_KT_SPRING cl_ktktspring//(20.*KbT/MONO_DIAM/MONO_DIAM)

#define KT_MT_SPRING_LENGTH (0.*MONO_DIAM)//10
#define KT_MT_STRONG_SPRING_LENGTH (0.*MONO_DIAM)//
#define KT_KT_REPEL_SPRING_LENGTH (100.*MONO_DIAM)//(25.*MONO_DIAM)
#define KT_KT_SPRING_LENGTH (100.*MONO_DIAM)//(100.*MONO_DIAM)

#define unif_rand() RNUM.get_double()//ranf0()
#define MONO_DIAM 1.0
#define KbT 1.0
#define KT_DRAG cl_drag
#define BOX_LENGTH 2000.//(2.*(INITIAL_FILAMENT_LENGTH+KT_MT_SPRING_LENGTH)+KT_KT_SPRING_LENGTH)
#define INFINITE_BOX cl_inf_box
#define NEGATIVE_ALLOWED cl_neg_allowed
#define AUTO_START_CATASTROPHE false//true
#define KT_RAND_MOTION true

