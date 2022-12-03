#include "TimerOrMotoPartOrMenuElem.h"

TimerOrMotoPartOrMenuElem::TimerOrMotoPartOrMenuElem() {
    setToZeros();
}

TimerOrMotoPartOrMenuElem::TimerOrMotoPartOrMenuElem(int timerNo, Micro *micro) {
    this->micro = micro;
    this->timerNo = timerNo;
}

// TimerOrMotoPartOrMenuElem::TimerOrMotoPartOrMenuElem(char *text, GameMenu *gameMenu, IMenuManager *menuManager) {
//     strcpy(this->text, text);
//     strcat(this->text, ">");
//     this->gameMenu = gameMenu;
//     this->menuManager = menuManager;
// }

void TimerOrMotoPartOrMenuElem::setToZeros() {
    xF16 = yF16 = angleF16 = 0;
    field_382 = field_383 = field_384 = 0;
    field_385 = field_386 = field_387 = 0;
}

void TimerOrMotoPartOrMenuElem::run() {
    // micro->gameCanvas.method_150(timerNo); // TODO
}

void TimerOrMotoPartOrMenuElem::setText(char *text) {
    strcpy(this->text, text);
    strcat(this->text, ">");
}

char* TimerOrMotoPartOrMenuElem::getText() {
    return text;
}

bool TimerOrMotoPartOrMenuElem::isNotTextRender() {
    return true;
}

void TimerOrMotoPartOrMenuElem::menuElemMethod(int var1) {
    // TODO
    // switch (var1) {
    //     case 1:
    //     case 2:
    //         menuManager->processMenu(this);
    //         gameMenu->setGameMenu(menuManager->getGameMenu());
    //         menuManager->method_1(gameMenu, false);
    //     case 3:
    //     default:
    // }
}

// void TimerOrMotoPartOrMenuElem::setGameMenu(GameMenu *gameMenu) {
//     this->gameMenu = gameMenu;
// }

// void TimerOrMotoPartOrMenuElem::render(Graphics *graphics, int y, int x) {
//     graphics->drawString(text, x, y, 20);
// }