#include <vector>
#include <queue>
#include <iostream>

#include "Molecule.h"
#include "Rigid.h"
#include "Linker.h"

#include "HyperGraph.h"
#include "HyperEdge.h"
#include "EdgeAggregator.h"

#include "Instantiator.h"
#include "Utilities.h"

//
// Add the hyperedge to the hypergraph
//
void Instantiator::AddEdge(const std::vector<Molecule>& antecedent, const Molecule& consequent, const EdgeAnnotationT& annotation)
{
    graph.AddEdge(antecedent, consequent, annotation);
}

//
// Main instantiation function for all figures stated in the given list; worklist technique to construct the graph
//
HyperGraph<Molecule, EdgeAnnotationT> Instantiator::Instantiate(std::vector<Linker>& linkers, std::vector<Rigid>& rigids)
{
    // The worklist initialized to initial set of ground clauses from the figure
    std::queue<Molecule> worklist;

    // Add all of the rigids to the worklist
    // Add the rigids to the hypergraph
    // Set the graph ID on the rigids AND the graph
    for (int r = 0; r < rigids.size(); r++)
    {
        worklist.push(rigids[r]);
        rigids[r].setGraphID(r);
        graph.AddNode(rigids[r]);
    }
    
    // Add all of the linkers to the worklist AND the graph
    for (int ell = 0; ell < linkers.size(); ell++)
    {
        worklist.push(linkers[ell]);
        linkers[ell].setGraphID(ell);
        graph.AddNode(linkers[ell]);
    }

    //
    // Process all molecules until the worklist is empty: fixed point of combining of all linkers, rigids, and new molecules.
    //
    while (!worklist.empty())
    {
        // Acquire the first molecule from the list for processing
        Molecule currentMolecule = worklist.front();
        worklist.pop();

        if (HYPERGRAPH_CONSTR_DEBUG) ds << "Working on: " << currentMolecule.getGraphID() << " " << currentMolecule;

        //
        // Take the current molecule and apply to all 'completed' mmolecules (which are the nodes in the hypergraph)
        //
        int currentGraphSize = graph.size();
        for (int n = 0; n < currentGraphSize; n++)
        {
            std::vector<EdgeAggregator> newEdges = currentMolecule.Compose(graph.vertices[n].data);

            HandleNewMolecules(worklist, newEdges);
        }
    }
    return graph;
}

//
// Add all new deduced clauses to the worklist if they have not been deduced before.
// If the given clause has been deduced before, update the hyperedges that were generated previously
//
// Forward Instantiation does not permit any cycles in the resultant graph.
//
void Instantiator::HandleNewMolecules(std::queue<Molecule>& worklist, std::vector<EdgeAggregator>& newEdges)
{
    for (int e = 0; e < newEdges.size(); e++)
    {
        //Debug.WriteLine(newEdge.Value.clauseId + "(" + graph.Size() + ")" + ": " + newEdge.Value);

        try
        {
            Molecule graphNode = graph.GetNode(newEdges[e].consequent);

            // Molecule is in the graph
            AddEdge(newEdges[e].antecedent, graphNode, newEdges[e].annotation);
        }

        // The new consequent Molecule is not in the graph
        catch(int)
        {
            // Add the node to the graph and set its id
            if (graph.AddNode(newEdges[e].consequent))
            {
                newEdges[e].consequent.setGraphID(graph.size());
            }

            // Also add to the worklist
            worklist.push(newEdges[e].consequent);

            // Add the actual edge
            AddEdge(newEdges[e].antecedent, newEdges[e].consequent, newEdges[e].annotation);
        }
    }
}