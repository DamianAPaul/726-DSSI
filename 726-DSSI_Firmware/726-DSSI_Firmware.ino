#include "AudioObjects.h"
#include "AudioSystem.h"
#include "Controls.h"
#include "AudioSystem.h"
#include "Voice.h"
#include "Display.h"
#include "Keyboard.h"
#include "Sequencer.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 3000);
    Serial.println("Starting Synth...");

    Voices.begin();
    initAudio();

    Controls::begin();
    OLED.begin();
    Keyboard::begin();
    Sequencer::begin();
}

void loop()
{
    Controls::update();
    OLED.update();
    Keyboard::update();
    Keyboard::update();
    Sequencer::update();
}
