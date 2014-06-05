#ifndef _BOND_GUARD
#define _BOND_GUARD 1

#include <string>
#include <vector>

typedef enum 
{
    /*1=1,
    2=2,
    3=3,*/
    am=4,
    ar=5,
    du=6,
    un=7,
    nc=8
} eTypeOfBondT;
    
typedef enum 
{
    UNASSIGNED=-1,
    TYPECOL=1,
    GROUP=2,
    CAP=3,
    BACKBONE=4,
    DICT=5,
    INTERRES=6
} eStatusBitT;

class Bond
{
  private:
    int bondID;
    int originAtomID;
    int targetAtomID;
    eTypeOfBondT typeOfBond;
    eStatusBitT statusBit;

  public:
    //
    // Get Functions
    //
    int getBondID() const { return this->bondID; }
    int getOriginAtomID() const { return this->originAtomID; }
    int getTargetAtomID() const { return this->targetAtomID; }
    eTypeOfBondT gettypeOfBond() const { return this->typeOfBond; }
    eStatusBitT getstatusBit() const { return this->statusBit; }

    //
    // Set Functions
    //
    void setBondID(int x) { this->bondID = x; }
    void setOriginAtomID(int x) { this->originAtomID = x; }
    void setTargetAtomID(int x) { this->targetAtomID = x; }
    void setTypeOfBond(eTypeOfBondT x) { this->typeOfBond = x; }
    void setstatusBit(eStatusBitT x) { this->statusBit = x; }

    Bond(int id, int origin, int target, eTypeOfBondT b, eStatusBitT s);
    ~Bond() {}

};

#endif