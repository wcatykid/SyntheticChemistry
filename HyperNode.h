#ifndef _HYPER_NODE_H
#define _HYPER_NODE_H 1

#include <vector>
#include <iostream>

#include "HyperEdge.h"

template<class T, class A>
class HyperNode
{
  public:
    T data;
    int id;
    std::vector<HyperEdge<A> > edges;

    HyperNode(const T& d, int i)
    {
        data = d;
        id = i;
    }

    void AddEdge(const HyperEdge<A>& edge) { edges.push_back(edge); }
};

template<class T, class A>
std::ostream& operator<< (std::ostream& os, HyperNode<T, A>& node)
{
    os << node.data.toString() << + "\t\t\t\t= { ";

    os << node.id + "SuccE = { ";
    for (int e = 0; e < node.edges.size(); e++)
    {
        os << node.edges[e];
        if (e+1 < node.edges.size()) os << ", ";
    }
    os << " }" << std::endl;

    return os;
}
#endif