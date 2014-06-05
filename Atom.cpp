#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>

#include "Atom.h"

/***************************************************************************************************************/

Atom::Atom(int id, const std::string& aType,
           int maxConn, const std::string& connType) : atomID(id),
                                                       atomType(aType),
                                                       maxConnect(maxConn),
                                                       canConnect(maxConn > 0),
                                                       connectType(connType)
{
}

/***************************************************************************************************************/

void Atom::pushBond(int x)
{
    this->bonds.push_back(x);
    if(this->bonds.size() == maxConnect)
        this->canConnect = false;
}

/***************************************************************************************************************/

void Atom::pushConnectionType(const std::string& x)
{
    this->connectionType.push_back(x);
}

/***************************************************************************************************************/

bool Atom::addBond(Atom& a)
{ 
    //
    // Can both atoms connect?
    //
    if(!this->canConnect) return false;

    if(!a.getCanConnect()) return false;

    //
    // search for matching connection types
    //
    bool canConnectToThis = false;
    for(int x = 0; x < this->numberOfConnectionTypes(); x++)
    {
        if(this->getConnectionType(x) == a.getConnectType())
        {
            canConnectToThis = true;
            break;
        }
    }

    if(!canConnectToThis) return false;

    canConnectToThis = false;
    for(int x = 0; x < a.numberOfConnectionTypes(); x++)
    {
        if(a.getConnectionType(x) == this->getConnectType())
        {
            canConnectToThis = true;
            break;
        }
    }

    if(!canConnectToThis) return false;

    // the connection can take place, so connect
    this->pushBond(a.getAtomID());
    a.pushBond(this->getAtomID());
    
    return true;

} /* addBond */