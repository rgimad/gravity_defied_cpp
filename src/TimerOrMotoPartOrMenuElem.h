#pragma once

class TimerOrMotoPartOrMenuElem { // TODO  extends TimerTask implements IGameMenuElement
private:
    String *text;
    GameMenu *gameMenu;
    IMenuManager menuManager;

public:
    int xF16;
    int yF16;
    int angleF16;
    int field_382;
    int field_383;
    int field_384;
    int field_385;
    int field_386;
    int field_387;
    int timerNo;
    Micro *micro;

    TimerOrMotoPartOrMenuElem() {
        setToZeros();
    }

    void setToZeros() {
        xF16 = yF16 = angleF16 = 0;
        field_382 = field_383 = field_384 = 0;
        field_385 = field_386 = field_387 = 0;
    }

    TimerOrMotoPartOrMenuElem(int timerNo, Micro *micro) {
        this->micro = micro;
        this->timerNo = timerNo;
    }

    void run() {
        micro->gameCanvas.method_150(timerNo);
    }

    TimerOrMotoPartOrMenuElem(String *text, GameMenu *gameMenu, IMenuManager *menuManager) {
        String::copy(this->text, text)
        String::append(this->text, ">");
        this->gameMenu = gameMenu;
        this->menuManager = menuManager;
    }

    void setText(String *text) {
        String::copy(this->text, text)
        String::append(this->text, ">");
    }

    String *getText() {
        return text;
    }

    bool isNotTextRender() {
        return true;
    }

    void menuElemMethod(int var1) {
        switch (var1) {
            case 1:
            case 2:
                menuManager->processMenu(this);
                gameMenu->setGameMenu(menuManager.getGameMenu());
                menuManager->method_1(gameMenu, false);
            case 3:
            default:
        }
    }

    void setGameMenu(GameMenu *gameMenu) {
        this->gameMenu = gameMenu;
    }

    void render(Graphics *graphics, int y, int x) {
        graphics->drawString(text, x, y, 20);
    }
};