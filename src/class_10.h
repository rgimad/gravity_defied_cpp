#pragma once

#include <vector>

#include "TimerOrMotoPartOrMenuElem.h"

class class_10 {
public:
    bool unusedBool;
    int field_257;
    int field_258;
    int field_259;
    int field_260;
    std::vector<std::unique_ptr<TimerOrMotoPartOrMenuElem>> motoComponents = std::vector<std::unique_ptr<TimerOrMotoPartOrMenuElem>>(6);

    class_10();
    ~class_10();
    void reset();
};