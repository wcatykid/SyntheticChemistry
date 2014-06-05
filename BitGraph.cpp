#include <vector>
//#include <set>
#include <algorithm> // for ::find
#include <iostream>

#include "BitGraph.h"
#include "Utilities.h"

BitGraph::BitGraph(BitGraph& that) : graph(that.graph), numNodes(that.numNodes),
                                     numEdges(that.numEdges), maxNodeValue(that.maxNodeValue)
{
}
//
// Acquire a node record from the stream (with edges)
// This is O(|V|) operation.
//
BitGraph::Node* BitGraph::getNode(unsigned int id)
{
    //
    // Iterate over the nodes
    //
    int offset = 0;
    bool found = false;
    for (unsigned int n = 0; n < numNodes; n++)
    {
        if (id == get(offset, sizeofNode()))
        {
            found = true;
            break;
        }
        offset += sizeofNode();

        unsigned int edgeCount = get(offset, sizeofEdgeCount());
        offset += sizeofEdgeCount() + sizeofEdge() * edgeCount;
    }
    
    if (!found) return null;

    //
    // Extract the edge count; extract that number of edges from the stream.
    //
    offset += sizeofNode();
    Node* node = new Node(id, get(offset, sizeofEdgeCount()));

    offset += sizeofEdgeCount();
    for (unsigned int e = 0; e < node->edgeCount; e++, offset += sizeofNode())
    {
        node->edges.push_back(get(offset, sizeofNode()));
    }
    
    return node;
}

//
// Acquire a node record from the stream (no edges)
// This is O(|V|) operation.
//
BitGraph::Node* BitGraph::getBasicNode(unsigned int id)
{
    //
    // Iterate over the nodes
    //
    int offset = 0;
    bool found = false;
    for (unsigned int n = 0; n < numNodes; n++)
    {
        if (id == get(offset, sizeofNode()))
        {
            found = true;
            break;
        }
        offset += sizeofNode();

        unsigned int edgeCount = get(offset, sizeofEdgeCount());
        offset += sizeofEdgeCount() + sizeofEdge() * edgeCount;
    }

    if (!found) return null;

    //
    // Extract the edge count; extract that number of edges from the stream.
    //
    offset += sizeofNode();
    Node* node = new Node(id, get(offset, sizeofEdgeCount()));
    
    return node;
}

//
// Given a node, acquire all the edges.
//
vector<unsigned int> BitGraph::getEdges(unsigned int id)
{
    Node* node = getNode(id);

    vector<unsigned int> edges;
    return node == null ? edges : node->edges;
}


//
// Does this graph contain this node?
//
bool BitGraph::contains(unsigned int id)
{
    return containsSub(id) != null;
}

//
// Does this graph contain this node?
//
BitGraph::Node* BitGraph::containsSub(unsigned int id)
{
    // Check if the node is larger than the max node; if it is, it's not in the graph.
    if (numBinaryBits(id) > sizeofNode()) return null;

    return getBasicNode(id);
}

//
// Does this graph contain this edge? Edges are stored twice (to and from)
//
bool BitGraph::contains(unsigned int from, unsigned int to)
{
    Node* node = getNode(from);

    if (node == null) return false;
    
    return (find(node->edges.begin(), node->edges.end(), to) != node->edges.end());
}

//
// Add the edge twice: one for each (to and from)
//
// a) Update the edge count for <from, to> nodes
// b) Add the edge information.
//
// For efficiency, we are not calling addNode in this method.
//
bool BitGraph::addEdge(unsigned int from, unsigned int to)
{
    if (this->contains(from, to)) return false;

    //
    // Add the nodes, if necessary and acquire the records.
    //
    Node* fromNode = addNodeSub(from);
    Node* toNode = addNodeSub(to);
    
    // Do we need to elongate the bit field size for edge counts?
    unsigned int newMaxVertexDegree = maxVertexDegree;
    if (fromNode != null)
    {
        if (fromNode->edgeCount == newMaxVertexDegree)
        {
            newMaxVertexDegree++;
        }
    }
    if (toNode != null)
    {
        if (toNode->edgeCount == newMaxVertexDegree)
        {
            newMaxVertexDegree++;
        }
    }

cout << "Old vertex: " << maxVertexDegree << "New: " << newMaxVertexDegree << endl;

    //
    // Traverse the graph and
    //   1) extend the edge count bitfield if needed
    //   2) Add the edge to (to and from)
    //
    vector<bool>* newGraph = new vector<bool>();
    for (unsigned int n = 0, offset = 0; n < numNodes; n++)
    {
        //
        // Read the record
        //
        unsigned int oldId = get(offset, sizeofNode());
        offset += sizeofNode();

        unsigned int edgeCount = get(offset, sizeofEdgeCount());

        offset += sizeofEdgeCount();
        vector<unsigned int> edges;
        for (unsigned int e = 0; e < edgeCount; e++)
        {
            edges.push_back(get(offset, sizeofEdge()));
            offset += sizeofEdge();
        }

cout << "Node: " << oldId << endl << "Edge count: " << edgeCount << ": ";
for (vector<unsigned int>::const_iterator it = edges.begin(); it != edges.end(); it++)
{
    cout << *it << " "; 
}
cout << endl;
        
        //
        // Write the record (increasing edge count bitfield size if needed)
        //
        push_back(newGraph, oldId, sizeofNode());

        // Increment the edge count for this node
        if (oldId == from || oldId == to)
        {
            push_back(newGraph, edgeCount + 1, numBinaryBits(newMaxVertexDegree));
        }
        else
        {
            push_back(newGraph, edgeCount, numBinaryBits(newMaxVertexDegree));
        }

        // Push the edges onto the graph
        for (vector<unsigned int>::const_iterator it = edges.begin(); it != edges.end(); it++)
        {
            push_back(newGraph, *it, sizeofNode());
        }
        // Push the new edge for from and to nodes
        if (oldId == from)
        {
            push_back(newGraph, to, sizeofNode());
        }
        else if (oldId == to)
        {
            push_back(newGraph, from, sizeofNode());
        }
    }

    // Adjust the degree
    maxVertexDegree = newMaxVertexDegree;
    numEdges++;

    // Kill the old graph and keep the new representation.
    delete graph;
    graph = newGraph;
    
    return true;
}

