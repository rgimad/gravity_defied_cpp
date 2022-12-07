#pragma once

#include <memory>

#include "Displayable.h"

class CanvasImpl;
class Graphics;

class Canvas : public Displayable {
private:
    std::unique_ptr<CanvasImpl> impl;

public:
    Canvas();
    ~Canvas();

    bool isShown() override;
    int getWidth() override;
    int getHeight() override;

    CanvasImpl* getCanvasImpl();

    void repaint();
    void serviceRepaints();

    virtual void paint(Graphics *g) = 0;
};