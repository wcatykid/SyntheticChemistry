#ifndef _EDGE_AGGREGATOR_GUARD
#define _EDGE_AGGREGATOR_GUARD 1

#include <vector>

#include "Molecule.h"
#include "EdgeAnnotation.h"

//
// An aggregation class of information to pass back from instantiation.
//
class EdgeAggregator
{
  public:
    std::vector<Molecule> antecedent;
    Molecule consequent;
    EdgeAnnotationT annotation;
        
    EdgeAggregator(const std::vector<Molecule>& antes, const Molecule& c, const EdgeAnnotationT& ann)
    {
        for (int a = 0; a < antes.size(); a++)
        {
            antecedent.push_back(antes[a]);
        }

        consequent = c;
        annotation = ann;
    }
};

#endif