#ifndef programmObject_hpp
#define programmObject_hpp

#include <SFML/Graphics.hpp>

using namespace sf;

class ProgramObject
{
protected:
    float x_;
    float y_;
    float width_;
    float height_;
    
public:
    ProgramObject(float x, float y, float width, float height);
    ~ProgramObject(void);
    
    virtual void draw(RenderWindow &window) = 0;
    virtual bool update(RenderWindow *window) = 0;
};

#endif
