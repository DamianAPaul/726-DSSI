#pragma once

#include <stdint.h>

namespace Keyboard
{
    void begin();
    void update();

    void octaveUp();
    void octaveDown();

    bool notePressed();
    bool noteReleased();

    uint8_t getPressedMidiNote();

    int getOctave();
}