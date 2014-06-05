#ifndef _PEBBLER_HYPER_NODE_H
#define _PEBBLER_HYPER_NODE_H 1

#include <vector>
#include <iostream>

#include "PebblerHyperEdge.h"

template<class T, class A>
class PebblerHyperNode
{
  public:
    T data; // Original Hypergraph representation
    int id; // index of original hypergraph node
    std::vector<PebblerHyperEdge<A> > edges;
    bool pebbled;

    PebblerHyperNode() {}
    ~PebblerHyperNode() {}
    PebblerHyperNode(const T& d, int i)
    {
        data = d;
        id = i;
        pebbled = false;
    }
    
    void ClearPebbles();
    void AddEdge(const PebblerHyperEdge<A>& edge) { edges.push_back(edge); }
    void AddEdge(const std::vector<int> src, int target, T annotation)
    {
        AddEdge(PebblerHyperEdge<A>(src, target, annotation));
    }
};

template<class T, class A>
void PebblerHyperNode<T, A>::ClearPebbles()
{
    pebbled = false;
    
    for (int e = 0; e < edges.size(); e++)
    {
        edges[e].ClearPebbles();
    }
}  

template<class T, class A>
std::ostream& operator<< (std::ostream& os, PebblerHyperNode<T, A>& node)
{
    os << node.data.toString() << + "\t\t\t\t= { ";

    os << node.id + " = { ";
    for (int e = 0; e < node.edges.size(); e++)
    {
        os << node.edges[e];
        if (e+1 < node.edges.size()) os << ", ";
    }
    os << " }" << std::endl;
    
    return os;
}
#endif