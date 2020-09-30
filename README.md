# kMTs
## Description
Stochastic simulation of kinetochore microtubules as in Ref [1]

## Detailed description
**Basic components:**
1) a list of filament objects (see filament.h) which grow, shrink, etc. at different rates and have tip positions. There is a vector for left_filaments, right_filaments, and left_merotelic_filaments (which extend from the left to attach to the right kinetochore). The single kinetochore case only has left_filaments

2) a list of doubles for kinetochore position, kt_position

3) a bunch of parameters related to these objects and methods to do calculations

4) a few linked lists, which aren't particularly important, but are helpful to ignore (most of the time) filaments that are very far from the kinetochore

5) params.h has a list of parameters/variables, many of which are set via command line options

6) protos.h has a list of subroutines

7) To compile, there is a makefile. Depending on the system, you may need to fiddle with the makefile, the include lines in .cpp files, and the .h files to get it to work.

8) I included a submission script called merotelic_01.sh that I used on a cluster with a queueing system.  It has a list of variables that are the "standard" set used for the paper (except nummero was 0 for the paper).  **pex, dex**, etc. are force sensitivity exponents for MT rates. **pflux** is the poleward flux velocity. **detfactor** is the factor the the detachment rate is multiplied when filaments switch to shrinking state from growing state. **gdetexp (sdetexp)** is exponential force sensitivity of detachment rate in growing (shrinking) state.  **attrange** is the max distance from the kinetochore that a MT will try to attach to a kinetochore (we used 20, though 10 might be more realistic.  **inf** makes the box infinite in the positive (rightward) direction, **neg** makes the box infinite in the negative direction. **skip** is for NUMSKIP, the variable used for increments between printing data & some other stuff, dt is set by **tstep**, but note that it can change automatically (in a very unsophisticated way) via change_dt.cpp, **mero** is a boolean for whether or not we have merotelic attachments. note that **seed** serves as both the random seed and the trial number (appended to all data files). To get different initial conditions vary seed (option -s) (In submission script, seed is determined by $SGE_TASK_ID).

**Basic methodology:**
1. Take input parameters from command line (MT rates, kinetochore drag, 1 vs. 2 kinetochores, spring constants, finite vs. infinite-sized system, time step, num MTs, MT force sensitivity, simulation time, applied force, num merotelic attachments, poleward flux velocity, a few other misc variables) & set up system (assign variables, generate MT list objects, initialize positions, construct linked lists, etc.). If it's a simulation restarted from a prior realization, the input says so, and the code reads from a restart file. 

2. Enter loop:

i) reset saved variables, averages, etc. if necessary, print data, etc.
ii) Exert forces on kinetochores (due to MTs, other kinetochores, and external load [in the case of 1 kineto]) to move them via forces_kts
iii) Exert forces on MTs, calculate their rates, grow/shrink/switch/attach/detach via evolve_mts (which can change tip positions), change tip positions due to POLEWARD_FLUX_VELOCITY
iv) Update linked lists if necessary
v) Print data if desired, relocate temp data files to permanent directories if necessary

Basically, the most important part of the code resides in forces_kts.cpp, evolve_mts.cpp, filament.h, and filament_methods.h. Almost everything else is just auxiliary stuff related to printing data, initialization, and calculating misc stuff.

**Misc notes:**
evolve_mts calculates the rate constants based on MT tip positions and current time step position of kinetochore through filament-specific methods  (called in evolve_mts.cpp and can be found in filament_methods.h, they have names like try_polymerize, which calls calc_polym(), which calculates rate based on force and whether or not MT is attached to a kinetochore.  try_polymerize then conditionally alters tip position).  

Filaments have a polarization to tell you if the tip grows toward the left or right side (+1 growing to right side from left side)

There are a few commands scattered throughout main.cpp and initialize.cpp that move files from a scratch directory to my home directory. You can find and change them by searching for sprintf lines, usually with some combination of ebanigan, scratch, or tmp in the line.

To run code, you should have directories restart/ and whatever output and temp files you designate in place of what I had in the sprintf lines above.

**Below is a description of the output files in format of name, col1, col2, etc.**
printed from main:
**ktpos**, sim time in sim units, left kinetochore position, right kineto position (if applicable)

**attached**, sim time, num MTs attached to left kineto (from left_filaments list), num MTs attached to right (from right_filaments list if applicable), num merotelic filaments attached (if applicable)

printed from print_distrib_vs_time:
**distrib_stats**, sim time, average tip position of growing MT on left, avg tip pos of shrinking MT on left, variance for left growing pos, variance for left shrinking, number MTs attached and growing on left, num MTs attached and shrinking on left, next 6 columns are same data for right MTs if applicable

**tension**, sim time, avg force (per MT) on (left) MTs under tension, avg force per MT on MTs compressed, avg num MTs under tension, avg num MTs compressed, next 4 cols are for right_filaments if applicable

from print_mt_stats:
**mtstats**
first line of file: avg polym rate of MTs (in growing state) instantaneously under tension, average depolym rate of MTs under tension, avg rescue rate of MTs under tension, avg catastrophe rate of MTs under tension, next 4 columns are rates (in the same order) but for MTs instantaneously compressed, avg num MTs growing under tension, avg num MTs shrinking while under tension, avg num MTs growing subject to compression, avg num MTs shrinking and compressed
next 2N lines for all left_filaments followed by all right_filaments: avg num switches (rescue or catastrophe) per unit time, percentage of time spent in growing state, percentage of time attached to kineto, percentage of time under tension, percentage of time compressed, % of time under "high" tension (threshold chosen to be =KT_MT_SPRING), % time under high compression, rate of changes in sign of force on MT

(note data for the above are calculated in evolve_mts or methods called from evolve_mts and forces_kts)

from print_distrib_stats:
**mtdistrib**, position of bin relative to left kinetochore (bin width = 0.5 sim unit), avg num (left) attached MTs in that bin

**mtdistrib_all**, position of bin relative to left kinetochore, avg num left MTs in that bin

**mtdistrib_det**, position of bin relative to left kinetochore, avg num left detached MTs in that bin 

(note the 3 above files are printed at end of a run and their data is accumulated via tally_filament_distrib)

not printed in version I sent (printed from main, but all lines are commented out):
**mtpos**, sim time, position of n MT tips with n set by print_size variable

**Known possible issues**
1. There is a potential conflict/confusion in distrib_stats between the globally defined distribfile and the distribfile defined in mtdistrib - right now the code should function properly, but if the subroutines were shuffled there could be a problem.


## References
[1] EJ Banigan, KK Chiou, ER Ballister, MA Lampson, AJ Liu. Proc. Natl. Acad. Sci. 112:12699 (2015).
