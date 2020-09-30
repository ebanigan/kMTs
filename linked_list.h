#include <vector>
#include "common.h"
using namespace std;

class linked_list
{
protected:
	int last_fil;
	int first_fil;
        vector<int> prev_fil;
        vector<int> next_fil;


public:

	linked_list()
	{
	   next_fil.reserve(NUMBER_OF_FILAMENTS_PER_KT);
           prev_fil.reserve(NUMBER_OF_FILAMENTS_PER_KT);
	   last_fil = -1;
	   first_fil = -1;
	}
	linked_list(int size)
	{
           next_fil.reserve(NUMBER_OF_FILAMENTS_PER_KT);
           prev_fil.reserve(NUMBER_OF_FILAMENTS_PER_KT);

		initialize_lists(size);
	}	

	void initialize_lists(int size)
	{
           for(int ii = 0; ii < size; ii++)
           {
                next_fil.push_back(-1);
                prev_fil.push_back(-1);
           }
	}

	void add_fil(int new_id)
	{
		if(last_fil != -1)
		  next_fil[last_fil] = new_id;
                else//(last_fil == -1)
                  first_fil = new_id;

		prev_fil[new_id] = last_fil;
		last_fil = new_id;
	}

	void delete_fil(int id_to_delete)
	{
		int pf = prev_fil[id_to_delete];
		int nf = next_fil[id_to_delete];

		//fprintf(stderr, "%i %i %i\n", id_to_delete, pf, nf);

		if(pf != -1)
		 next_fil[pf] = nf;
		else
		 first_fil = nf;
		if(nf != -1)
		  prev_fil[nf] = pf;
		else
		  last_fil = pf; 
	
		next_fil[id_to_delete] = -1;
		prev_fil[id_to_delete] = -1;
	}

	int get_next_fil(int place) const {return next_fil[place];}
	int get_prev_fil(int place) const {return prev_fil[place];}
	int get_first_fil() const {return first_fil;}
	int get_last_fil() const {return last_fil;}

};







extern linked_list left_near_list;//list of filaments near the kinetochore
extern linked_list left_far_list; 

extern linked_list right_near_list;
extern linked_list right_far_list;

extern linked_list left_merotelic_near_list;
extern linked_list left_merotelic_far_list;
