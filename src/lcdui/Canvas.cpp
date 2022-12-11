#include "Canvas.h"

#include <stdexcept>
#include <algorithm>

#include "CanvasImpl.h"
#include "Graphics.h"
#include "Command.h"
#include "CommandListener.h"

Canvas::Canvas() {
    impl = std::make_unique<CanvasImpl>(this);
    graphics = std::make_unique<Graphics>(impl->getRenderer());
}

Canvas::~Canvas() {

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
    // IMPROVE two lines below is a hack to make the game work in one thread
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
    auto f = [&command](const Command *i){return *i == *command;};
    auto position = std::find_if(currentCommands.cbegin(), currentCommands.cend(), f);
    if (position != currentCommands.cend()) {
        currentCommands.erase(position);
    }
}

void Canvas::addCommand(Command *command) {
    auto f = [&command](const Command *i){return *i == *command;};
    if (std::find_if(currentCommands.cbegin(), currentCommands.cend(), f) == currentCommands.cend()) {
        currentCommands.push_back(command);
    }
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

void Canvas::pressedEsc() {
    for (auto command = currentCommands.begin(); command != currentCommands.end(); command++) {
        if ((*command)->type == Command::Type::BACK || currentCommands.size() == 1) {
            commandListener->commandAction(*command, this);
            return;
        }
    }
}