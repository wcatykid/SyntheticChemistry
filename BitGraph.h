#ifndef _BIT_GRAPH_GUARD
#define _BIT_GRAPH_GUARD 1

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

#include "Utilities.h"

//
// Implements an undrected graph using bits for representation of nodes and edges as follows:
//
//  Let G(V, E) be a graph G(Vertices, Edges)
//
//   Each node is stored as:
//
//   | -- id -- | ----  edge-count ----| ... edges ... |
// 
// Nodes:
//   a) Each node is a triple: <node id number, edge count, edges>
//          * Node number is the identifier of the node.
//          * count is the number of edges associated with this node: |E|_n
//   b) Uses log (largest vertex id) bits for each node identifier.
//
// Edges:
//   a) undirected
//   b) each edge is stored twice (source and target), but this is total 2log(largest vertex id) bits
//   c) An edge is log (largest vertex id) bits for each edge-'pair'.
//

class BitGraph
{
  public:
    BitGraph()
    {
      numNodes = 0;
      numEdges = 0;
      maxNodeValue = 1; // To avoid log (0) 
      maxVertexDegree = 0;
      graph = new vector<bool>();
    }
    
    BitGraph(BitGraph&);

    ~BitGraph()
    {
    }

    bool contains(unsigned int);
    bool contains(unsigned int, unsigned int);
    bool addNode(unsigned int);
    bool addEdge(unsigned int, unsigned int);

    // For simple output of the graph (ala toString())    
    friend ostream& operator<< (ostream &out, BitGraph& graph);

    // For interest
    unsigned int bitsize() { return graph->size(); }
    unsigned int nodeCardinality() { return numNodes; }
    unsigned int edgeCardinality() { return numEdges; }
    
  private:
    vector<bool>* graph;
    unsigned int numNodes;
    unsigned int numEdges;

    // Store the largest node value of all the nodes;
    // we need it for the number of bits
    unsigned int maxNodeValue;

    // log (maxDegree) gives the width of the edge count field.
    unsigned int maxVertexDegree;

    // A simple reference record for node information
    struct Node
    {
        Node(unsigned int i, unsigned int ec) : id(i), edgeCount(ec) {}
        unsigned int id;
        unsigned int edgeCount;
        vector<unsigned int> edges;
    };

    // Insert the given node (with no edges) into the graph node partition.
    void insertNode(Node* node);
     
    // Insert the specified number of bits (numBits) into the vector starting at the specified position (offset)
    // Note: This DOES NOT update any offsets.
    void push_front(unsigned int value, unsigned int numBits);
    
    // Push a specified number of bits to the end of the graph structure.
    void push_back(vector<bool>* const structure, unsigned int value, unsigned int numBits);

    // Main containment determination
    Node* containsSub(unsigned int id);
    // Main addition code
    Node* addNodeSub(unsigned int id);

    // A simple determination of the number of bits required to store node values; minimum value is 1 bit.
    unsigned int sizeofNode() { return numBinaryBits(maxNodeValue); }
    unsigned int sizeofEdgeCount() { return numBinaryBits(maxVertexDegree); }

    // How many bits are required for an edge? For a node?
    int sizeofEdge() { return sizeofNode(); }
    
    // Get a certain number of bits at the specific offset; convert it to an integer
    unsigned int get(unsigned int offset, unsigned int numBits);

    // Acquire a node record from the stream.
    Node* getNode(unsigned int id); // edges
    Node* getBasicNode(unsigned int id); // no edges

    // Given a node, acquire all the edges.
    vector<unsigned int> getEdges(unsigned int id);
};

#endif