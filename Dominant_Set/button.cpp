#include "button.hpp"
#include <iostream>

using namespace std;

Button::Button(float x, float y, float width, float height, Color color, Color color_act, string text, int f) : ProgramObject(x, y, width, height)
{
    state_ = up;
    rect_ = new RectangleShape(Vector2f(width_, height_));
    color_ = col_ = color;
    color_act_ = color_act;
    rect_->setFillColor(color);
    rect_->setPosition(Vector2f(x, y));
    
    
    font_ = new Font();
    font_->loadFromFile("cour.ttf");
    text_ = new Text("", *font_, 55);
    text_->setString(text);
    if(f)
        text_->setPosition(x_ + (width_ - text_->getString().getSize() * 30) / 2, y_);
    else
        text_->setPosition(x_ + 5, y_);
}


Button::~Button(void)
{
}


void Button::setText(std::string text)
{
    text_->setString(text);
}

void Button::setFont(int size)
{
    text_->setCharacterSize(size);
}


void Button::draw(RenderWindow &window)
{
    rect_->setFillColor(color_);
    if (color_ == color_act_)
        text_->setFillColor(Color(245, 245, 245));
    else
        text_->setFillColor(Color(245, 245, 245));
    window.draw(*rect_);
    window.draw(*text_);
}


bool Button::update(RenderWindow *window)
{
    control(window);
    int ret = 0;
    switch (state_)
    {
        case up:
            break;
        case down:
            state_ = released;
            break;
        case pressed:
            state_ = down;
            ret = 1;
            break;
        case released:
            state_ = up;
            break;
    }
    
    
    return ret;
}


void Button::control(RenderWindow *window)
{
    
    if (x_ <= (Mouse::getPosition(*window)).x && x_ + width_ >= (Mouse::getPosition(*window)).x && y_ <= (Mouse::getPosition(*window)).y && y_ + height_ >= (Mouse::getPosition(*window)).y)
        color_ = color_act_;
    else
        color_ = col_;
    
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        if (x_ <= (Mouse::getPosition(*window)).x && x_ + width_ >= (Mouse::getPosition(*window)).x && y_ <= (Mouse::getPosition(*window)).y && y_ + height_ >= (Mouse::getPosition(*window)).y)
        {
            if (state_ == up)
                state_ = pressed;
            else
                state_ = down;
        }
    }
}
