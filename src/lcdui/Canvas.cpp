#include "Canvas.h"

#include <stdexcept>

#include "CanvasImpl.h"
#include "Graphics.h"

Canvas::Canvas() {
    impl = std::make_unique<CanvasImpl>(this);
    graphics = std::make_unique<Graphics>(impl->getRenderer());
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
    paint(graphics.get());
    impl->processEvents();
    impl->repaint();
}

void Canvas::serviceRepaints() {

}

int Canvas::getGameAction(int keyCode) {
    switch (keyCode) {
        case Keys::UP:
        case Keys::DOWN:
        case Keys::LEFT:
        case Keys::RIGHT:
        case Keys::FIRE:
            return keyCode;
        default:
            throw std::runtime_error("getGameAction(" + std::to_string(keyCode) + ") isn't implemented!");
    }
}

void Canvas::removeCommand(Command *command) {
    // TODO
}

void Canvas::addCommand(Command *command) {
    // TODO
}

void Canvas::setCommandListener(CommandListener *listener) {
    commandListener = listener;
}

void Canvas::publicKeyPressed(int keyCode) {
    keyPressed(keyCode);
}

void Canvas::publicKeyReleased(int keyCode) {
    keyReleased(keyCode);
}