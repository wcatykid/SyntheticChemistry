
#include "Bond.h"

/***************************************************************************************************************/

Bond::Bond(int id, int origin, int target, eTypeOfBondT b, eStatusBitT s) : bondID(id),
                                                                            originAtomID(origin),
                                                                            targetAtomID(target),
                                                                            typeOfBond(b),
                                                                            statusBit(s)
{
}
