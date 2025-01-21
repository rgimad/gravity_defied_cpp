#include "Canvas.h"

Canvas::Canvas()
{
    impl = std::make_unique<CanvasImpl>(this);
    graphics = std::make_unique<Graphics>(impl->getRenderer());
}

int Canvas::getWidth()
{
    return impl->getWidth();
}

int Canvas::getHeight()
{
    return impl->getHeight();
}

void Canvas::setWindowTitle(const std::string& title)
{
    impl->setWindowTitle(title);
}

bool Canvas::isShown()
{
    return true;
}

CanvasImpl* Canvas::getCanvasImpl()
{
    return impl.get();
}

void Canvas::repaint()
{
    // IMPROVE two lines below is a hack to make the game work in one thread
    paint(graphics.get());
    impl->processEvents();

    impl->repaint();
}

void Canvas::serviceRepaints()
{
}

void Canvas::removeCommand(Command* command)
{
    currentCommands.erase(command);
}

void Canvas::addCommand(Command* command)
{
    currentCommands.insert(command);
}

void Canvas::setCommandListener(CommandListener* listener)
{
    commandListener = listener;
}

void Canvas::publicKeyPressed(const Keys keyCode)
{
    keyPressed(keyCode);
}

void Canvas::publicKeyReleased(const Keys keyCode)
{
    keyReleased(keyCode);
}

void Canvas::pressedEsc()
{
    for (const auto& command : currentCommands) {
        if (command->type == Command::Type::BACK  || (!Micro::isInGameMenu && currentCommands.size() == 1)) {
            commandListener->commandAction(command, this);
            return;
        }
    }
}