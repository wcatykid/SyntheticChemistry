#ifndef _EDGE_ANNOTATION_GUARD
#define _EDGE_ANNOTATION_GUARD 1

#include <string>

class EdgeAnnotationT
{
  public:
    std::string justification;
    bool active;

    EdgeAnnotationT()
    {
        justification = "";
        active = false;
    }

    EdgeAnnotationT(const std::string& just, int act) : justification(just), active(act) { }
    bool IsActive() { return active; }
};

#endif