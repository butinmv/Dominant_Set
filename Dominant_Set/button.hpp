#ifndef button_hpp
#define button_hpp

#include "programmObject.hpp"

enum State {up, down, pressed, released};

class Button : public ProgramObject
{
private:
    RectangleShape *rect_;
    Font *font_;
    Text *text_;
    State state_;
    Color color_;
    Color col_;
    Color color_act_;
    
public:
    Button(float x, float y, float width, float height, Color color, Color color_act, std::string text, int f = 1);
    ~Button(void);
    
    void setText(std::string text);
    void setFont(int size);
    
    void draw(RenderWindow &window);
    bool update(RenderWindow *window);
    void control(RenderWindow *window);
};

#endif
