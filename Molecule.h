#ifndef _MOLECULE_GUARD
#define _MOLECULE_GUARD 1

#include <string>
#include <vector>

#include <openbabel/mol.h>

#include "Bond.h"
#include "Atom.h"

class EdgeAggregator;

typedef enum
{
    RIGID,
    LINKER,
    COMPLEX
} MoleculeT;

class Molecule
{
  public:
    // Some are for compilation right now, default constructor should be eliminated.
    Molecule() {}
    Molecule(int id, MoleculeT t) : moleculeID(id), type(t) {}
    Molecule(OpenBabel::OBMol& mol, MoleculeT t) : obmol(mol), type(t) {}
    Molecule(OpenBabel::OBMol& mol, int id) : obmol(mol), moleculeID(id) {}

    ~Molecule() {}

    void setGraphID(int id) { graphID = id; }
    int getGraphID() { return graphID; }
    virtual std::vector<EdgeAggregator> Compose(const Molecule&);

    //
    // Get Functions
    //
    int getmoleculeID()const { return this->moleculeID; }
    bool IsLinker() const { return type == LINKER; }
    bool IsComplex() const { return type == COMPLEX; }
    bool IsRigid() const { return type == RIGID; }
    
    int getNumberOfAtoms()const { return this->atoms.size(); }
    int getNumberOfBonds() const { return this->bonds.size(); }
    int getNumberOfRigids() const { return this->rigids.size(); }
    int getNumberOfLinkers() const { return this->linkers.size(); }

    //
    // Set Functions
    //
    void setmoleculeID(int x) { this->moleculeID = x; }
    void makeLinker() { type = LINKER; }
    void makeRigid() { type = RIGID; }
    void makeComplex() { type = COMPLEX; }
    
    bool addBond(int xID, int yID, eTypeOfBondT bt, eStatusBitT s);
    
    Atom getAtom(int id);
    Bond getBond(int id);
    Bond getBond(int xID, int yID);
    void addAtom(const Atom& a);

    virtual bool Equals(const Molecule&) const;
    std::string toString() const;
    friend std::ostream& operator<< (std::ostream& os, Molecule& mol);

  private:
    int graphID;

  protected:
    int moleculeID;
    MoleculeT type;
	 OpenBabel::OBMol obmol;
    std::vector<Atom> atoms;
    std::vector<Bond> bonds;
   
    std::vector<Molecule> rigids;
    std::vector<Molecule> linkers;

    int getAtomIndex(int id) const;
    int getBondIndex(int id) const;
    int getBondIndex(int xID, int yID) const;  
};

#endif