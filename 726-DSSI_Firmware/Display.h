#pragma once

#include <Arduino.h>

enum DisplayParameter
{
    DISP_NONE,
    DISP_CUTOFF,
    DISP_RESONANCE,
    DISP_ATTACK,
    DISP_DECAY,
    DISP_SUSTAIN,
    DISP_RELEASE,
    DISP_DETUNE,
    DISP_VOLUME,
    DISP_SOURCEMOD,
    DISP_TEMPO,
    DISP_STEPS
};

class Display
{
public:

    void begin();

    void update();

    void showPopup(const String &label,
               const String &value);

private:

    //int lastVoice = -1;
    String popupLabel = "";
    String popupValue = "";
};

extern Display OLED;