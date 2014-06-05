#include <vector>
#include <queue>
#include <iostream>

//
// Open Babel
//
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

//
// This project molecular representation
//
#include "Molecule.h"
#include "Rigid.h"
#include "Linker.h"


//
// Synthesis-Based Functionality
//
#include "HyperGraph.h"
#include "EdgeAnnotation.h"
#include "Instantiator.h"

#include "PebblerHyperGraph.h"
#include "Utilities.h"


void Cleanup(std::vector<Linker>& linkers, std::vector<Rigid>& rigids);

//
// Read the input data files
//
void readInputFiles(int argc, char** argv,
                    std::vector<Linker>& linkers, std::vector<Rigid>& rigids)
{
    // Input parser functionality for Open babel
    OpenBabel::OBConversion obConversion;
    obConversion.SetInFormat("sdf");

    //
    // For each file
    //
    for (int f = 1; f < argc; f++)
    {
        //
        // For each molecule in this file.
        //
        OpenBabel::OBMol* mol = new OpenBabel::OBMol();
        bool notatend = obConversion.ReadFile(mol, argv[f]);
        while (notatend)
        {
            //
            // Create this particular molecule type.
            //
            // Is there a better way to handle distinguising linkers vs. rigids
            // other than the name of the file (prefix)?
            switch(argv[f][0])
            {
              case 'l':
                linkers.push_back(*(new Linker(*mol)));
                break;

              case 'r':
                rigids.push_back(*(new Rigid(*mol)));
                break;

              default: 
                cerr << "Unexpected file prefix: " << argv[f][0] << " with file " << argv[f][0] << endl; 
            }

            //
            // Read the next molecule in from the file.
            //
            mol = new OpenBabel::OBMol();
            notatend = obConversion.Read(mol);
        }        
    }
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        cerr << "Usage: <program> <SDF-file-list>" << endl;
        return 1;
    }

    std::vector<Linker> linkers;
    std::vector<Rigid> rigids;
    
    readInputFiles(argc, argv, linkers, rigids);

    cerr << "1" << endl;
    
    Instantiator instantiator(cout);
    
    cerr << "2" << endl;
    
    HyperGraph<Molecule, EdgeAnnotationT> graph = instantiator.Instantiate(linkers, rigids);

    cerr << "3" << endl;

    PebblerHyperGraph<Molecule, EdgeAnnotationT> pebblerGraph = graph.GetPebblerHyperGraph();

    cerr << "4" << endl;

    if (DEBUG) cout << graph;

    cerr << "5" << endl;

    Cleanup(linkers, rigids);

    cerr << "6" << endl;

    return 0;
}

void Cleanup(std::vector<Linker>& linkers, std::vector<Rigid>& rigids)
{
    for (int ell = 0; ell < linkers.size(); ell++)
    {
        delete &linkers[ell];
    }

    for (int r = 0; r < rigids.size(); r++)
    {
        delete &rigids[r];
    }
}