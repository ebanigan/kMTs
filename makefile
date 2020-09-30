#tell make to create runme
all: runme

#compiler
CPP = icpc
#CPP = g++

#optimization
OPTFLAGS = -O3 -xhost -ipo
#OPTFLAGS = -g
#OPTFLAGS = -O3

#list of types of OBJS which have different dependencies or are dependencies for other OBJS
ALLOBJS = \
	main.o \
	change_dt.o \
	check_finite.o \
	evolve_mts.o \
	forces_kts.o \
	initialize.o \
	phosphorylate.o \
	print_mt_stats.o \
	options_menu.o \
	print_distrib_data.o \
	print_distrib_vs_time.o \
	restart.o \
	tally_filament_distrib.o \
	update_linked_lists.o

SUBOBJS = \
	change_dt.o \
	check_finite.o \
	evolve_mts.o \
	forces_kts.o \
	initialize.o \
	options_menu.o \
	phosphorylate.o \
	print_mt_stats.o \
	print_distrib_data.o \
	print_distrib_vs_time.o \
	restart.o \
	tally_filament_distrib.o \
	update_linked_lists.o

FOBJS = \
	main.o \
	change_dt.o \
	evolve_mts.o \
	forces_kts.o \
	initialize.o \
	phosphorylate.o \
	print_mt_stats.o \
	print_distrib_vs_time.o \
	restart.o \
	tally_filament_distrib.o \
	update_linked_lists.o

LOBJS = \
	main.o \
	change_dt.o \
	evolve_mts.o \
	forces_kts.o \
	initialize.o \
	update_linked_lists.o \
	restart.o

#list of dependencies of objects and main, which depends on all objects
$(ALLOBJS): \
	common.h params.h protos.h RNG_taus.h 

$(FOBJS): \
	filament.h filament_methods.h 

$(LOBJS): \
	linked_list.h

main.o: \
	$(SUBOBJS) 

restart.o: \
	initialize.o


#compile each .cpp file
.cpp.o:
	$(CPP) $(OPTFLAGS) -c -o $@ $<
#    $@  means "the target"
#    $<  means "whatever the dependencies are"

#create executable
#EXECUTABLE = runme
#$(EXECUTABLE): $(ALLOBJS)
runme: $(ALLOBJS)
	$(CPP) $(OPTFLAGS) -o $@ $(ALLOBJS)

