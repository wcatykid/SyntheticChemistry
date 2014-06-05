#ifndef _ATOM_GUARD
#define _ATOM_GUARD 1

#include <string>
#include <vector>

class Atom
{
  private:
    int atomID;
    std::string atomType;
    bool canConnect;
    int maxConnect;
    std::string connectType;
    std::vector<int> bonds;
    std::vector<std::string> connectionType;

  public:
    //
    // Get functions
    //
    int getAtomID() const { return this->atomID; }
    std::string getAtomType() const { return this->atomType; }
    bool getCanConnect() const { return this->canConnect; }
    int getMaxConnect() const { return this->maxConnect; }
    std::string getConnectType() const { return this->connectType; }
    int getBond(int index) const { return this->bonds[index]; }
    std::string getConnectionType(int index) const { return this->connectionType[index]; }

    //
    // Set Functions
    //
    void setAtomID(int x) { this->atomID = x; }
    void setAtomType(std::string x) { this->atomType = x; }
    void setCanConnect(bool x) { this->canConnect = x; }
    void setMaxConnect(int x) { this->maxConnect = x; }
    void setConnectType(std::string x) { this->connectType = x; }

    int numberOfBonds() const { return this->bonds.size(); }
    int numberOfConnectionTypes() const { return this->connectionType.size(); }

    Atom(int id, const std::string& aType, int maxConn, const std::string& connType);
    ~Atom() {}

    bool addBond (Atom& a);
    void pushBond(int x);
    void pushConnectionType(const std::string& x);
};

#endif