#ifndef _INSTANTIATOR_GUARD
#define _INSTANTIATOR_GUARD 1

#include <vector>
#include <queue>
#include <iostream>

#include "Molecule.h"
#include "Rigid.h"
#include "Linker.h"
#include "HyperGraph.h"
#include "EdgeAnnotation.h"

class Instantiator
{
  private:
    // Contains all processed clauses and relationships amongst the clauses
    HyperGraph<Molecule, EdgeAnnotationT> graph;

    // debug stream
    std::ostream& ds;

    void HandleNewMolecules(std::queue<Molecule>& worklist, std::vector<EdgeAggregator>& newEdges);
    void AddEdge(const std::vector<Molecule>& antecedent, const Molecule& consequent, const EdgeAnnotationT& annotation);

  public:
    Instantiator(std::ostream& out = std::cout) : ds(out) {}

    // Main instantiation function for all linkers and rigidss; worklist technique to construct the graph
    HyperGraph<Molecule, EdgeAnnotationT> Instantiate(std::vector<Linker>& linkers, std::vector<Rigid>& rigids);
};

#endif