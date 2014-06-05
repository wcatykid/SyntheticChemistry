#include <cstring>
#include <vector>

#include "Rigid.h"
#include "EdgeAggregator.h"



std::vector<EdgeAggregator> Rigid::Compose(const Molecule& thatMolecule)
{
    std::vector<EdgeAggregator> newMolecules;
    
    // To be completed.

    return newMolecules;
}


void Rigid::parseAppendix(const std::string& appendix)
{
	/* get past first 3 lines */
	/* get number of atoms */
	/* get number of bonds */
	/* get past rest of line */
	/* for each atoms line */
		/* skip the first line */
		/* read next 2 integers ??? */
		/* insert atom into molecule */
	/* for each bonds line */
		/* read origin atom id */
		/* read target atom id */
		/* insert new bond into molecule */

	/* read until ">" */
	/* for each atom */
		/* read atom type */
		/* update atom */
	/* read until "connect>" */
	/* for each line until "$$$$" */
		/* read atom id */
		/* while not end of line */
			/* read eligible atom type to connect */
			/* push eligible atom type to connect onto atom with id */
}


bool Rigid::Equals(const Molecule&) const
{

    return false;
}

std::ostream& operator<< (std::ostream& os, Rigid& rigid)
{

    return os;
}