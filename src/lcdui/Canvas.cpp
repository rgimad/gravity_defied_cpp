#include "Canvas.h"

#include "CanvasImpl.h"

Canvas::Canvas() {
    impl = std::make_unique<CanvasImpl>(this);
}

Canvas::~Canvas() {
    // TODO
}

int Canvas::getWidth() {
    return impl->getWidth();
}

int Canvas::getHeight() {
    return impl->getHeight();
}

bool Canvas::isShown() {
    return true;
}

CanvasImpl* Canvas::getCanvasImpl() {
    return impl.get();
}

void Canvas::repaint() {
    impl->repaint();
}

void Canvas::serviceRepaints() {

}