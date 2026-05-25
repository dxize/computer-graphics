#pragma once

class Drawable
{
public:
    virtual ~Drawable() = default;

    virtual void build() = 0;
    virtual void draw() const = 0;
};
