#include "programmObject.hpp"
#include <string.h>

ProgramObject::ProgramObject(float x, float y, float width, float height)
{
    x_ = x;
    y_ = y;
    width_ = width;
    height_ = height;
}


ProgramObject::~ProgramObject(void)
{
}
