#include "Display.h"
#include "Voice.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Sequencer.h"
#include "Controls.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire2,
    -1
);

Display OLED;


static uint32_t lastPopupTime = 0;
static bool popupActive = false;
String currentMessage = "";


void Display::begin()
{
    if(!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
    {
        Serial.println("OLED FAILED");
        return;
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
    Serial.println("OLED READY");
}


const char* waveformName(WaveformType w)
{
    switch(w)
    {
        case SINE:      return "Sine";
        case SAW:       return "Saw";
        case SQUARE:    return "Square";
        case TRIANGLE:  return "Triangle";
        case NOISE:     return "Noise";
    }

    return "";
}
const char* filterName(FilterType f)
{
    switch(f)
    {
        case LOWPASS: return "LP";

        case HIGHPASS: return "HP";

        case BANDPASS: return "BP";
    }

    return "";
}
const char* modeName()
{
    switch(Sequencer::getMode())
    {
        case STOPPED:     return "STOP";
        case PLAYING:     return "PLAY";
        case LIVE_RECORD: return "RECORD";
        case STEP_EDIT:   return "STEP EDIT";
    }

    return "STOP";
}

void Display::showPopup(const String &label,
                        const String &value)
{
    popupLabel = label;
    popupValue = value;

    popupActive = true;
    lastPopupTime = millis();
}




void Display::update()
{
    if(millis()-lastPopupTime>2000)
    {
        popupActive = false;

        popupLabel = "";
        popupValue = "";
    }
    
    display.clearDisplay();

    //-------------------------
    // Prepare strings
    //-------------------------


    String line1 = "VOICE " + String(Voices.getCurrentVoice() + 1);
    String line2 = popupLabel;
    String line3 = popupValue;

    // Line 1

    display.setTextSize(1);

    int16_t x1 = (SCREEN_WIDTH - line1.length() * 6) / 2;

    display.setCursor(x1, 0);
    display.print(line1);

    // Line 2 & Line 3

    if(popupActive)
    {
        display.setTextSize(2);

        int16_t x2 =
            (SCREEN_WIDTH-line2.length()*12)/2;

        display.setCursor(x2,16);
        display.print(line2);

        int16_t x3 =
            (SCREEN_WIDTH-line3.length()*12)/2;

        display.setCursor(x3,38);
        display.print(line3);
    }
    else if(Sequencer::getMode() == STEP_EDIT)
    {
        line2 = "Step";
        line3 = String(Sequencer::currentStep() + 1) +
                "/" +
                String(Sequencer::getLength());

        display.setTextSize(2);

        int16_t x2 =
            (SCREEN_WIDTH-line2.length()*12)/2;

        display.setCursor(x2,16);
        display.print(line2);

        int16_t x3 =
            (SCREEN_WIDTH-line3.length()*12)/2;

        display.setCursor(x3,38);
        display.print(line3);
    }

    // Line 4

    display.setTextSize(1);

    display.setCursor(0, 56);
    display.print(modeName());

    String steps = String(Sequencer::getLength()) + " STEP";

    int16_t x4 = SCREEN_WIDTH - steps.length() * 6;

    display.setCursor(x4, 56);
    display.print(steps);

    display.display();
}
