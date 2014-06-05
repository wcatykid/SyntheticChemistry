#ifndef _PEBBLER_HYPER_GRAPH_GUARD
#define _PEBBLER_HYPER_GRAPH_GUARD 1

#include <vector>
#include <deque>
#include <exception>
#include <utility>
#include <sstream>
#include <string>

#include "PebblerHyperNode.h"
#include "PebblerHyperEdge.h"
#include "HyperEdgeMultiMap.h"
#include "HyperNode.h"
#include "HyperEdge.h"
#include "Utilities.h"

//
// Integer-based representation of the main hypergraph
//
template<class T, class A>
class PebblerHyperGraph
{
  private:
    // The main graph data structure
    std::vector<PebblerHyperNode<T, A> > vertices;
    HyperEdgeMultiMap<A> forwardEdges;
    HyperEdgeMultiMap<A> backEdges;

    bool IsNodePebbled(int index) const { return vertices[index].pebbled; }    
    void PebbleBackward(int numLinkerRigids);
    void ForwardTraversal(HyperEdgeMultiMap<A>& edgeDatabase, const std::vector<int>& nodesToPebble);
    void Pebble(const std::vector<int>& linkers, const std::vector<int>& rigids);

  public:
    PebblerHyperGraph() { }
    PebblerHyperGraph(const std::vector<PebblerHyperNode<T, A> >& vs);
    ~PebblerHyperGraph() { }
    int size() const { return vertices.size(); }
    
    template<class TS, class AS>
    friend std::ostream& operator<< (std::ostream& os, PebblerHyperGraph<TS, AS>& graph);

    void ClearPebbles();
};

template<class T, class A>
PebblerHyperGraph<T,A>::PebblerHyperGraph(const std::vector<PebblerHyperNode<T, A> >& vs)
{
    for (int v = 0; v < vs.size(); v++)
    {
        vertices.push_back(vs[v]);
    }
}

//
// Clear all pebbles from all nodes and edges in the hypergraph
//
template <class T, class A>
void PebblerHyperGraph<T, A>::ClearPebbles()
{
    for (int v = 0; v < vertices.size(); v++)
    {
        vertices[v].ClearPebbles();
    }
}

template<class T, class A>
std::ostream& operator<< (std::ostream& os, PebblerHyperGraph<T, A>& graph)
{
    os << "TBC";
    
    return os;
}

///////////////////////////////////////////////////////////////////////////////////
//
// Use Dowling-Gallier pebbling technique to pebble using all given nodes
//
// Pebbling requires TWO phases:
//    1. Pebble in the forward direction any node which is reached once is RED.
//       Any node reached twice is PURPLE and identifies a backward node (via an eventual backward edge)
//       The result of this phase are:
//           a. marked (RED) nodes and (RED) edges for forward analysis through the graph.
//           b. marked (PURPLE) nodes (no edges).
//    2. Using the purple nodes as a starting point, we pebble the graph in a backward direction.
//       This is the same algorithm as pebbling forward (RED) edges, but this time we color the edges BLUE for backward.
//       The result of this phase are:
//           a. All applicable nodes marked BLUE or PURPLE.
//           b. All applicable backward edges marked BLUE.
//
template<class T, class A>
void PebblerHyperGraph<T, A>::Pebble(const std::vector<int>& linkers, const std::vector<int>& rigids)
{
    std::vector<int> nodesToPebble;

    //
    // Construct the list 
    //
    for (std::vector<int>::iterator it = linkers.begin(); it != linkers.end(); it++)
    {
        nodesToPebble.push_back(*it);
    }

    for (std::vector<int>::iterator it = rigids.begin(); it != rigids.end(); it++)
    {
        nodesToPebble.push_back(*it);
    }
    
    std::sort(nodesToPebble.begin(), nodesToPebble.end());


    // Forward pebble: it acquires the valid list of forward edges 
    ForwardTraversal(forwardEdges, nodesToPebble);

    // Backward pebble: acquires the valid list of bakcward edges 
    PebbleBackward(nodesToPebble.size());
}

//
// Pebble the graph in the backward direction using all pebbled nodes from the forward direction.
// Note: we do so in a descending order (opposite the way we did from the forward direction); this attempts to 
//
template<class T, class A>
void PebblerHyperGraph<T, A>::PebbleBackward(int numLinkerRigids)
{
    //
    // Acquire all nodes which are to be pebbled (reachable during forward analysis)
    //
    std::vector<int> deducedNodesToPebbleBackward;

    // Avoid re-pebbling figure again so start after the figure
    for (int v = vertices.size() - 1; v >= numLinkerRigids; v--)
    {
        if (IsNodePebbled(v))
        {
            deducedNodesToPebbleBackward.push_back(v);
        }
    }

    // Clear all pebbles (nodes and edges)
    ClearPebbles();

    //
    // Pebble the graph in the backward direction using all pebbled nodes from the forward direction.
    // Note: we do so in a descending order (opposite the way we did from the forward direction)
    // We create an ascending list and will pull from the back of the list
    //
    ForwardTraversal(backEdges, deducedNodesToPebbleBackward);
}

//
// Given a node, pebble the reachable parts of the graph (in the forward direction)
// We pebble in a breadth first manner
//
template <class T, class A>
void PebblerHyperGraph<T, A>::ForwardTraversal(HyperEdgeMultiMap<A>& edgeDatabase, const std::vector<int>& nodesToPebble)
{
    std::deque<int> worklist;
    
    for (std::vector<int>::iterator it = nodesToPebble.begin(); it != nodesToPebble.end(); it++)
    {
        worklist.push_back(*it);
    }

    //
    // Pebble until the list is empty
    //
    while (!worklist.empty())
    {
        // Acquire the next value to consider
        int currentNodeIndex = worklist.front();
        worklist.pop_front();

        // Pebble the current node as a forward node and percolate forward
        vertices[currentNodeIndex].pebble = true;

        // For all hyperedges leaving this node, mark a pebble along the arc
        for (int e = 0; e < vertices[currentNodeIndex].edges.size(); e++)
        {
            PebblerHyperEdge<A> currentEdge = vertices[currentNodeIndex].edges[e];

            if (!currentEdge.IsFullyPebbled())
            {
                // Indicate the node has been pebbled by adding to the list of pebbled vertices; should not have to be a unique addition
                currentEdge.sourcePebbles[currentEdge.SourceIndex(currentNodeIndex)] = true;

                // With this new node, check if the edge is full pebbled; if so, percolate
                if (currentEdge.IsFullyPebbled())
                {
                    // Has the target of this edge been pebbled previously? Pebbled -> Pebbled means we have a backward edge
                    if (!IsNodePebbled(currentEdge.targetNode))
                    {
                        // Success, we have a forward edge
                        // Construct a static set of pebbled hyperedges for problem construction
                        edgeDatabase.Put(currentEdge);

                        // Add this node to the worklist to percolate further
                        if (!Contains(worklist, currentEdge.targetNode))
                        {
                            worklist.push_back(currentEdge.targetNode);
                            std::sort(worklist.begin(), worklist.end());
                        }
                    }
                }
            }
        }
    }
}

#endif