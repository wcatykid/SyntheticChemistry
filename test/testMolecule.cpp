#include <vector>
#include <iostream>
#include <cstdio>
#include <string>

#include "Molecule.h"
#include "Atom.h"
#include "Bond.h"
#include "Linker.h"
#include "Rigid.h"

void displayMolecules(Molecule *a)
{
	std::cout << "ID = " << a->getmoleculeID() << std::endl;
	if(a->getisLinker())
		std::cout << "is a linker" << std::endl;
	else if(a->getisRigid())
		std::cout << "is a rigid" << std::endl;
	else
		std::cout << "is neither a linker nor a rigid" << std::endl;
	std::cout << "There are " << a->getNumberOfAtoms() << " atoms." << std::endl;
	std::cout << "There are " << a->getNumberOfBonds() << " bonds." << std::endl;
	
}

int main()
{
	Molecule *m;
	Linker *l;
	Rigid *r;
	Atom *a1, *a2;
	Bond *b;

	l = new Linker(1);
	r = new Rigid(2);
	m = new Molecule(3, false, false);

	displayMolecules(l);
	std::cout << std::endl << std::endl << std::endl;
	displayMolecules(r);
	std::cout << std::endl << std::endl << std::endl;
	displayMolecules(m);
	std::cout << std::endl << std::endl << std::endl;

	/* add atoms */	
	a1 = new Atom(1, "C.1", 50, "C.1");
	a2 = new Atom(2, "C.1", 50, "C.1");
	a1->pushConnectionType("C.1");
	a2->pushConnectionType("C.1");

	m->addAtomToMolecule(*a1);	
	m->addAtomToMolecule(*a2);	

	l->addAtomToMolecule(*a1);	
	l->addAtomToMolecule(*a2);	

	r->addAtomToMolecule(*a1);	
	r->addAtomToMolecule(*a2);	

	/* add bond */
	if(m->addBond(1, 2, ar, GROUP))
		std::cout << "bonds successfully added to molecule" << std::endl;
	else
		std::cout << "failed to add bond to molecule" << std::endl;

	if(l->addBond(1, 2, ar, GROUP))
		std::cout << "bonds successfully added to linker" << std::endl;
	else
		std::cout << "failed to add bond to linker" << std::endl;

	if(r->addBond(1, 2, ar, GROUP))
		std::cout << "bonds successfully added to rigid" << std::endl;
	else
		std::cout << "failed to add bond to rigid" << std::endl;


	/* display molecules again */
	displayMolecules(l);
	std::cout << std::endl << std::endl << std::endl;
	displayMolecules(r);
	std::cout << std::endl << std::endl << std::endl;
	displayMolecules(m);
	std::cout << std::endl << std::endl << std::endl;

	std::cout << "done" << std::endl;

    return 0;
}