//
// This does not successfully disambiguate between a 'new' node and adding a node successively (without edges).
//    Problem:  add 12 add 12 returns true twice
//              add 12 add (12, 13) add 12 return true then false (successfully)
//
bool BitGraph::addNode(unsigned int id)
{
    Node* node = addNodeSub(id);
    
    if (node == null) return false;

    return node->edgeCount == 0;
}

//
// Add a new node to the end of the stream.
//
BitGraph::Node* BitGraph::addNodeSub(unsigned int id)
{
    Node* old = this->containsSub(id);
    if (old != null) return old;
    
    unsigned int oldNodeBits = sizeofNode();
    unsigned int newNodeBits = numBinaryBits(id);

    //
    // If necessary, increase the number of bits allocated for node id: for all nodes AND edges.
    //
    if (newNodeBits > oldNodeBits && !graph->empty())
    {
        vector<bool>* newGraph = new vector<bool>();
        for (unsigned int n = 0, offset = 0; n < numNodes; n++)
        {
            //
            // Read the record
            //
            unsigned int oldId = get(offset, sizeofNode());
            offset += sizeofNode();

            unsigned int edgeCount = get(offset, sizeofEdgeCount());

            offset += sizeofEdgeCount();
            vector<unsigned int> edges;
            for (unsigned int e = 0; e < edgeCount; e++)
            {
                edges.push_back(get(offset, sizeofEdge()));
                offset += sizeofEdge();
            }
        
            //
            // Write the record (increasing node size)
            //
            push_back(newGraph, oldId, newNodeBits);
            push_back(newGraph, edgeCount, sizeofEdgeCount());
            
            for (vector<unsigned int>::const_iterator it = edges.begin(); it != edges.end(); it++)
            {
                push_back(newGraph, *it, newNodeBits);
            }
        }
        
        // Kill the old graph and keep the new representation.
        delete graph;
        graph = newGraph;
    }

    //
    // Simple add: no increase is needed to the number of bits allocated to node id.    
    //
    // Save the largest node id
    maxNodeValue = id > maxNodeValue ? id : maxNodeValue;
    
    // Add the new node to the graph
    push_back(graph, id, sizeofNode());
    push_back(graph, 0, sizeofEdgeCount());
    numNodes++;

    return new Node(id, 0);
}

//
// Push a specified number of bits to the end of the graph structure.
//
void BitGraph::push_back(vector<bool>* const structure, unsigned int value, unsigned int numBits)
{
    for (unsigned int index = 1; index < numBits; index++)
    {
        structure->push_back((value >> (numBits - index)) & 0x1);
    }

    // pick up last bit
    structure->push_back(value & 0x1);
}

//
// Get a certain number of bits at the specific offset; convert it to an integer
//
unsigned int BitGraph::get(unsigned int offset, unsigned int numBits)
{
    if (offset < 0 || offset + numBits > graph->size()) return -1;

    unsigned int value = (*graph)[offset] & 0x1;
    for (unsigned int index = 1; index < numBits; index++)
    {
        // Multiply by 2 / shift the bit into place.
        value <<= 1;
        
        // Mask in new bit
        value |= ((*graph)[offset + index] & 0x1);
    }
    
    return value;
}

//
// For each node, get the edges and print them.
// This is a toString() style function.
//
ostream& operator<< (ostream& os, BitGraph& graph)
{
    os << "<Nodes, Edges, bits>: <" << graph.numNodes << ", " << graph.numEdges << ", " << graph.bitsize() << ">" << endl;
    os << "Saved " << sizeof(unsigned int) * 8 * (graph.numNodes + graph.numEdges) - graph.bitsize() << " bits" << endl;
    os << "|";
    for (vector<bool>::const_iterator it = graph.graph->begin(); it != graph.graph->end(); it++)
    {
        os << *it;
    }
    os << "|" << endl;

    //
    // Iterate over the nodes
    //
    for (unsigned int n = 0, offset = 0; n < graph.numNodes /* && offset < graph.graph->size() */; n++)
    {
        //
        // Read the record
        //
        unsigned int id = graph.get(offset, graph.sizeofNode());
        offset += graph.sizeofNode();

        unsigned int edgeCount = graph.get(offset, graph.sizeofEdgeCount());

        offset += graph.sizeofEdgeCount();
        vector<unsigned int> edges;
        for (unsigned int e = 0; e < edgeCount; e++, offset += graph.sizeofEdge())
        {
            edges.push_back(graph.get(offset, graph.sizeofEdge()));
        }
        
        //
        // Write the record: "id: edges"
        //
        os << id << ": { ";
        
        for (vector<unsigned int>::const_iterator it = edges.begin(); it != edges.end(); it++)
        {
            os << *it << " ";
        }
        os << " }" << endl;
    }

    return os;
}