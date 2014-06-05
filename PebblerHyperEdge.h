#ifndef _PEBBLER_HYPEREDGE_GUARD
#define _PEBBLER_HYPEREDGE_GUARD 1

#include <vector>
#include <string>
#include <algorithm>

#include "EdgeAnnotation.h"

template<class A>
class PebblerHyperEdge
{
  public:
    std::vector<int> sourceNodes;
    // parallel array to the sourceNodes; pebbles are boolean marks
    std::vector<bool> pebbles;
    int targetNode;
    A annotation;

    // Once completely pebbled, we mark as pebbled for memoizing purposes.
    bool fullyPebbled;
  
    PebblerHyperEdge(const std::vector<int>& src, int target, const A& annot);
    ~PebblerHyperEdge() {}

    bool IsFullyPebbled() const;
    bool Equals(const PebblerHyperEdge<A>& thatEdge) const;
    std::string toString() const;
    void ClearPebbles();
    int SourceIndex(int srcNode) const;
    
    template<class SA>
    friend std::ostream& operator<< (std::ostream& os, PebblerHyperEdge<SA>& edge);
};

template<class A>
PebblerHyperEdge<A>::PebblerHyperEdge(const std::vector<int>& src, int target, const A& annot)
{
   sourceNodes = src;
   targetNode = target;
   annotation = annot; 
   // Ensure parallel array is large enough for marking.
   pebbles.reserve(src.size());
   fullyPebbled = false;
}

template<class A>
int PebblerHyperEdge<A>::SourceIndex(int srcNode) const
{
    for (int i = 0; i < sourceNodes.size(); i++)
    {
        if (sourceNodes[i] == srcNode) return i;
    }
    return -1;
}

template<class A>
void PebblerHyperEdge<A>::ClearPebbles()
{
    // memoized
    fullyPebbled = false;
    
    pebbles.clear();
    pebbles.reserve(sourceNodes.size());
}

template<class A>
bool PebblerHyperEdge<A>::IsFullyPebbled() const
{
    // memoized
    if (fullyPebbled) return true;

    // calculate
    if (std::find(pebbles.begin(), pebbles.end(), false) == pebbles.end()) return false;

    // Mark
    fullyPebbled = true;

    return true;
}

// Ahe source nodes and target must be the same for equality.
template<class A>
bool PebblerHyperEdge<A>::Equals(const PebblerHyperEdge<A>& thatEdge) const
{
    if (targetNode != thatEdge.targetNode) return false;

    if (this->sourceNodes.size() != thatEdge.sourcePebbles.size()) return false;

    for (int s = 0; s < thatEdge.sourceNodes.size(); s++)
    {
        if (std::find(this->sourceNodes.begin(), this->sourceNodes.end(), thatEdge.sourceNodes[s]) == sourceNodes.end()) return false;
    }
    
    return true;
}

template<class A>
std::string PebblerHyperEdge<A>::toString() const
{
    std::string retS = "";

    retS += " { ";

    for (int n = 0; n < sourceNodes.size(); n++)
    {
        retS += sourceNodes[n];
        if (n+1 < sourceNodes.size()) retS += ", ";
    }
    retS += " } -> ";
    retS += targetNode;

    return retS;
}

template<class SA>
std::ostream& operator<< (std::ostream& os, PebblerHyperEdge<SA>& edge)
{
    os << edge.toString();

    return os;
}

#endif