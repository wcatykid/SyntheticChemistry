#ifndef _HYPEREDGE_GUARD
#define _HYPEREDGE_GUARD 1

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include "EdgeAnnotation.h"

template<class A>
class HyperEdge
{
  public:
  
    int targetNode;
    std::vector<int> sourceNodes;
    A annotation;
    
    HyperEdge(const std::vector<int>& src, int target, const A& annot);
    ~HyperEdge() {}

    bool DefinesEdge(const std::vector<int>& thatAnte, int thatConseq) const;
    bool Equals(const HyperEdge& thatEdge) const;

    // For simple output of the graph (ala toString())  
    template<class AS>  
    friend std::ostream& operator<< (std::ostream& os, HyperEdge<AS>& edge);
};

template<class A>
HyperEdge<A>::HyperEdge(const std::vector<int>& src, int target, const A& annot)
{
   sourceNodes = src;
   targetNode = target;
   annotation = annot;
}

/*************************************************************************/

template<class A>
bool HyperEdge<A>::DefinesEdge(const std::vector<int>& thatAnte, int thatConseq) const
{
    if (this->targetNode != thatConseq) return false;

    if (this->sourceNodes.size() != thatAnte.size()) return false;
	
    // Only need to check containment in one direction.
    for (int a = 0; a < thatAnte.size(); a++)
    {
        if (std::find(this->sourceNodes.begin(), this->sourceNodes.end(), thatAnte[a]) == this->sourceNodes.end()) return false;
    }

	return true;
}

/*************************************************************************/

template<class A>
bool HyperEdge<A>::Equals(const HyperEdge& thatEdge) const
{
    return this->DefinesEdge(thatEdge.sourceNodes, thatEdge.targetNode);
}

/*************************************************************************/

template<class A>
std::ostream& operator<< (std::ostream& os, HyperEdge<A>& edge)
{
    os << " { ";

    for (int n = 0; n < edge.sourceNodes.size(); n++)
    {
        os << edge.sourceNodes[n];
        if (n+1 < edge.sourceNodes.size()) os << ", ";
    }
    os << " } -> " << edge.targetNode;

    return os;
}
#endif