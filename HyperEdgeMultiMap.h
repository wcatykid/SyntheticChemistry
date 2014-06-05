#ifndef _HYPER_EDGE_MULTI_MAP_GUARD
#define _HYPER_EDGE_MULTI_MAP_GUARD 1

#include <vector>
#include <algorithm>
#include <string>

#include "Molecule.h"

#include "HyperGraph.h"
#include "PebblerHyperEdge.h"
#include "EdgeAnnotation.h"
#include "Utilities.h"

template<class A>
class HyperEdgeMultiMap
{
  public:
    int getSize() { return size; }
    HyperEdgeMultiMap();
    HyperEdgeMultiMap(int sz);
    ~HyperEdgeMultiMap();
    void Put(const PebblerHyperEdge<A>& edge);
    std::vector<PebblerHyperEdge<A> > GetBasedOnGoal(int goalNodeIndex);
    std::string toString() const;

  private:
    const int TABLE_SIZE;
    std::vector<PebblerHyperEdge<A> >* table;
    int size; // Number of total hyper-edges
};

// If the user specifies the size, we will never have to rehash
template<class A>
HyperEdgeMultiMap<A>::HyperEdgeMultiMap() : TABLE_SIZE(1000)
{
    size = 0;            
    table = new std::vector<PebblerHyperEdge<A> >[TABLE_SIZE];
}

// If the user specifies the size, we will never have to rehash
template<class A>
HyperEdgeMultiMap<A>::HyperEdgeMultiMap(int sz) : TABLE_SIZE(sz) 
{
    size = 0;
    table = new std::vector<PebblerHyperEdge<A> >[TABLE_SIZE];
}

template<class A>
HyperEdgeMultiMap<A>::~HyperEdgeMultiMap()
{
    delete table;
}

//
// Add the PebblerHyperEdge to all source node hash values
//
template<class A>
void HyperEdgeMultiMap<A>::Put(const PebblerHyperEdge<A>& edge)
{
    //
    // Analyze the edge to determine if it is a mixed edge; all edges are
    // such that the target is greater than or less than all source nodes
    // Find the minimum non-intrinsic node (if it exists)
    //
    std::sort(edge.sourceNodes.begin(), edge.sourceNodes.end());

    if (edge.sourceNodes.size() >= 2)
    {
        if (edge.sourceNodes[0] < edge.targetNode && edge.targetNode < edge.sourceNodes[edge.sourceNodes.size()-1])
        {
            throw MakeString("A mixed edge was pebbled as valid: ", edge.toString());
        }       
    }

    long hashVal = (edge.targetNode % TABLE_SIZE);

    if (Contains(table[hashVal], edge)) table[hashVal].push_back(edge);

    size++;
}

// Acquire pertinent problems based on target nodes
template<class A>
std::vector<PebblerHyperEdge<A> > HyperEdgeMultiMap<A>::GetBasedOnGoal(int goalNodeIndex)
{
    if (goalNodeIndex < 0 || goalNodeIndex >= TABLE_SIZE)
    {
        throw MakeString("HyperEdgeMultimap::Get::key", goalNodeIndex);
    }

    return table[goalNodeIndex];
}

template<class A>
std::string HyperEdgeMultiMap<A>::toString() const
{
    std::string retS = "";

    for (int ell = 0; ell < TABLE_SIZE; ell++)
    {
        if (!table[ell].empty())
        {
            retS += ell + ":\n";
            for (int e = 0; e < table[ell].size(); e++)
            {
                retS += table[ell][e].toString() + "\n";
            }
        }
    }

    return retS;
}

#endif