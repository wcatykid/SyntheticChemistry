#ifndef _LINKER_GUARD
#define _LINKER_GUARD 1

#include <vector>

#include <openbabel/mol.h>

#include "EdgeAggregator.h"
#include "Molecule.h"

class Linker : public Molecule
{
  public:
    Linker(OpenBabel::OBMol& obmol) : Molecule(obmol, LINKER) {}
    Linker() {}
    ~Linker() {}

    std::vector<EdgeAggregator> Compose(const Molecule&);
    bool Equals(const Molecule&) const;

    void parseAppendix(const std::string&);

  private:
};

#endif