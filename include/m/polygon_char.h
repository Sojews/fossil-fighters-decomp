#ifndef M_POLYGON_CHAR_H
#define M_POLYGON_CHAR_H

#include "m/task.h"

class MPolygonChar : public MProcess {
public:
    virtual ~MPolygonChar();
    virtual void Idle();
};

#endif
