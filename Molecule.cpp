#include <cstring>
#include <vector>

#include "Molecule.h"
#include "Bond.h"
#include "Atom.h"

#include "EdgeAggregator.h"


std::vector<EdgeAggregator> Molecule::Compose(const Molecule& thatMolecule)
{
    std::vector<EdgeAggregator> newMolecules;

    // To be completed
    
    return newMolecules;
}

// *****************************************************************************

int Molecule::getAtomIndex(int id) const
{
    for(int x = 0; x < this->atoms.size(); x++)
    {
        if(this->atoms[x].getAtomID() == id)
        {
				return x;
        }
    }
    
    return -1;
}

// *****************************************************************************

Atom Molecule::getAtom(int id)
{
    int index = getAtomIndex(id);
    
    if (index == -1) throw "Bond id not found";
    
    return atoms[index];
}

// *****************************************************************************

bool Molecule::addBond(int xID, int yID, eTypeOfBondT bt, eStatusBitT s)
{
    int xIndex = getAtomIndex(xID);
    int yIndex = getAtomIndex(yID);

    if (xIndex == -1 || yIndex == -1) return false;

    if(!atoms[xIndex].addBond(atoms[yIndex])) return false;

    this->bonds.push_back(Bond(this->bonds.size(), xID, yID, bt, s));

    // TODO: add bond to this->obmol
    
    return true;
}

// *****************************************************************************

int Molecule::getBondIndex(int id) const
{
    for(int x = 0; x < this->bonds.size(); x++)
    {
        if(this->bonds[x].getBondID() == id)
        {
				return x;
        }
    }
    
    return -1;
}

// *****************************************************************************

int Molecule::getBondIndex(int xID, int yID) const
{
    for(int x = 0; x < this->bonds.size(); x++)
    {
        if(this->bonds[x].getOriginAtomID() == xID && this->bonds[x].getTargetAtomID() == yID)
        {
            return x;
        }
         
        if(this->bonds[x].getOriginAtomID() == yID && this->bonds[x].getTargetAtomID() == xID)
        {
            return x;
        }
    }

    return -1;
}

// *****************************************************************************

Bond Molecule::getBond(int id)
{
    int index = getBondIndex(id);
    
    if (index == -1) throw "Bond id not found";
    
    return bonds[index];
}

// *****************************************************************************

Bond Molecule::getBond(int xID, int yID)
{
    int index = getBondIndex(xID, yID);
    
    if (index == -1) throw "Bond id not found";
    
    return bonds[index];
}

// *****************************************************************************

void Molecule::addAtom(const Atom& a)
{
    this->atoms.push_back(a);

    // TODO: Add atom to this->obmol
}

// *****************************************************************************

bool Molecule::Equals(const Molecule&) const
{
    return false;
}

// *****************************************************************************

std::string Molecule::toString() const
{
    return "TBD";
}

// *****************************************************************************

std::ostream& operator<< (std::ostream& os, Molecule& mol)
{
    os << mol.toString();
    
    return os;
}