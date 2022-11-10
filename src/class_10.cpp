#include "class_10.h"

class_10::class_10() : motoComponents(6, TimerOrMotoPartOrMenuElem()) {
    reset();
}

class_10::~class_10() {
    //
}

void class_10::reset() {
    field_257 = field_259 = field_260 = 0;
    unusedBool = true;
    for (int i = 0; i < 6; ++i) {
        motoComponents[i].setToZeros();
    }
}
