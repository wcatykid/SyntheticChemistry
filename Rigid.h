#ifndef _RIGID_GUARD
#define _RIGID_GUARD 1

#include <vector>

#include <openbabel/mol.h>

#include "EdgeAggregator.h"
#include "Molecule.h"

class Rigid : public Molecule
{
  public:
    Rigid(OpenBabel::OBMol& obmol) : Molecule(obmol, RIGID) {}
    ~Rigid() {}

    std::vector<EdgeAggregator> Compose(const Molecule&);
    bool Equals(const Molecule&) const;
    
    void parseAppendix(const std::string&);
    
    //
    // Parsing is completed by OpenBabel
    // ;
    //
    // See Further Examples for details: http://openbabel.org/docs/2.3.1/UseTheLibrary/CppExamples.html#quickstart-example
    //
    //

  private:
    OpenBabel::OBMol obmol;
    //Connections info?
};

#endif